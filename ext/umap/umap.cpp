// Uniform Manifold Approximation and Projection for Ruby
// https://github.com/kojix2/umap


#include <rice/rice.hpp>
#include <rice/stl.hpp>
#include "numo.hpp"
#include "Umap.hpp"

typedef float Float;
typedef typename umappp::Umap<Float> Umap;

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace Rice;

// This function is used to view default parameters from Ruby.

Hash umap_default_parameters(Object self)
{
  Hash d;
  d[Symbol("local_connectivity")] = Umap::Defaults::local_connectivity;
  d[Symbol("bandwidth")] = Umap::Defaults::bandwidth;
  d[Symbol("mix_ratio")] = Umap::Defaults::mix_ratio;
  d[Symbol("spread")] = Umap::Defaults::spread;
  d[Symbol("min_dist")] = Umap::Defaults::min_dist;
  d[Symbol("a")] = Umap::Defaults::a;
  d[Symbol("b")] = Umap::Defaults::b;
  d[Symbol("repulsion_strength")] = Umap::Defaults::repulsion_strength;
  d[Symbol("num_epochs")] = Umap::Defaults::num_epochs;
  d[Symbol("learning_rate")] = Umap::Defaults::learning_rate;
  d[Symbol("negative_sample_rate")] = Umap::Defaults::negative_sample_rate;
  d[Symbol("num_neighbors")] = Umap::Defaults::num_neighbors;
  d[Symbol("seed")] = Umap::Defaults::seed;
  d[Symbol("batch")] = Umap::Defaults::batch;

  return d;
}

// Function to perform umap.

