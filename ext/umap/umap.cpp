#include <rice/rice.hpp>
#include <rice/stl.hpp>

#include "Umap.hpp"

extern "C"
void Init_umap()
{
  auto m = Rice::define_module("Umap");
}