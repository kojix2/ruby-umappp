# frozen_string_literal: true

require "mkmf-rice"
require "numo/narray"

vendor = File.expand_path("../../vendor", __dir__)

apple_clang = RbConfig::CONFIG["CC_VERSION_MESSAGE"] =~ /apple clang/i

# check omp first
if have_library("omp") || have_library("gomp")
  $CXXFLAGS += " -Xclang" if apple_clang
  $CXXFLAGS += " -fopenmp"
end

# numo-narray-alt
numo = File.join(Gem.loaded_specs["numo-narray-alt"].require_path, "numo")
abort "Numo header not found" unless find_header("numo/narray.h", numo)
if Gem.win_platform?
  narray_lib_dirs = [numo, File.join(numo, "narray")]
  abort "Numo library not found" unless find_library("narray", nil, *narray_lib_dirs)
end
find_header("numo.hpp", File.expand_path("../../include", __dir__))

dir_config "umap", vendor, vendor
dir_config "umappp", File.join(vendor, "umappp"), File.join(vendor, "umappp")

create_makefile("umappp/umappp")
