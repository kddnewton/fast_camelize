# frozen_string_literal: true

require 'test_helper'

class FastCamelizeTest < Minitest::Test
  def test_basic
    assert_equal 'ActiveModel', 'active_model'.camelize
    assert_equal 'ActiveModel::Errors', 'active_model/errors'.camelize
  end

  def test_version
    refute_nil ::FastCamelize::VERSION
  end
end
