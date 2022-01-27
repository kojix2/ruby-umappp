require "datasets"
require "umap"
require "gr/plot"

mnist = Datasets::MNIST.new

n = 24_000
idx = Array.new(60_000) { |i| i }.sample(n)

pixels = []
labels = []
mnist.each_with_index do |r, i|
  next unless idx.include?(i)

  pixels << r.pixels
  labels << r.label
end

puts "start umap"
d = Umap.umap(pixels, nthreads: 7)
puts "end umap"

x = d[true, 0]
y = d[true, 1]

GR.scatter(x, y, [100] * n, labels, colormap: 0)

gets
