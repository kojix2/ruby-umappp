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

Object umap_run(Object self, int ndim, int nthreads, int tick = 0)
{
  int nd = 3;
  int nobs = 3;

#ifdef _OPENMP
  omp_set_num_threads(nthreads);
#endif

  return self;
}

Hash define_defaults(Object self)
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

extern "C" void Init_umap()
{
  Module rb_mUmap =
      define_module("Umap")
          .define_singleton_method("run", &umap_run)
          .define_singleton_method("define_defaults", &define_defaults);
}