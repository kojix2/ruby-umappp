# frozen_string_literal: true

require "mkmf-rice"

vendor = File.expand_path("../../vendor", __dir__)

dir_config "umap", vendor, vendor

# find_header("umappp/umappp.h", vendor)
dir_config "umappp", File.join(vendor, "umappp"), File.join(vendor, "umappp")

create_makefile("umap/umap")
