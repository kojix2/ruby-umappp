# frozen_string_literal: true
require 'numo/narray'
require_relative "umap/version"
require_relative "umap/umap"

module Umap
  class Error < StandardError; end
  # Your code goes here...

  class << self
    def umap(y, method: :annoy, ndim: 2, nthreads: 1, tick: 0, **opts)
      opts.transform_keys!(&:to_sym)
      params   = define_defaults.merge(opts)
      nnmethod = %i[annoy vptree].index(method.to_sym)
      data     = Numo::SFloat.cast(y).transpose

      x = Umap.run(params, data, ndim, nnmethod, nthreads) if tick.zero?
    end
  end
end
