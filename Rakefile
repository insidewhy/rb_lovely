require 'mkmf'
project = 'rb_lovely'
create_makefile(project)

require 'rake/extensiontask'

spec = Gem::Specification.new do |s|
  s.name = project
  s.version = '0.6.3'
  s.authors = ['James Pike']
  s.email   = %w(gems@chilon.net)
  s.homepage = "https://github.com/nuisanceofcats/rb_lovely"
  s.license = 'Expat'
  s.summary = 'Fast sorted sets and hashes.'
  s.description = 'A fast sorted set built using std::set and a fast sorted hash built using boost::multi_index_container.'
  s.platform = Gem::Platform::RUBY
  s.files = FileList['ext/rb_lovely/*.{cpp,hpp}', 'yard.rb', '.yardopts']
  s.extensions = FileList['ext/**/extconf.rb']
end

# not really sure what this does but whatever
Gem::PackageTask.new(spec) { |pkg| pkg.gem_spec = spec }

Rake::ExtensionTask.new(project)

task :default => :compile

task :test => [:clobber, :compile] do
  system 'rspec'
end
