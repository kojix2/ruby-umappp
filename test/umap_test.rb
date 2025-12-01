# frozen_string_literal: true

require "test_helper"

class UmapppTest < Test::Unit::TestCase
  test "VERSION" do
    assert Umappp.const_defined?(:VERSION)
  end

  test "default_parameters" do
    assert_instance_of(Hash, Umappp.default_parameters)
  end

  test "run" do
    embedding = Numo::SFloat.new(10, 10).rand
    r = assert_nothing_raised do
      Umappp.run(embedding)
    end
    assert_instance_of Numo::SFloat, r
    assert_equal [10, 2], r.shape
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
