# frozen_string_literal: true

require_relative 'lib/fast_camelize/version'

Gem::Specification.new do |spec|
  spec.name    = 'fast_camelize'
  spec.version = FastCamelize::VERSION
  spec.authors = ['Kevin Deisz']
  spec.email   = ['kevin.deisz@gmail.com']

  spec.summary     = 'Fast String#camelize implementation'
  spec.description = 'Provides a C-optimized method for camelizing a string'
  spec.homepage    = 'https://github.com/kddeisz/fast_camelize'
  spec.license     = 'MIT'

  spec.files = Dir.chdir(__dir__) do
    `git ls-files -z`.split("\x0").reject do |f|
      f.match(%r{^(test|spec|features)/})
    end
  end

  spec.bindir        = 'exe'
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = %w[lib]
  spec.extensions    = ['ext/fast_camelize/extconf.rb']
end
