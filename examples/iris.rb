# frozen_string_literal: true

require "umappp"
require "datasets-numo-narray" # https://github.com/red-data-tools/red-datasets-numo-narray
require "gr/plot" # https://github.com/red-data-tools/GR.rb

iris = Datasets::LIBSVM.new("iris").to_narray
d = iris[true, 1..-1]
l = iris[true, 0]

r = Umappp.run(d)
x = r[true, 0]
y = r[true, 1]
s = [1500] * l.size

GR.scatter(
  x, y, s, l,
  title: "iris",
  colormap: 16
)

Dir.chdir(__dir__) do
  GR.savefig("data/iris.png")
end

puts "Press any key to exit"
gets # Wait for key input
