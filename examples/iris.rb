# frozen_string_literal: true

require "umappp"
require "datasets"
require "numo/gnuplot"

iris = Datasets::Iris.new

d = iris.to_table
        .fetch_values(
          :sepal_length,
          :sepal_width,
          :petal_length,
          :petal_width
        ).transpose

r = Umappp.run(d)

pp r
