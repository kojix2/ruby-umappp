# frozen_string_literal: true

require "mkmf-rice"

vendor = File.expand_path("../../vendor", __dir__)
find_header("umappp/umappp.hpp", vendor)

create_makefile("umap/umap")
