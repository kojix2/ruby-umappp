#include <rice/rice.hpp>
#include <rice/stl.hpp>

#include "umappp.hpp"

extern "C"
void Init_umap()
{
  Rice::define_module("Umap");
}