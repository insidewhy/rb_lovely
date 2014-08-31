# Ruby Lovely Sets

Has good tests.

1. SortedSet: like Ruby's [SortedSet](http://ruby-doc.org/stdlib-1.9.3/libdoc/set/rdoc/SortedSet.html) but built using C++ STL's std::set.

2. SortedHash: An associative array hashed by the keys ordered by the values. Used to build the [catmist matching algorithm](http://catmist.com) as [the builtin SortedSet class turned out not to be that great](http://architecturalatrocities.com/post/23659800703/the-ruby-standard-library-is-a-disgracene).

## Ordered Set

An ordered set with no duplicates.

### Adding elements
Compares elements using <=> methods to order elements, adding elements has complexity O(log(n)).
```ruby
class Person < Struct.new(:name, :age)
  def <=> other
    return age - other.age
  end
end

empty_set = RbLovelySets::SortedSet.new

set = RbLovelySets::SortedSet.new([ Person.new('Nyamuk', 2), Person.new('Cold Rain', 9999) ])
set.add Person.new('Beards', 15)
# << is the same as add, it works for arrays also
set << Person.new('Anna', 12)
set.add Person.new('Moust', 18)
expect(set.length).to equal 5

# Gond isn't added because <=> returns 0 for him and Anna
set.add Person.new('Gond', 12)
expect(set.length).to equal 5
```

### Removing elements

Deletes first element for which: (argument <=> element) == 0. Complexity: O(log(n)).
```ruby
set = RbLovelySets::SortedSet.new([ 1, 5, 3 ])
set.delete 3
expect(set.to_a).to eql [1, 5]
```

### Access to first and last elements
```ruby
set = RbLovelySets::SortedSet.new [4, 0, 2]
expect(set.first).to equal 0
expect(set.last).to equal 4
```

### Filtering
Complexity: O(n).
```ruby
set = RbLovelySets::SortedSet.new [0, 1, 2, 9]
set.reject!(&:odd?)
set.reject_first!(&:odd?)
set.select!(&:even?)
```

### Iteration
```ruby
set = RbLovelySets::SortedSet.new ['bustin', 'all my dreams']
set.each { |str| p str }
```

### Enumerable methods are mixed in
```ruby
set = RbLovelySets::SortedSet.new [12, 4, 42]
set.reject { |num| num < 15 }
```

### Removing first and last elements
```ruby
set = RbLovelySets::SortedSet.new [5, 2, 3]
set.pop
expect(set.to_a).to eql [2, 3]
set.shift
expect(set.to_a).to eql [3]
```

## SortedHash
A SortedHash provides hashed keys and ordered values. Values are compared using "<=>" and keys are tested for equality using "==". This class is only provided if boost is available on the system when the gem is installed as it is built using the [Boost Multi-index Containers Library](http://www.boost.org/doc/libs/1_56_0/libs/multi_index/doc/index.html).

### Adding elements
```ruby
set = RbLovelySets::SortedHash.new
set[20] = 5
set[9] = 1
set[2] = 16
set[20] = 4
expect(set[2]).to equal 16
expect(set.length).to equal 3
# proc is guarantee to get values in value order so:
#   9, 1
#   20, 4
#   2, 16:
set.each { |key, value| do_stuff(key, value) }
```

# TODO:

## Return enumerator when calling iterator function without block:
```ruby
iter = s.each
p iter.next
p iter.next
```

## More SortedHash methods
```ruby
set.delete key
# etc.
```
