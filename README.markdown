# Ruby Lovely Ordered Set

Like Ruby's [Sorted Set](http://ruby-doc.org/stdlib-1.9.3/libdoc/set/rdoc/SortedSet.html) but built using C++ STL's std::set. Has good tests. Used to build the [catmist matching algorithm](http://catmist.com) as [the builtin OrderedSet class turned out not to be that great](http://architecturalatrocities.com/post/23659800703/the-ruby-standard-library-is-a-disgracene).

## Adding elements to a set
Compares elements using <=> methods to order elements:
```ruby
class Person < Struct.new(:name, :age)
  def <=> other
    return age - other.age
  end
end

empty_set = RbLovelySorted::Set.new

set = RbLovelySorted::Set.new([ Person.new('Nyamuk', 2), Person.new('Cold Rain', 9999) ])
set.add Person.new('Beards', 15)
# << is the same as add, it works for arrays also
set << Person.new('Anna', 12)
set.add Person.new('Moust', 18)
expect(set.length).to equal 5

# Gond isn't added because <=> returns 0 for him and Anna
set.add Person.new('Gond', 12)
expect(set.length).to equal 5
```

## Removing elements

Deletes first element for which: (argument <=> element) == 0
```ruby
set = RbLovelySorted::Set.new([ 1, 5, 3 ])
set.delete 3
expect(set.to_a).to eql [1, 5]
```

## Access to first and last elements
```ruby
set = RbLovelySorted::Set.new [4, 0, 2]
expect(set.first).to equal 0
expect(set.last).to equal 4
```

## Filtering the set
```ruby
set = RbLovelySorted::Set.new [0, 1, 2, 9]
set.reject!(&:odd?)
set.reject_first!(&:odd?)
set.select!(&:even?)
```

## Iteration
```ruby
set = RbLovelySorted::Set.new ['bustin', 'all my dreams']
set.each { |str| p str }
```

## Enumerable methods are mixed in
```ruby
set = RbLovelySorted::Set.new [12, 4, 42]
set.reject { |num| num < 15 }
```

# TODO:

Return enumerator when calling iterator function without block:
```ruby
iter = s.each
p iter.next
p iter.next
```
