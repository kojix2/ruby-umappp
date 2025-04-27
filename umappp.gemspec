# frozen_string_literal: true

require_relative "lib/umappp/version"

Gem::Specification.new do |spec|
  spec.name          = "umappp"
  spec.version       = Umappp::VERSION
  spec.authors       = ["kojix2"]
  spec.email         = ["2xijok@gmail.com"]

  spec.summary       = "Umap for Ruby"
  spec.description   = "Umappp wrapper for Ruby"
  spec.homepage      = "https://github.com/kojix2/ruby-umappp"
  spec.license       = "BSD-2-Clause"

  spec.files         = Dir["{ext,vendor}/**/*", "lib/**/*.rb", "LICENSE.txt", "README.md"]
  spec.require_paths = ["lib"]
  spec.extensions    = ["ext/umappp/extconf.rb"]

  spec.required_ruby_version = ">= 2.7"

  spec.add_dependency "numo-narray"
  spec.add_dependency "rice", ">= 4.5.0"
end
