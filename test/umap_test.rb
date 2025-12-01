# frozen_string_literal: true

require "test_helper"

class UmapppTest < Test::Unit::TestCase
  test "VERSION" do
    assert Umappp.const_defined?(:VERSION)
  end

  test "default_parameters" do
    params = Umappp.default_parameters
    assert_instance_of(Hash, params)

    expected_keys = %i[
      local_connectivity
      bandwidth
      mix_ratio
      spread
      min_dist
      a
      b
      repulsion_strength
      initialize
      num_epochs
      learning_rate
      negative_sample_rate
      num_neighbors
      seed
      num_threads
      parallel_optimization
    ]

    assert_equal expected_keys.sort, params.keys.map(&:to_sym).sort
  end

  test "run" do
    embedding = Numo::SFloat.new(10, 10).rand
    r = assert_nothing_raised do
      Umappp.run(embedding)
    end
    assert_instance_of Numo::SFloat, r
    assert_equal [10, 2], r.shape
  end

  # Ensure that Umappp.run accepts various non-default options at once
  # (including threading-related params) without raising exceptions.
  test "run with various options" do
    embedding = Numo::SFloat.new(50, 10).rand

    assert_nothing_raised do
      Umappp.run(
        embedding,
        method: :vptree,              # default :annoy
        ndim: 3,                      # default 2
        num_threads: 4,               # default Umap::Defaults::num_threads
        parallel_optimization: true,  # default Umap::Defaults::parallel_optimization
        local_connectivity: 2.0,      # default Umap::Defaults::local_connectivity
        bandwidth: 1.5,               # default Umap::Defaults::bandwidth
        mix_ratio: 0.7,               # default Umap::Defaults::mix_ratio
        spread: 1.2,                  # default Umap::Defaults::spread
        min_dist: 0.05,               # default Umap::Defaults::min_dist
        a: 1.8956,                    # default Umap::Defaults::a
        b: 0.8006,                    # default Umap::Defaults::b
        repulsion_strength: 1.0,      # default Umap::Defaults::repulsion_strength
        initialize: :spectral,        # default Umap::Defaults::initialize
        num_epochs: 100,              # default Umap::Defaults::num_epochs
        learning_rate: 0.5,           # default Umap::Defaults::learning_rate
        negative_sample_rate: 10,     # default Umap::Defaults::negative_sample_rate
        num_neighbors: 30,            # default Umap::Defaults::num_neighbors
        seed: 123                     # default Umap::Defaults::seed
      )
    end
  end

  test "run with initialize as symbol" do
    embedding = Numo::SFloat.new(20, 5).rand

    assert_nothing_raised do
      Umappp.run(embedding, initialize: :spectral)
    end
  end

  test "run with invalid initialize symbol" do
    embedding = Numo::SFloat.new(20, 5).rand

    assert_raise(ArgumentError) do
      Umappp.run(embedding, initialize: :foo)
    end
  end

  test "one dimensional embedding" do
    embedding = Numo::SFloat.new(10).rand
    assert_raise(ArgumentError) do
      Umappp.run(embedding)
    end
  end

  test "empty embedding" do
    embedding = Numo::SFloat.new(0, 10)
    assert_raise(ArgumentError) do
      Umappp.run(embedding)
    end
  end

  test "unknown option" do
    embedding = Numo::SFloat.new(10, 10).rand
    assert_raise(ArgumentError) do
      Umappp.run(embedding, foo: 1)
    end
  end

  test "invalid method" do
    embedding = Numo::SFloat.new(10, 10).rand
    assert_raise(ArgumentError) do
      Umappp.run(embedding, method: :foo)
    end
  end

  test "invalid ndim" do
    embedding = Numo::SFloat.new(10, 10).rand
    assert_raise(RuntimeError) do
      Umappp.run(embedding, ndim: -1)
    end
  end
end
