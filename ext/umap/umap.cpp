#include <rice/rice.hpp>
#include <rice/stl.hpp>

#include "Umap.hpp"

using namespace Rice;

extern "C"
void Init_umap()
{
  Module rb_mUmap = Rice::define_module("Umap");
}