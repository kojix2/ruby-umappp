/*
 * Uniform Manifold Approximation and Projection for Ruby
 * https://github.com/kojix2/ruby-umappp
 */

#include <rice/rice.hpp>
#include <rice/stl.hpp>
#include <ruby/thread.h>
#include <exception>
#include <string>
#include "numo.hpp"
#include "Umap.hpp"

typedef float Float;
typedef typename umappp::Umap<Float> Umap;

using namespace Rice;

// Data structure for running UMAP calculation without GVL.
struct UmapRunData
{
  Umap *umap_ptr;
  knncolle::Base<int, Float> *knncolle_ptr;
  int ndim;
  std::vector<Float> *embedding;
  std::string exception_message;
  bool exception_thrown = false;
};

// Callback for UMAP calculation (executed without GVL).
static void *umap_run_without_gvl(void *data)
{
  UmapRunData *run_data = static_cast<UmapRunData *>(data);
  try
  {
    auto status = run_data->umap_ptr->initialize(
        run_data->knncolle_ptr,
        run_data->ndim,
        run_data->embedding->data());
    int epoch_limit = 0;
    status.run(epoch_limit);
  }
  catch (const std::exception &e)
  {
    run_data->exception_message = e.what();
    run_data->exception_thrown = true;
  }
  catch (...)
  {
    run_data->exception_message = "Unknown exception occurred in UMAP calculation.";
    run_data->exception_thrown = true;
  }
  return nullptr;
}

// Returns default parameters from the Umappp C++ library.
Hash umappp_default_parameters(Object self)
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
  d[Symbol("initialize")] = Umap::Defaults::initialize;
  d[Symbol("num_epochs")] = Umap::Defaults::num_epochs;
  d[Symbol("learning_rate")] = Umap::Defaults::learning_rate;
  d[Symbol("negative_sample_rate")] = Umap::Defaults::negative_sample_rate;
  d[Symbol("num_neighbors")] = Umap::Defaults::num_neighbors;
  d[Symbol("seed")] = Umap::Defaults::seed;
  d[Symbol("num_threads")] = Umap::Defaults::num_threads;
  d[Symbol("parallel_optimization")] = Umap::Defaults::parallel_optimization;

  return d;
}

