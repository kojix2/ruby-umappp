# frozen_string_literal: true
require 'numo/narray'
require_relative "umap/version"
require_relative "umap/umap"

module Umap
  class Error < StandardError; end
  # Your code goes here...

  class << self
    def umap(y, method: :Annoy, ndim: 2, nthreads: 1, tick: 0, **opts)
      opts.transform_keys!(&:to_sym)
      params   = define_defaults.merge(opts)
      m        = %i[annoy vptree].index(method.to_sym)
      data     = Numo::SFloat.cast(y).transpose
      nd, nobs = data.shape # really?

      x = Umap.run(params, data, nd, nobs, m, ndim, nthreads) if tick.zero?
    end
  end
end
