# rb\_lovely

Fast containers with good tests.

1. SortedSet: A sorted set with no duplicates like Ruby's [SortedSet](http://ruby-doc.org/stdlib-1.9.3/libdoc/set/rdoc/SortedSet.html) but built using C++ STL's std::set.

2. SortedHash: An associative array hashed by the keys ordered by the values. Used to build the [catmist matching algorithm](http://catmist.com) as [the builtin SortedSet class turned out not to be that great](http://architecturalatrocities.com/post/23659800703/the-ruby-standard-library-is-a-disgracene).

## SortedSet

An ordered set with no duplicates. Do not modify while iterating.

### Adding elements
Compares elements using <=> methods to order elements, adding elements has complexity O(log(n)).
```ruby
class Person < Struct.new(:name, :age)
  def <=> other
    return age - other.age
  end
end

empty_set = RbLovely::SortedSet.new

set = RbLovely::SortedSet.new [ Person.new('Nyamuk', 2), Person.new('Cold Rain', 9999) ]
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
set = RbLovely::SortedSet.new [ 1, 5, 3 ]
set.delete 3
expect(set.to_a).to eql [1, 5]
```

### Access to first and last elements
Complexity: O(c)
```ruby
set = RbLovely::SortedSet.new [4, 0, 2]
expect(set.first).to equal 0
expect(set.last).to equal 4
```

### Filtering
Complexity: O(n).
```ruby
set = RbLovely::SortedSet.new [0, 1, 2, 9]
set.reject!(&:odd?)
set.reject_first!(&:odd?)
set.select!(&:even?)
```

### Iteration
```ruby
set = RbLovely::SortedSet.new ['bustin', 'all my dreams']
set.each { |str| p str }
```

### Enumerable methods are mixed in
```ruby
set = RbLovely::SortedSet.new [12, 4, 42]
set.reject { |num| num < 15 }
```

### Removing first and last elements
Complexity: O(c)
```ruby
set = RbLovely::SortedSet.new [5, 2, 3]
set.pop
expect(set.to_a).to eql [2, 3]
set.shift
expect(set.to_a).to eql [3]
```

## SortedHash
A SortedHash provides hashed keys and ordered values. Values are compared using "<=>" and keys are tested for equality using "eql?". This class is only provided if boost is available on the system when the gem is installed as it is built using the [Boost Multi-index Containers Library](http://www.boost.org/doc/libs/1_56_0/libs/multi_index/doc/index.html). Do not modify while iterating.

### Adding elements
```ruby
empty_hash = RbLovely::SortedHash.new

# constructor is like: hash[20] = 5 ; hash[9] = 1
hash = RbLovely::SortedHash.new [20, 5, 9, 1]
hash[2] = 16
hash[20] = 4 # updates previous value
expect(hash[20]).to equal 4
expect(hash.length).to equal 3
```

### Iteration
```ruby
hash = RbLovely::SortedHash.new [20, 5, 9, 1, 2, 16]
# proc is guarantee to get values in value order so:
#   9, 1
#   20, 4
#   2, 16:
hash.each { |key, value| do_stuff(key, value) }
```

### Overriding comparison function
```ruby
hash = RbLovely::SortedHash.new(compare: proc { |a, b| b <=> a })
hash[1] = 15
hash[5] = 1
# the order of values will be reversed due to the compare function
hash.each { |key, value| do_stuff(key, value) }
```

### Retrieving elements
Complexity: O(c)
```ruby
hash = RbLovely::SortedHash.new [20, 5, 9, 1, 2, 16]
expect(hash[20]).to equal(5)
expect(hash.include? 9).to equal true
expect(hash.has_key? 2).to equal true
expect(hash.first).to equal 1  # lowest value
expect(hash.last).to equal 16  # highest value
```

### Removing elements
Complexity: O(c)
```ruby
hash = RbLovely::SortedHash.new [9, 2, 3, 4]
expect(hash.delete 9).to equal 2
expect(hash.delete 10).to equal nil
expect(hash.to_a).to eql [[3, 4]]
# etc. etc.
```