Object umap_run(
    Object self,
    Hash params,
    numo::SFloat data,
    int ndim,
    int nn_method,
    int nthreads,
    int tick = 0)
{
  // Parameters are taken from a Ruby Hash object.
  // If there is no key, set the default value.
  
  double local_connectivity = Umap::Defaults::local_connectivity;
  if (RTEST(params.call("has_key?", Symbol("local_connectivity"))))
  {
    local_connectivity = params.get<double>(Symbol("local_connectivity"));
  }

  double bandwidth = Umap::Defaults::bandwidth;
  if (RTEST(params.call("has_key?", Symbol("bandwidth"))))
  {
    bandwidth = params.get<double>(Symbol("bandwidth"));
  }

  double mix_ratio = Umap::Defaults::mix_ratio;
  if (RTEST(params.call("has_key?", Symbol("mix_ratio"))))
  {
    mix_ratio = params.get<double>(Symbol("mix_ratio"));
  }

  double spread = Umap::Defaults::spread;
  if (RTEST(params.call("has_key?", Symbol("spread"))))
  {
    spread = params.get<double>(Symbol("spread"));
  }

  double min_dist = Umap::Defaults::min_dist;
  if (RTEST(params.call("has_key?", Symbol("min_dist"))))
  {
    min_dist = params.get<double>(Symbol("min_dist"));
  }

  double a = Umap::Defaults::a;
  if (RTEST(params.call("has_key?", Symbol("a"))))
  {
    a = params.get<double>(Symbol("a"));
  }

  double b = Umap::Defaults::b;
  if (RTEST(params.call("has_key?", Symbol("b"))))
  {
    b = params.get<double>(Symbol("b"));
  }

  double repulsion_strength = Umap::Defaults::repulsion_strength;
  if (RTEST(params.call("has_key?", Symbol("repulsion_strength"))))
  {
    repulsion_strength = params.get<double>(Symbol("repulsion_strength"));
  }

  int num_epochs = Umap::Defaults::num_epochs;
  if (RTEST(params.call("has_key?", Symbol("num_epochs"))))
  {
    num_epochs = params.get<int>(Symbol("num_epochs"));
  }

  double learning_rate = Umap::Defaults::learning_rate;
  if (RTEST(params.call("has_key?", Symbol("learning_rate"))))
  {
    learning_rate = params.get<double>(Symbol("learning_rate"));
  }

  double negative_sample_rate = Umap::Defaults::negative_sample_rate;
  if (RTEST(params.call("has_key?", Symbol("negative_sample_rate"))))
  {
    negative_sample_rate = params.get<double>(Symbol("negative_sample_rate"));
  }

  int num_neighbors = Umap::Defaults::num_neighbors;
  if (RTEST(params.call("has_key?", Symbol("num_neighbors"))))
  {
    num_neighbors = params.get<int>(Symbol("num_neighbors"));
  }

  int seed = Umap::Defaults::seed;
  if (RTEST(params.call("has_key?", Symbol("seed"))))
  {
    seed = params.get<int>(Symbol("seed"));
  }

  bool batch = Umap::Defaults::batch;
  if (RTEST(params.call("has_key?", Symbol("batch"))))
  {
    batch = params.get<bool>(Symbol("batch"));
  }

  // setup_parameters

  auto umap_ptr = new Umap;
  umap_ptr->set_local_connectivity(local_connectivity);
  umap_ptr->set_bandwidth(bandwidth);
  umap_ptr->set_mix_ratio(mix_ratio);
  umap_ptr->set_spread(spread);
  umap_ptr->set_min_dist(min_dist);
  umap_ptr->set_a(a);
  umap_ptr->set_b(b);

  umap_ptr->set_repulsion_strength(repulsion_strength);
  umap_ptr->set_num_epochs(num_epochs);
  umap_ptr->set_learning_rate(learning_rate);
  umap_ptr->set_negative_sample_rate(negative_sample_rate);

  umap_ptr->set_num_neighbors(num_neighbors);
  umap_ptr->set_seed(seed);
  umap_ptr->set_batch(batch);

  // initialize_from_matrix

  const float *y = data.read_ptr();
  size_t *shape = data.shape();

  int nd = shape[1];
  int nobs = shape[0];

#ifdef _OPENMP
  omp_set_num_threads(nthreads);
#else
  if (nthreads > 1)
  {
    fprintf(stderr, "[umappp] Compiled without OpenMP. Multi-threading is not available.\n");
  }
#endif

  std::unique_ptr<knncolle::Base<int, Float>> knncolle_ptr;
  if (nn_method == 0)
  {
    knncolle_ptr.reset(new knncolle::AnnoyEuclidean<int, Float>(nd, nobs, y));
  }
  else if (nn_method == 1)
  {
    knncolle_ptr.reset(new knncolle::KmknnEuclidean<int, Float>(nd, nobs, y));
  }

  std::vector<Float> embedding(ndim * nobs);

  auto status = umap_ptr->initialize(knncolle_ptr.get(), ndim, embedding.data());

  if (tick == 0)
  {
#ifdef _OPENMP
    omp_set_num_threads(nthreads);
#endif
    umap_ptr->run(status, ndim, embedding.data(), 0);

    auto na = numo::SFloat({(uint)nobs, (uint)ndim});
    std::copy(embedding.begin(), embedding.end(), na.write_ptr());

    return na;
  }
  else
  {
    VALUE ret = rb_ary_new();

    while (status.epoch() < status.num_epochs())
    {
      int epoch_limit = status.epoch() + tick;

#ifdef _OPENMP
      omp_set_num_threads(nthreads);
#endif

      umap_ptr->run(status, ndim, embedding.data(), epoch_limit);

      auto na = numo::SFloat({(uint)nobs, (uint)ndim});
      std::copy(embedding.begin(), embedding.end(), na.write_ptr());

      rb_ary_push(ret, na.value());
    }

    return ret;
  }
}

extern "C" void Init_umap()
{
  Module rb_mUmap =
      define_module("Umap")
          .define_singleton_method("umap_run", &umap_run)
          .define_singleton_method("default_parameters", &umap_default_parameters);
}
