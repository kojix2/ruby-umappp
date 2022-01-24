#include "umap.h"

VALUE rb_mUmap;

void
Init_umap(void)
{
  rb_mUmap = rb_define_module("Umap");
}
