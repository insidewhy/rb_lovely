require 'mkmf'
project = 'rb_lovely_sets'
create_makefile(project)

require 'rake/extensiontask'

spec = Gem::Specification.new do |s|
  s.name = project
  s.version = '0.3.2'
  s.authors = ['James Pike']
  s.email   = %w(gems@chilon.net)
  s.homepage = "https://github.com/nuisanceofcats/rb_lovely_sets"
  s.license = 'Expat'
  s.summary = 'A fast sorted set.'
  s.description = 'A fast sorted set built using std::set.'
  s.platform = Gem::Platform::RUBY
  s.files = FileList['ext/rb_lovely_sets/*.{cpp,hpp}']
  s.extensions = FileList['ext/**/extconf.rb']
end

# not really sure what this does but whatever
Gem::PackageTask.new(spec) { |pkg| pkg.gem_spec = spec }

Rake::ExtensionTask.new(project)

task :default => :compile

task :test => [:clobber, :compile] do
  system 'rspec'
end
