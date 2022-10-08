# frozen_string_literal: true

require "test_helper"

class UmapppTest < Test::Unit::TestCase
  test "VERSION" do
    assert do
      ::Umappp.const_defined?(:VERSION)
    end
  end
end
