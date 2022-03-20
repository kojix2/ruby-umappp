# frozen_string_literal: true

require "umap"
require "datasets" # red-datasets https://github.com/red-data-tools/red-datasets
require "gr/plot"  # GR.rb https://github.com/red-data-tools/GR.rb

mnist = Datasets::MNIST.new

pixels = []
labels = []
mnist.each_with_index do |r, _i|
  pixels << r.pixels
  labels << r.label
end

puts "start umap"
d = Umap.run(pixels, nthreads: 8, a: 1.8956, b: 0.8006)
puts "end umap"

x = d[true, 0]
y = d[true, 1]

GR.scatter(x, y, [100] * x.size, labels, colormap: 0)

gets
