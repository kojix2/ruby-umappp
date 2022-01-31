# frozen_string_literal: true

require "umap"
require "datasets-numo-narray"
require "gr/plot"

iris = Datasets::LIBSVM.new("iris").to_narray
x = iris[true, 1..-1]
y = iris[true, 0]

r = Umap.run(x)
GR.scatter(
  r[true, 0], r[true, 1], [200] * y.size, y,
  title: "iris",
  colormap: 16,
  colorbar: true,
)
gets
