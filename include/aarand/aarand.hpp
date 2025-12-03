#ifndef AARAND_AARAND_HPP
#define AARAND_AARAND_HPP

#include <cmath>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <type_traits>

/**
 * @file aarand.hpp
 * @brief Aaron's random distribution functions.
 */

/**
 * @namespace aarand
 * @brief Aaron's random distribution functions.
 */
namespace aarand {

/**
 * @tparam Output_ Floating point type of the output.
 * This is also used for intermediate calculations, so it is usually safest to provide a type that is at least as precise as a `double`. 
 * @tparam Engine_ A random number generator class with `operator()`, `min()` (static) and `max()` (static) methods,
 * where the `result_type` is an unsigned integer value.
 *
 * @param eng Instance of an RNG class like `std::mt19937_64`.
 *
 * @return Draw from a standard uniform distribution.
 * All values are guaranteed to lie in `[0, 1)`.
 */
template<typename Output_ = double, class Engine_>
Output_ standard_uniform(Engine_& eng) {
    typedef typename Engine_::result_type R;
    static_assert(std::numeric_limits<R>::is_integer, "RNG engine must yield integer results");

    // Can't be bothered to figure out whether the range fits into 'R' for signed values.
    // So instead, we just require unsigned integers, where the range will always fit.
    static_assert(!std::numeric_limits<R>::is_signed, "RNG engine must yield unsigned integers"); 

    // Stolen from Boost, see https://www.boost.org/doc/libs/1_67_0/boost/random/uniform_01.hpp
    // The +1 probably doesn't matter for 64-bit generators, but is helpful for engines with 
    // fewer output bits, to reduce the (small) probability of sampling 1's.
    constexpr Output_ ONE = 1;
    constexpr Output_ factor = ONE / (static_cast<Output_>(Engine_::max() - Engine_::min()) + ONE);

    // Note that it still might be possible to get a result = 1, depending on
    // the numerical precision used to compute the product; hence the loop.
    Output_ result;
    do {
        result = static_cast<Output_>(eng() - Engine_::min()) * factor;
    } while (result == ONE);

    return result;
}

/**
 * @cond
 */
// Some of the functions below log-transform a uniform random variable.
// However, standard_uniform() has a small chance of returning zero, resulting in an undesirable -Inf after log.
// To avoid this, any time we sample zero, we roll again.
// Note to self: don't try to do something like replacing zeros with 1 to get the log-transform to work,
// as this introduces inflated occurrences of log(1); better to just reroll to get a true sampling from (0, 1).
template<typename Output_, class Engine_>
Output_ non_zero_uniform(Engine_& eng) {
    Output_ val;
    do {
        val = standard_uniform<Output_>(eng);
    } while (val == 0);
    return val;
}
/**
 * @endcond
 */

/**
 * @tparam Output_ Floating point type of the output.
 * This is also used for intermediate calculations, so it is usually safest to provide a type that is at least as precise as a `double`. 
 * @tparam Engine_ A random number generator class with `operator()`, `min()` (static) and `max()` (static) methods,
 * where the `result_type` is an unsigned integer value.
 *
 * @param eng Instance of an RNG class like `std::mt19937_64`.
 *
 * @return A pair of independent draws from a standard normal distribution with mean 0 and variance 1.
 */
template<typename Output_  = double, class Engine_>
std::pair<Output_, Output_> standard_normal(Engine_& eng) {
    constexpr Output_ PI = 3.14159265358979323846;
    constexpr Output_ TWO = 2;

    // Box-Muller gives us two random values at a time.
    const Output_ constant = std::sqrt(-TWO * std::log(non_zero_uniform<Output_>(eng)));
    const Output_ angle = TWO * PI * standard_uniform<Output_>(eng);
    return std::make_pair(constant * std::sin(angle), constant * std::cos(angle));
}

/**
 * @tparam Output_ Floating point type to return.
 * This is also used for intermediate calculations, so it is usually safest to provide a type that is at least as precise as a `double`. 
 * @tparam Engine_ A random number generator class with `operator()`, `min()` (static) and `max()` (static) methods,
 * where the `result_type` is an unsigned integer value.
 *
 * @param eng Instance of an RNG class like `std::mt19937_64`.
 *
 * @return Draw from a standard exponential distribution.
 * Values are guaranteed to be non-negative.
 */
template<typename Output_ = double, class Engine_>
Output_ standard_exponential(Engine_& eng) {
    return -std::log(non_zero_uniform<Output_>(eng));
}

/**
 * @tparam Output_ Integer type of the output.
 * @tparam Engine_ A random number generator class with `operator()`, `min()` (static) and `max()` (static) methods,
 * where the `result_type` is an unsigned integer value.
 *
 * @param eng Instance of an RNG class like `std::mt19937_64`.
 * @param bound Positive integer specifying the upper bound of the discrete distribution.
 *
 * @return Draw from a discrete uniform distribution in `[0, bound)`.
 */
template<typename Output_ = int, class Engine_>
Output_ discrete_uniform(Engine_& eng, const Output_ bound) {
    static_assert(std::numeric_limits<Output_>::is_integer);
    if (bound <= 0) {
        throw std::runtime_error("'bound' should be a positive integer");
    }

    typedef typename Engine_::result_type R;
    static_assert(std::numeric_limits<R>::is_integer);
    static_assert(!std::numeric_limits<R>::is_signed); // don't want to figure out how to store the range if it might not fit into R.

    constexpr R range = Engine_::max() - Engine_::min();
    if (static_cast<typename std::make_unsigned<Output_>::type>(bound) > range) { // force an unsigned comparison.
        throw std::runtime_error("'bound' should be less than the RNG range");
    }

    R draw = eng() - Engine_::min();

    // Conservative shortcut to avoid an extra modulo operation in computing
    // 'limit' if 'draw' is below 'limit'. This is based on the observation
    // that 'range - bound <= limit', so any condition that triggers the loop
    // will also pass this check. Allows early return when 'range >> bound'.
    if (draw > range - bound) {

        // The limit is necessary to provide uniformity in the presence of the
        // modulus. The idea is to re-sample if we get a draw above the limit.
        // Technically this can have problems as bound approaches range, in which
        // case we might end up discarding a lot of the sample space... but this
        // is unlikely to happen in practice, and even if it does, it's a rejection
        // rate that's guaranteed to be less than 50%, so whatever.
        //
        // Note that the +1 is necessary because range is inclusive but bound is not.
        const R limit = range - ((range % bound) + 1);

        // In addition, we don't have to deal with the crap about combining draws
        // to get enough entropy, which is 90% of the Boost implementation.
        while (draw > limit) {
            draw = eng() - Engine_::min();
        }
    }

    return draw % bound;
}

/**
 * @tparam InputIterator_ Random-access iterator or pointer.
 * @tparam Length_ Integer type of the number of elements.
 * @tparam Engine_ A random number generator class with `operator()`, `min()` (static) and `max()` (static) methods,
 * where the `result_type` is an unsigned integer value.
 *
 * @param[in, out] values Iterator or pointer to an array of values to shuffle.
 * On return, contents of `values` are randomly permuted in place using the Fisher-Yates algorithm.
 * @param n Number of values in the array pointed to by `values`.
 * @param eng Instance of an RNG class like `std::mt19937_64`.
 */
template<class InputIterator_, typename Length_, class Engine_>
void shuffle(const InputIterator_ values, const Length_ n, Engine_& eng) {
    if (n <= 1) {
        return;
    }

    const Length_ last = n - 1;
    for (Length_ i = 0; i < last; ++i) {
        const auto chosen = discrete_uniform(eng, n - i);
        if (chosen) {
            const auto current = values + i;
            using std::swap;
            swap(*current, *(current + chosen));
        }
    }
}

/**
 * @tparam InputIterator_ Forward iterator or pointer for the inputs.
 * @tparam Length_ Integer type of the number of elements.
 * @tparam OutputIterator_ Forward iterator or pointer for the outputs.
 * @tparam Engine_ A random number generator class with `operator()`, `min()` (static) and `max()` (static) methods,
 * where the `result_type` is an unsigned integer value.
 *
 * @param[in] values Iterator or pointer to an array of values to sample from.
 * @param n Number of values in the array pointed to by `values`.
 * @param s Number of values to sample.
 * @param[out] output Iterator or pointer to an array of length `s`, to store the sampled values. 
 * On return, `output` is filled with `s` sampled values from `values`.
 * If `s > n`, `values` is copied into the first `n` elements of `output` and the remaining values of `output` are undefined.
 * @param eng Instance of an RNG class like `std::mt19937_64`.
 */
template<class InputIterator_, typename Length_, class OutputIterator_, class Engine_>
void sample(InputIterator_ values, const Length_ n, const Length_ s, OutputIterator_ output, Engine_& eng) {
    if (!s) {
        return;
    }

    auto remaining = s;
    for (Length_ i = 0; i < n; ++i, ++values) {
        const Length_ denom = n - i;
        const double threshold = static_cast<double>(remaining) / denom;
        if (threshold >= 1) {
            // Once remaining >= denom, all remaining values must be selected.
            // Both values will drop at the same rate so threshold will always be >= 1 in subsequent loops.
            std::copy_n(values, denom, output);
            return;
        }

        if (standard_uniform(eng) <= threshold) {
            *output = *values;
            ++output;
            --remaining;
            if (!remaining) {
                return;
            }
        }
    }
}

/**
 * @tparam Length_ Integer type of the number of elements.
 * @tparam OutputIterator_ Forward iterator or pointer for the outputs.
 * @tparam Engine_ A random number generator class with `operator()`, `min()` (static) and `max()` (static) methods,
 * where the `result_type` is an unsigned integer value.
 *
 * @param bound Upper bound of the indices to sample from.
 * @param s Number of values to sample.
 * @param[out] output Iterator or pointer to an array of length `s`, to store the sampled values. 
 * `output` is filled with `s` sampled values from the sequence of integers in `{0, 1, ..., bound - 1}`.
 * If `s > bound`, the first `bound` elements of `output` will contain the sequence of integers from `0` to `bound - 1`.
 * The remaining values of `output` are undefined.
 * @param eng Instance of an RNG class like `std::mt19937_64`.
 */
template<typename Length_, class OutputIterator_, class Engine_>
void sample(const Length_ bound, const Length_ s, OutputIterator_ output, Engine_& eng) {
    if (!s) {
        return;
    }

    auto remaining = s;
    for (Length_ i = 0; i < bound; ++i) {
        const Length_ denom = bound - i;
        const double threshold = static_cast<double>(remaining) / denom;
        if (threshold >= 1) {
            // Once remaining >= denom, all remaining indices must be selected.
            // Both values will drop at the same rate so threshold will always be >= 1 in subsequent loops.
            std::iota(output, output + denom, i);
            return;
        }

        if (standard_uniform(eng) <= threshold) {
            *output = i;
            ++output;
            --remaining;
            if (!remaining) {
                return;
            }
        }
    }
}

}

#endif
