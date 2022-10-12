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
d = Umappp.run(pixels, ndim: 3, num_threads: n, a: 1.8956, b: 0.8006)
puts "end umap"

x = d[true, 0]
y = d[true, 1]
z = d[true, 2]

Dir.chdir(__dir__) do
  # Save results
  File.binwrite("data/mnist2.dat", Marshal.dump([d, labels]))
  puts "Saved to data/mnist2.dat"
  # Save gif animation
  GR.beginprint("data/mnist2.gif") do
    30.times do |i|
      puts "frame #{i + 1}"
      GR.scatter3(x, y, z, labels, colormap: 0, backgroundcolor: 1, rotation: i * 3)
    end
  end
  puts "Saved to data/mnist2.gif"
end
