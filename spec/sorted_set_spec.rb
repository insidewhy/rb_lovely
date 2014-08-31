require 'rb_lovely_sets'

describe RbLovelySets::SortedSet do
  before :each do
    @set = RbLovelySets::SortedSet.new
  end

  def make_set *vals
    @set = RbLovelySets::SortedSet.new vals
  end

  it "has working add, << and length methods" do
    @set.add 14
    expect(@set.length).to equal 1
    @set << 19
    expect(@set.length).to equal 2
  end

  it "adds items to set in order according to <=> operator and iterates with each" do
    class Item < Struct.new(:num)
      def <=> other
        return num - other.num
      end
    end

    args = [ Item.new(1), Item.new(99), Item.new(18), Item.new(50), Item.new(6) ]
    args.each { |a| @set.add a }
    expect { |b| @set.each(&b) }.to yield_successive_args(*args.sort)
  end

  it "constructs from array" do
    expect { @set = RbLovelySets::SortedSet.new("string should be array") }.to raise_error
    make_set 9, 5, 4, 5
    expect(@set.to_a).to eql [4, 5, 9]
  end

  it "does not add duplicates" do
    class NamedItem < Struct.new(:name, :num)
      def <=> other
        return num - other.num
      end
    end
    make_set NamedItem.new('jimmy', 4), NamedItem.new('anna', 2)
    expect(@set.length).to equal 2
    @set.add NamedItem.new('cats', 2)
    expect(@set.length).to equal 2
  end

  it "mixes in method from Enumerable" do
    make_set 2, 4, 1, 3, 5
    not_rejected = @set.reject { |v| v.odd? }
    expect(not_rejected).to eql([2, 4])
  end

  it "has nice #to_s return value" do
    make_set "the green", "manalishi", "with the two pronged crown"
    expect(@set.to_s).to eql "RbLovelySets::SortedSet { manalishi, the green, with the two pronged crown }"
  end

  it "has working last and first methods" do
    expect(@set.first).to equal nil
    expect(@set.last).to equal nil

    make_set 2, 4, 0
    expect(@set.first).to equal 0
    expect(@set.last).to equal 4
  end

  it "has working delete" do
    make_set 1, 5, 3
    expect(@set.delete 4).to eql nil
    expect(@set.delete 3).to eql 3
    expect(@set.to_a).to eql [1, 5]
  end

  it "has working reject!" do
    make_set 8, 3, 2, 1
    @set.reject!(&:odd?)
    expect(@set.to_a).to eql [2, 8]
  end

  it "has working reject_first!" do
    make_set 8, 3, 2, 1
    rejected = @set.reject_first!(&:odd?)
    expect(rejected).to equal 1
    expect(@set.to_a).to eql [2, 3, 8]

    rejected = @set.reject_first! { |val| val > 8 }
    expect(rejected).to equal nil
  end

  it "has working select!" do
    make_set 9, 8, 1, 4
    @set.select!(&:even?)
    expect(@set.to_a).to eql [4, 8]
  end

  it "has working pop" do
    make_set 5, 2, 3
    expect(@set.pop).to equal 5
    expect(@set.to_a).to eql [2, 3]
  end

  it "has working shift" do
    make_set 5, 2, 3
    expect(@set.shift).to equal 2
    expect(@set.to_a).to eql [3, 5]
  end

  it "has working include?" do
    make_set 5, 2, 3
    expect(@set.include? 2).to equal true
    expect(@set.include? 4).to equal false
  end
end
