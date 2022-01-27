# frozen_string_literal: true

require_relative "umap/version"
require_relative "umap/umap"

module Umap
  class Error < StandardError; end
  # Your code goes here...

  class << self
    def umap(_y, method: "Annoy", ndim: 2, nthreads: 1, tick: 0, **options)
      options.transform_keys!(&:to_sym)
      args = define_defaults
    end
  end
end
