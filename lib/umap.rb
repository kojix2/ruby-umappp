# frozen_string_literal: true

require "numo/narray"
require_relative "umap/version"
require_relative "umap/umap"

# Uniform Manifold Approximation and Projection
module Umap
  class Error < StandardError; end
  # Your code goes here...

  class << self
    def umap(ary, method: :annoy, ndim: 2, nthreads: 1, tick: 0, **opts)
      opts.transform_keys!(&:to_sym)
      params = default_parameters
      unless (u = (opts.keys - params.keys)).empty?
        raise ArgumentError, "[umappp.rb] unknown option : #{u.inspect}"
      end

      params.merge!(opts)
      nnmethod = %i[annoy vptree].index(method.to_sym)
      data     = Numo::SFloat.cast(ary).transpose

      Umap.run(params, data, ndim, nnmethod, nthreads) if tick.zero?
    end
  end
end
