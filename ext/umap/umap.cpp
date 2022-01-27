#include <rice/rice.hpp>
#include <rice/stl.hpp>
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

Object define_defaults(Object self)
{
  Hash d;
  d[String("local_connectivity")]   = Umap::Defaults::local_connectivity;
  d[String("bandwidth")]            = Umap::Defaults::bandwidth;
  d[String("mix_ratio")]            = Umap::Defaults::mix_ratio;
  d[String("spread")]               = Umap::Defaults::spread;
  d[String("min_dist")]             = Umap::Defaults::min_dist;
  d[String("a")]                    = Umap::Defaults::a;
  d[String("b")]                    = Umap::Defaults::b;
  d[String("repulsion_strength")]   = Umap::Defaults::repulsion_strength;
  d[String("num_epochs")]           = Umap::Defaults::num_epochs;
  d[String("learning_rate")]        = Umap::Defaults::learning_rate;
  d[String("negative_sample_rate")] = Umap::Defaults::negative_sample_rate;
  d[String("num_neighbors")]        = Umap::Defaults::num_neighbors;
  d[String("seed")]                 = Umap::Defaults::seed;
  d[String("batch")]                = Umap::Defaults::batch;

  return d;
}

extern "C"
void Init_umap()
{
  Module rb_mUmap = 
    define_module("Umap")
    .define_singleton_method("run", &umap_run)
    .define_singleton_method("define_defaults", &define_defaults);
}