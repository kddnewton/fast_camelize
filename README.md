# `String#camelize` Ruby Extension

[![Build Status](https://github.com/kddnewton/fast_camelize/workflows/Main/badge.svg)](https://github.com/kddnewton/fast_camelize/actions)
[![Gem Version](https://img.shields.io/gem/v/fast_camelize.svg)](https://rubygems.org/gems/fast_camelize)

`fast_camelize` is a C extension that provides a fast implementation of [ActiveSupport's `String#camelize` method](http://api.rubyonrails.org/classes/String.html#method-i-camelize).

## Is it fast?

At last check, these were the benchmarks (obtained by running `bin/bench` with ActiveSupport 6.1.3):

```
Warming up --------------------------------------
       ActiveSupport    20.000  i/100ms
        FastCamelize    74.000  i/100ms
Calculating -------------------------------------
       ActiveSupport    202.091  (± 7.4%) i/s -      1.020k in   5.081583s
        FastCamelize    974.025  (± 2.3%) i/s -      4.884k in   5.016944s

Comparison:
        FastCamelize:      974.0 i/s
       ActiveSupport:      202.1 i/s - 4.82x  (± 0.00) slower
```

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'fast_camelize'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install fast_camelize

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/kddnewton/fast_camelize.

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).