// Main function to perform UMAP.
Object umappp_run(
    Object self,
    Hash params,
    numo::SFloat data,
    int ndim,
    int nn_method)
{
  if (ndim < 1)
  {
    throw std::runtime_error("ndim is less than 1");
  }

  std::unique_ptr<Umap> umap_ptr(new Umap);

  double local_connectivity = Umap::Defaults::local_connectivity;
  if (RTEST(params.call("has_key?", Symbol("local_connectivity"))))
  {
    local_connectivity = params.get<double>(Symbol("local_connectivity"));
    umap_ptr->set_local_connectivity(local_connectivity);
  }

  double bandwidth = Umap::Defaults::bandwidth;
  if (RTEST(params.call("has_key?", Symbol("bandwidth"))))
  {
    bandwidth = params.get<double>(Symbol("bandwidth"));
    umap_ptr->set_bandwidth(bandwidth);
  }

  double mix_ratio = Umap::Defaults::mix_ratio;
  if (RTEST(params.call("has_key?", Symbol("mix_ratio"))))
  {
    mix_ratio = params.get<double>(Symbol("mix_ratio"));
    umap_ptr->set_mix_ratio(mix_ratio);
  }

  double spread = Umap::Defaults::spread;
  if (RTEST(params.call("has_key?", Symbol("spread"))))
  {
    spread = params.get<double>(Symbol("spread"));
    umap_ptr->set_spread(spread);
  }

  double min_dist = Umap::Defaults::min_dist;
  if (RTEST(params.call("has_key?", Symbol("min_dist"))))
  {
    min_dist = params.get<double>(Symbol("min_dist"));
    umap_ptr->set_min_dist(min_dist);
  }

  double a = Umap::Defaults::a;
  if (RTEST(params.call("has_key?", Symbol("a"))))
  {
    a = params.get<double>(Symbol("a"));
    umap_ptr->set_a(a);
  }

  double b = Umap::Defaults::b;
  if (RTEST(params.call("has_key?", Symbol("b"))))
  {
    b = params.get<double>(Symbol("b"));
    umap_ptr->set_b(b);
  }

  double repulsion_strength = Umap::Defaults::repulsion_strength;
  if (RTEST(params.call("has_key?", Symbol("repulsion_strength"))))
  {
    repulsion_strength = params.get<double>(Symbol("repulsion_strength"));
    umap_ptr->set_repulsion_strength(repulsion_strength);
  }

  umappp::InitMethod initialize = Umap::Defaults::initialize;
  if (RTEST(params.call("has_key?", Symbol("initialize"))))
  {
    initialize = params.get<umappp::InitMethod>(Symbol("initialize"));
    umap_ptr->set_initialize(initialize);
  }

  int num_epochs = Umap::Defaults::num_epochs;
  if (RTEST(params.call("has_key?", Symbol("num_epochs"))))
  {
    num_epochs = params.get<int>(Symbol("num_epochs"));
    umap_ptr->set_num_epochs(num_epochs);
  }

  double learning_rate = Umap::Defaults::learning_rate;
  if (RTEST(params.call("has_key?", Symbol("learning_rate"))))
  {
    learning_rate = params.get<double>(Symbol("learning_rate"));
    umap_ptr->set_learning_rate(learning_rate);
  }

  double negative_sample_rate = Umap::Defaults::negative_sample_rate;
  if (RTEST(params.call("has_key?", Symbol("negative_sample_rate"))))
  {
    negative_sample_rate = params.get<double>(Symbol("negative_sample_rate"));
    umap_ptr->set_negative_sample_rate(negative_sample_rate);
  }

  int num_neighbors = Umap::Defaults::num_neighbors;
  if (RTEST(params.call("has_key?", Symbol("num_neighbors"))))
  {
    num_neighbors = params.get<int>(Symbol("num_neighbors"));
    umap_ptr->set_num_neighbors(num_neighbors);
  }

  int seed = Umap::Defaults::seed;
  if (RTEST(params.call("has_key?", Symbol("seed"))))
  {
    seed = params.get<int>(Symbol("seed"));
    umap_ptr->set_seed(seed);
  }

  int num_threads = Umap::Defaults::num_threads;
  if (RTEST(params.call("has_key?", Symbol("num_threads"))))
  {
    num_threads = params.get<int>(Symbol("num_threads"));
    umap_ptr->set_num_threads(num_threads);
  }

  bool parallel_optimization = Umap::Defaults::parallel_optimization;
  if (RTEST(params.call("has_key?", Symbol("parallel_optimization"))))
  {
    parallel_optimization = params.get<bool>(Symbol("parallel_optimization"));
    umap_ptr->set_parallel_optimization(parallel_optimization);
  }

  const float *y = data.read_ptr();
  size_t *shape = data.shape();

  int nd = shape[1];
  int nobs = shape[0];
  if (nobs <= 0)
  {
    throw std::runtime_error("number of observations must be positive");
  }
  if (nd <= 0)
  {
    throw std::runtime_error("number of dimensions must be positive");
  }

  std::unique_ptr<knncolle::Base<int, Float>> knncolle_ptr;
  if (nn_method == 0)
  {
    knncolle_ptr.reset(new knncolle::AnnoyEuclidean<int, Float>(nd, nobs, y));
  }
  else if (nn_method == 1)
  {
    knncolle_ptr.reset(new knncolle::KmknnEuclidean<int, Float>(nd, nobs, y));
  }
  else
  {
    throw std::runtime_error("unknown nearest neighbor method");
  }

  std::vector<Float> embedding(ndim * nobs);

  // Run UMAP calculation without GVL.
  UmapRunData run_data = {
      umap_ptr.get(),
      knncolle_ptr.get(),
      ndim,
      &embedding};

  rb_thread_call_without_gvl(
      umap_run_without_gvl,
      &run_data,
      NULL,
      NULL);

  if (run_data.exception_thrown)
  {
    throw std::runtime_error(run_data.exception_message);
  }

  auto na = numo::SFloat({(unsigned int)nobs, (unsigned int)ndim});
  std::copy(embedding.begin(), embedding.end(), na.write_ptr());

  return na;
}

extern "C" void Init_umappp()
{
  Module rb_mUmappp =
      define_module("Umappp")
          .define_singleton_method("umappp_run", &umappp_run)
          .define_singleton_method("umappp_default_parameters", &umappp_default_parameters);
  Enum<umappp::InitMethod> init_method =
      define_enum<umappp::InitMethod>("InitMethod")
          .define_value("SPECTRAL", umappp::InitMethod::SPECTRAL)
          .define_value("SPECTRAL_ONLY", umappp::InitMethod::SPECTRAL_ONLY)
          .define_value("RANDOM", umappp::InitMethod::RANDOM)
          .define_value("NONE", umappp::InitMethod::NONE);
}
