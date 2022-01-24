#include "umappp.hpp"

VALUE rb_mUmap;

extern "C"
void Init_umap()
{
  Rice::define_module("Umap")
}