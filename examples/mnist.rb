# frozen_string_literal: true

require "umappp"
require "datasets" # red-datasets https://github.com/red-data-tools/red-datasets
require "gr/plot"  # GR.rb https://github.com/red-data-tools/GR.rb
require "etc"

mnist = Datasets::MNIST.new

pixels = []
labels = []
mnist.each_with_index do |r, _i|
  pixels << r.pixels
  labels << r.label
end

nproc = Etc.nprocessors
n = nproc > 4 ? nproc - 1 : nproc

puts "start umap #{n} threads"
d = Umappp.run(pixels, num_threads: n, a: 1.8956, b: 0.8006)
puts "end umap"

x = d[true, 0]
y = d[true, 1]
s = [500] * x.size

GR.scatter(x, y, s, labels, colormap: 0)
Dir.chdir(__dir__) do
  # Save results
  File.binwrite("data/mnist.dat", Marshal.dump([d, labels]))
  # Save plot
  GR.savefig("data/mnist.png")
end

puts "Press any key to exit"
gets # Wait for key input
