# frozen_string_literal: true

require "umappp"
require "datasets-numo-narray" # https://github.com/red-data-tools/red-datasets-numo-narray
require "numo/gnuplot"

iris = Datasets::LIBSVM.new("iris").to_narray
d = iris[true, 1..-1]
l = iris[true, 0]

r = Umappp.run(d)
x = r[true, 0]
y = r[true, 1]

plots = (1..3).map do |i|
  [x[l.eq(i)], y[l.eq(i)], { title: "#{i}", with: "points", pt: 7, pointsize: 1 }]
end

Dir.chdir(__dir__) do
  Numo.gnuplot do
    set terminal: "pngcairo"
    set output: "data/iris_gnuplot.png"
    plot(*plots)
  end
  puts "Saved to data/iris_gnuplot.png"
end
