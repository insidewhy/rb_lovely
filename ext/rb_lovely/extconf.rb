require 'mkmf'
$CPPFLAGS += ' -std=c++0x'
with_cflags('-x c++') do
  # needed for SortedHash, optional
  unless have_header 'boost/multi_index_container.hpp'
    warn 'Could not find boost, disabling SortedHash'
  end
end
create_makefile('rb_lovely')
