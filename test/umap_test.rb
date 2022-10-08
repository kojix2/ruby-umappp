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
    r = nil
    assert_nothing_raised do
      r = Umappp.run(Numo::SFloat.new(10, 10).rand)
    end
    assert_instance_of Numo::SFloat, r
    assert_equal [10, 2], r.shape
  end
end
