# frozen_string_literal: true

require "numo/narray"
require_relative "umappp/version"
require_relative "umappp/umappp"

# Uniform Manifold Approximation and Projection
module Umappp
  class Error < StandardError; end
  # Your code goes here...

  class << self
    def run(ary, method: :annoy, ndim: 2, tick: 0, **params)
      unless (u = (params.keys - default_parameters.keys)).empty?
        raise ArgumentError, "[umappp.rb] unknown option : #{u.inspect}"
      end

      nnmethod = %i[annoy vptree].index(method.to_sym)
      data     = Numo::SFloat.cast(ary)

      Umappp.umap_run(params, data, ndim, nnmethod, tick)
    end
  end
end
