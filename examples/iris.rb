# frozen_string_literal: true

require "umap"

# red-datasets-numo-narray https://github.com/red-data-tools/red-datasets-numo-narray
require "datasets-numo-narray"

# GR.rb https://github.com/red-data-tools/GR.rb
require "gr/plot"

iris = Datasets::LIBSVM.new("iris").to_narrayGR.rb # GR.rb https://github.com/red-data-tools/GR.rb
x = iris[true, 1..-1]
y = iris[true, 0]

r = Umap.run(x)

GR.scatter(
  r[true, 0], r[true, 1], [200] * y.size, y,
  title: "iris",
  colormap: 16,
  colorbar: true
)
gets
