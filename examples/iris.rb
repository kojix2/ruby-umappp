# frozen_string_literal: true

require "umappp"

# red-datasets-numo-narray https://github.com/red-data-tools/red-datasets-numo-narray
require "datasets-numo-narray"

# GR.rb https://github.com/red-data-tools/GR.rb
require "gr/plot"

iris = Datasets::LIBSVM.new("iris").to_narray
x = iris[true, 1..-1]
y = iris[true, 0]

r = Umappp.run(x)

GR.scatter(
  r[true, 0], r[true, 1], [2000] * y.size, y,
  title: "iris",
  colormap: 16,
  colorbar: true
)
gets
