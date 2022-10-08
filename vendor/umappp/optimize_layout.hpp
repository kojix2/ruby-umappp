#ifndef UMAPPP_OPTIMIZE_LAYOUT_HPP
#define UMAPPP_OPTIMIZE_LAYOUT_HPP

#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>

#include "NeighborList.hpp"
#include "aarand/aarand.hpp"

namespace umappp {

template<typename Float>
struct EpochData {
    EpochData(size_t nobs) : head(nobs) {}

    int total_epochs;
    int current_epoch = 0;

    std::vector<size_t> head;
    std::vector<int> tail;
    std::vector<Float> epochs_per_sample;

    std::vector<Float> epoch_of_next_sample;
    std::vector<Float> epoch_of_next_negative_sample;
    Float negative_sample_rate;
};

template<typename Float>
EpochData<Float> similarities_to_epochs(const NeighborList<Float>& p, int num_epochs, Float negative_sample_rate) {
    Float maxed = 0;
    size_t count = 0;
    for (const auto& x : p) {
        count += x.size();
        for (const auto& y : x) {
            maxed = std::max(maxed, y.second);
        }
    }

    EpochData<Float> output(p.size());
    output.total_epochs = num_epochs;
    output.tail.reserve(count);
    output.epochs_per_sample.reserve(count);
    const Float limit = maxed / num_epochs;

    size_t last = 0;
    for (size_t i = 0; i < p.size(); ++i) {
        const auto& x = p[i];
        for (const auto& y : x) {
            if (y.second >= limit) {
                output.tail.push_back(y.first);
                output.epochs_per_sample.push_back(maxed / y.second);
                ++last;
            }
        }
        output.head[i] = last;
    }

    // Filling in some epoch-related running statistics.
    output.epoch_of_next_sample = output.epochs_per_sample;
    output.epoch_of_next_negative_sample = output.epochs_per_sample;
    for (auto& e : output.epoch_of_next_negative_sample) {
        e /= negative_sample_rate;
    }
    output.negative_sample_rate = negative_sample_rate;

    return output;       
}

template<typename Float>
Float quick_squared_distance(const Float* left, const Float* right, int ndim) {
    Float dist2 = 0;
    for (int d = 0; d < ndim; ++d, ++left, ++right) {
        dist2 += (*left - *right) * (*left - *right);
    }
    constexpr Float dist_eps = std::numeric_limits<Float>::epsilon();
    return std::max(dist_eps, dist2);
}

template<typename Float>
Float clamp(Float input) {
    constexpr Float min_gradient = -4;
    constexpr Float max_gradient = 4;
    return std::min(std::max(input, min_gradient), max_gradient);
}

template<bool batch, typename Float, class Setup, class Rng> 
void optimize_sample(
    size_t i,
    int ndim,
    Float* embedding,
    Float* buffer,
    Setup& setup,
    Float a,
    Float b,
    Float gamma,
    Float alpha,
    Rng& rng,
    Float epoch
) {
    const auto& head = setup.head;
    const auto& tail = setup.tail;
    const auto& epochs_per_sample = setup.epochs_per_sample;
    auto& epoch_of_next_sample = setup.epoch_of_next_sample;
    auto& epoch_of_next_negative_sample = setup.epoch_of_next_negative_sample;
   
    const size_t num_obs = head.size(); 
    const Float negative_sample_rate = setup.negative_sample_rate;

    size_t start = (i == 0 ? 0 : setup.head[i-1]), end = setup.head[i];
    Float* left = embedding + i * ndim;

    for (size_t j = start; j < end; ++j) {
        if (epoch_of_next_sample[j] > epoch) {
            continue;
        }

        Float* right = embedding + tail[j] * ndim;
        Float dist2 = quick_squared_distance(left, right, ndim);
        const Float pd2b = std::pow(dist2, b);
        const Float grad_coef = (-2 * a * b * pd2b) / (dist2 * (a * pd2b + 1.0));
        {
            Float* lcopy = left;
            Float* rcopy = right;

            for (int d = 0; d < ndim; ++d, ++lcopy, ++rcopy) {
                Float gradient = alpha * clamp(grad_coef * (*lcopy - *rcopy));
                if constexpr(!batch) {
                    *lcopy += gradient;
                    *rcopy -= gradient;
                } else {
                    // Doubling as we'll assume symmetry from the same
                    // force applied by the right node. This allows us to
                    // avoid worrying about accounting for modifications to
                    // the right node.
                    buffer[d] += 2 * gradient;
                }
            }
        }

        // Here, we divide by epochs_per_negative_sample, defined as epochs_per_sample[j] / negative_sample_rate.
        const size_t num_neg_samples = (epoch - epoch_of_next_negative_sample[j]) * negative_sample_rate / epochs_per_sample[j];

        for (size_t p = 0; p < num_neg_samples; ++p) {
            size_t sampled = aarand::discrete_uniform(rng, num_obs); 
            if (sampled == i) {
                continue;
            }

            Float* right = embedding + sampled * ndim;
            Float dist2 = quick_squared_distance(left, right, ndim);
            const Float grad_coef = 2 * gamma * b / ((0.001 + dist2) * (a * std::pow(dist2, b) + 1.0));
            {
                Float* lcopy = left;
                const Float* rcopy = right;
                for (int d = 0; d < ndim; ++d, ++lcopy, ++rcopy) {
                    Float gradient = alpha * clamp(grad_coef * (*lcopy - *rcopy));
                    if constexpr(!batch) {
                        *lcopy += gradient;
                    } else {
                        buffer[d] += gradient;
                    }
                }
            }
        }

        epoch_of_next_sample[j] += epochs_per_sample[j];

        // The update to epoch_of_next_negative_sample involves adding
        // num_neg_samples * epochs_per_negative_sample, which eventually boils
        // down to setting epoch_of_next_negative_sample to 'n'.
        epoch_of_next_negative_sample[j] = epoch;
    }
}

template<typename Float, class Setup, class Rng>
void optimize_layout(
    int ndim,
    Float* embedding, 
    Setup& setup,
    Float a, 
    Float b, 
    Float gamma,
    Float initial_alpha,
    Rng& rng,
    int epoch_limit
) {
    auto& n = setup.current_epoch;
    auto num_epochs = setup.total_epochs;
    auto limit_epochs = num_epochs;
    if (epoch_limit> 0) {
        limit_epochs = std::min(epoch_limit, num_epochs);
    }

    for (; n < limit_epochs; ++n) {
        const Float epoch = n;
        const Float alpha = initial_alpha * (1.0 - epoch / num_epochs);
        for (size_t i = 0; i < setup.head.size(); ++i) {
            optimize_sample<false>(i, ndim, embedding, static_cast<Float*>(NULL), setup, a, b, gamma, alpha, rng, epoch);
        }
    }

    return;
}

template<typename Float, class Setup, class SeedFunction, class EngineFunction>
inline void optimize_layout_batched(
    int ndim,
    Float* embedding, 
    Setup& setup,
    Float a, 
    Float b, 
    Float gamma,
    Float initial_alpha,
    SeedFunction seeder,
    EngineFunction creator,
    int epoch_limit,
    int nthreads
) {
    auto& n = setup.current_epoch;
    auto num_epochs = setup.total_epochs;
    auto limit_epochs = num_epochs;
    if (epoch_limit > 0) {
        limit_epochs = std::min(epoch_limit, num_epochs);
    }

    const size_t num_obs = setup.head.size(); 
    std::vector<decltype(seeder())> seeds(num_obs);
    std::vector<Float> replace_buffer(num_obs * ndim);
    Float* replacement = replace_buffer.data();
    bool using_replacement = false;

    for (; n < limit_epochs; ++n) {
        const Float epoch = n;
        const Float alpha = initial_alpha * (1.0 - epoch / num_epochs);

        // Fill the seeds.
        for (auto& s : seeds) {
            s = seeder();
        }

        // Input and output alternate between epochs, to avoid the need for a
        // copy operation on the entire embedding at the end of each epoch.
        Float* reference = (using_replacement ? replacement : embedding); 
        Float* output = (using_replacement ? embedding : replacement);
        using_replacement = !using_replacement;

#ifndef UMAPPP_CUSTOM_PARALLEL
        #pragma omp parallel num_threads(nthreads)
        {
            std::vector<Float> buffer(ndim);
            #pragma omp for
            for (size_t i = 0; i < setup.head.size(); ++i) {
#else
        UMAPPP_CUSTOM_PARALLEL(setup.head.size(), [&](size_t first, size_t last) -> void {
            std::vector<Float> buffer(ndim);
            for (size_t i = first; i < last; ++i) {
#endif

                size_t shift = i * ndim;
                std::copy(reference + shift, reference + shift + ndim, buffer.data());
                auto rng = creator(seeds[i]);
                optimize_sample<true>(i, ndim, reference, buffer.data(), setup, a, b, gamma, alpha, rng, epoch);
                std::copy(buffer.begin(), buffer.end(), output + shift);

#ifndef UMAPPP_CUSTOM_PARALLEL
            }
        }
#else
            }
        }, nthreads);
#endif
    }

    if (using_replacement) {
        std::copy(replace_buffer.begin(), replace_buffer.end(), embedding);
    }

    return;
}

}

#endif
