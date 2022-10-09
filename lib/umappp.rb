# frozen_string_literal: true

require "numo/narray"
require_relative "umappp/version"
require_relative "umappp/umappp"

# Uniform Manifold Approximation and Projection
module Umappp

  # Run UMAP
  # @param embedding [Array, Numo::SFloat] 
  # @param method [Symbol]
  # @param ndim [Integer]
  # @param tick [Integer]
  # @param local_connectivity [Numeric]
  # @param bandwidth [Numeric]
  # @param mix_ratio [Numeric]
  # @param spread [Numeric]
  # @param min_dist [Numeric]
  # @param a [Numeric]
  # @param b [Numeric]
  # @param repulsion_strength [Numeric]
  # @param num_epochs [Integer]
  # @param learning_rate [Numeric]
  # @param negative_sample_rate [Numeric]
  # @param num_neighbors [Integer]
  # @param seed [Integer]
  # @param batch [Boolean]
  # @param num_threads [Integer]

  def self.run(ary, method: :annoy, ndim: 2, tick: 0, **params)
    unless (u = (params.keys - default_parameters.keys)).empty?
      raise ArgumentError, "[umappp.rb] unknown option : #{u.inspect}"
    end

    nnmethod = %i[annoy vptree].index(method.to_sym)
    data     = Numo::SFloat.cast(ary)

    Umappp.umap_run(params, data, ndim, nnmethod, tick)
  end
end
