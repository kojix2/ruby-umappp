# frozen_string_literal: true

require "umappp"
require "datasets" # red-datasets https://github.com/red-data-tools/red-datasets
require "numo/gnuplot"
require "etc"

mnist = Datasets::MNIST.new

pixels = []
labels = []
mnist.each_with_index do |r, _i|
  pixels << r.pixels
  labels << r.label
end
l = Numo::Int8.cast(labels)

nproc = Etc.nprocessors
n = nproc > 4 ? nproc - 1 : nproc

puts "start umap #{n} threads"
d = Umappp.run(pixels, num_threads: n, a: 1.8956, b: 0.8006)
puts "end umap"

x = d[true, 0]
y = d[true, 1]

plots = 10.times.map do |i|
  [x[l.eq(i)], y[l.eq(i)], { with: "points", pt: 7, ps: 0.2, t: i.to_s }]
end

Dir.chdir(__dir__) do
  Numo.gnuplot do
    set :term, "pngcairo"
    set :output, "data/mnist_gnuplot.png"
    set :key, "left top"
    plot(*plots)
  end
  puts "Saved to data/mnist_gnuplot.png"
end
