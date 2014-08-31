# Ruby Lovely Ordered Set

Like Ruby's [Sorted Set](http://ruby-doc.org/stdlib-1.9.3/libdoc/set/rdoc/SortedSet.html) but built using C++ STL's std::set.

```ruby
class Person < Struct.new(:name, :age)
  def <=> other
    return age - other.age
  end
end

set = RbLovelySorted::Set.new
set.add Person.new('Beards', 15)
set.add Person.new('Anna', 12)
set.add Person.new('Moust', 18)
expect(set.length).to equal 3

# Gond isn't added because <=> returns 0 for him and Anna
set.add Person.new('Gond', 12)
expect(set.length).to equal 3

# iterating
set.each { |person| p person }

# Enumerable methods are mixed in
set.reject { |person| p.age < 15 }
```
