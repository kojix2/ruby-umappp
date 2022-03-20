# frozen_string_literal: true

require "numo/narray"
require_relative "umap/version"
require_relative "umap/umap"

# Uniform Manifold Approximation and Projection
module Umap
  class Error < StandardError; end
  # Your code goes here...

  class << self
    def run(ary, method: :annoy, ndim: 2, nthreads: 1, tick: 0, **params)
      unless (u = (params.keys - default_parameters.keys)).empty?
        raise ArgumentError, "[umappp.rb] unknown option : #{u.inspect}"
      end

      nnmethod = %i[annoy vptree].index(method.to_sym)
      data     = Numo::SFloat.cast(ary)

      Umap.umap_run(params, data, ndim, nnmethod, nthreads, tick)
    end
  end
end
