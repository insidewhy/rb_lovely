require 'rb_lovely_sorted_set'

describe RbLovelySorted::Set do
  it "increases length when adding elements to set" do
    set = RbLovelySorted::Set.new
    set.add 14
    expect(set.length).to equal 1
    set.add 19
    expect(set.length).to equal 2
  end

  it "adds items to set in order according to <=> operator" do
    set = RbLovelySorted::Set.new
    class Item < Struct.new(:num)
      def <=> other
        return num - other.num
      end
    end

    args = [ Item.new(1), Item.new(99), Item.new(18), Item.new(50), Item.new(6) ]
    args.each { |a| set.add a }
    expect { |b| set.each(&b) }.to yield_successive_args(*args.sort)
  end

  it "mixes in method from Enumerable" do
    set = RbLovelySorted::Set.new
    set.add 2
    set.add 4
    set.add 1
    set.add 3
    set.add 5
    not_rejected = set.reject { |v| v % 2 != 0 }
    expect(not_rejected).to eql([2, 4])
  end

  it "does not add duplicates" do
    class NamedItem < Struct.new(:name, :num)
      def <=> other
        return num - other.num
      end
    end
    set = RbLovelySorted::Set.new
    set.add NamedItem.new('jimmy', 4)
    set.add NamedItem.new('anna', 2)
    expect(set.length).to equal 2
    set.add NamedItem.new('cats', 2)
    expect(set.length).to equal 2
  end

  it "#to_string looks nice" do
    set = RbLovelySorted::Set.new
    set.add 5
    set.add 3
    set.add 1
    expect(set.to_s).to eql "RbLovelySorted::Set { 1, 3, 5 }"
  end
end
