# frozen_string_literal: true

require "bundler/gem_tasks"
require "rake/testtask"

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList["test/**/*_test.rb"]
end

require "rake/extensiontask"

# Rake's default CLEAN includes **/core for core dumps, which matches Eigen's Core header.
CLEAN.exclude("vendor/Eigen/Core")

task build: :compile

Rake::ExtensionTask.new("umappp") do |ext|
  ext.lib_dir = "lib/umappp"
end

task default: %i[clobber compile test]
