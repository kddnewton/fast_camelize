# frozen_string_literal: true

require 'test_helper'

class FastCamelizeTest < Minitest::Test
  ACRONYMS = {
    'api' => 'API',
    'html' => 'HTML',
    'http' => 'HTTP',
    'restful' => 'RESTful',
    'w3c' => 'W3C',
    'phd' => 'PhD',
    'ror' => 'RoR',
    'ssl' => 'SSL'
  }.freeze

  CASES = {
    'API' => 'api',
    'APIController' => 'api_controller',
    'nokogiri::HTML' => 'nokogiri/html',
    'HTTPAPI' => 'http_api',
    'HTTP::Get' => 'http/get',
    'SSLError' => 'ssl_error',
    'RESTful' => 'restful',
    'RESTfulController' => 'restful_controller',
    'nested::RESTful' => 'nested/restful',
    'iHeartW3C' => 'i_heart_w3c',
    'PhDRequired' => 'phd_required',
    'iRoRU' => 'i_ror_u',
    'RESTfulHTTPAPI' => 'restful_http_api',
    'HTTP::RESTful' => 'http/restful',
    'HTTP::RESTfulAPI' => 'http/restful_api',
    'APIRESTful' => 'api_restful',
    'Capistrano' => 'capistrano',
    'capiController' => 'capi_controller',
    'httpsApis' => 'https_apis',
    'Html5' => 'html5',
    'Restfully' => 'restfully',
    'roRails' => 'ro_rails'
  }.freeze

  def test_basic
    assert_equal 'activeModel', 'active_model'.camelize
    assert_equal 'activeModel::Errors', 'active_model/errors'.camelize
  end

  def test_acronyms
    CASES.each do |camel, under|
      assert_equal(camel, under.camelize(acronyms: ACRONYMS))
    end
  end

  def test_version
    refute_nil ::FastCamelize::VERSION
  end
end
