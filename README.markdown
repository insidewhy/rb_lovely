# rb\_lovely

Fast containers with good tests.

1. SortedSet: A sorted set with no duplicates like Ruby's [SortedSet](http://ruby-doc.org/stdlib-1.9.3/libdoc/set/rdoc/SortedSet.html) but built using C++ STL's std::set.

2. SortedHash: An associative array hashed by the keys ordered by the values. Used to build the [catmist matching algorithm](http://catmist.com) as [the builtin SortedSet class turned out not to be that great](http://architecturalatrocities.com/post/23659800703/the-ruby-standard-library-is-a-disgracene).
