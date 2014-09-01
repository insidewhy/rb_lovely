require 'mkmf'
$CPPFLAGS += ' -std=c++0x'
with_cflags('-x c++') do
  # needed for SortedHash, optional
  have_header('boost/multi_index_container.hpp')
end
create_makefile('rb_lovely')
