require "datasets"
require "umap"
require "gr/plot"

mnist = Datasets::MNIST.new

pixels = []
labels = []
mnist.each_with_index do |r, _i|
  pixels << r.pixels
  labels << r.label
end

puts "start umap"
d = Umappp.run(pixels, nthreads: 7)
puts "end umap"

x = d[true, 0]
y = d[true, 1]

GR.scatter(x, y, [100] * x.size, labels, colormap: 0)

gets
