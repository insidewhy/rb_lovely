require 'rb_lovely'

describe RbLovely::SortedHash do
  before :each do
    @hash = RbLovely::SortedHash.new
  end

  def make_hash *vals
    @hash = RbLovely::SortedHash.new vals
  end

  it "can add elements to hash" do
    expect(@hash[:o] = 9).to equal 9
    expect(@hash.length).to equal 1
    expect(@hash[:s] = 2).to equal 2
    expect(@hash.length).to equal 2
  end

  it "has empty?" do
    expect(@hash.empty?).to equal true
    @hash[:a] = 1
    expect(@hash.empty?).to equal false
  end

  it "constructs from array" do
    expect { @hash = RbLovely::SortedHash.new("string should be array") }.to raise_error
    make_hash :i, 5, :d, 5, :n, 1
    expect(@hash.to_a).to eql [[:n, 1], [:i, 5], [:d, 5]]
  end

  it "has working constructor shortcut" do
    @hash = RbLovely::SortedHash[:a, 3, :b, 1]
    expect(@hash.to_a).to eql [[:b, 1], [:a, 3]]
  end

  it "iterates in value order according to <=> operator" do
    make_hash :b, 14, :c, 1, :d, 99
    expect { |b| @hash.each(&b) }.to yield_successive_args([:c, 1], [:b, 14], [:d, 99])
  end

  it "does not add duplicates" do
    make_hash :b, 9, :d, 10
    expect(@hash.length).to equal 2
    @hash[:b] = 12
    expect(@hash.length).to equal 2
  end

  it "can access element by key" do
    make_hash :b, 9, :d, 10
    expect(@hash[:b]).to equal 9
    expect(@hash[:c]).to equal nil
    expect(@hash[:d]).to equal 10
  end

  it "can update element by key" do
    @hash[:b] = 9
    @hash[:b] = 14
    expect(@hash[:b]).to equal 14
  end

  it "mixes in method from Enumerable" do
    make_hash 2, 9, 15, 2, 16, 4, 3, 1
    not_rejected = @hash.reject { |k, v| v.odd? }
    expect(not_rejected).to eql([[15, 2], [16, 4]])
  end

  it "has nice #to_s return value" do
    make_hash :b, 5, :e, 2
    expect(@hash.to_s).to eql "RbLovely::SortedHash { :e => 2, :b => 5 }"
  end

  it "has working first_value and last_value methods" do
    expect(@hash.first_value).to eql nil
    expect(@hash.last_value).to eql nil
    make_hash 2, "a", 1, "z", 2000, "b"
    expect(@hash.first_value).to eql "a"
    expect(@hash.last_value).to eql "z"
  end

  it "has working first and last methods" do
    expect(@hash.first).to eql nil
    expect(@hash.last).to eql nil
    make_hash :b, 5, :e, 2
    expect(@hash.first).to eql [:e, 2]
    expect(@hash.last).to eql [:b, 5]
  end

  it "has working first_key and last_key methods" do
    expect(@hash.first_key).to eql nil
    expect(@hash.last_key).to eql nil
    make_hash :b, 5, :e, 2
    expect(@hash.first_key).to eql :e
    expect(@hash.last_key).to eql :b
  end

  it "has working delete" do
    make_hash :h, 3, :i, 1, :b, 7
    expect(@hash.delete :h).to equal 3
    expect(@hash.delete :y).to equal nil
    expect(@hash.to_a).to eql [[:i, 1], [:b, 7]]
  end

  it "has working pop" do
    make_hash :e, 1, :c, 2
    expect(@hash.pop).to eql [:c, 2]
    expect(@hash.to_a).to eql [[:e, 1]]
  end

  it "has working shift" do
    make_hash :e, 1, :c, 2
    expect(@hash.shift).to eql [:e, 1]
    expect(@hash.to_a).to eql [[:c, 2]]
  end

  it "has working shift_value" do
    make_hash :e, 1, :c, 2
    expect(@hash.shift_value).to equal 1
    expect(@hash.to_a).to eql [[:c, 2]]
  end

  it "has working shift_key" do
    make_hash :e, 1, :c, 2
    expect(@hash.shift_key).to equal :e
    expect(@hash.to_a).to eql [[:c, 2]]
  end

  it "has working pop" do
    make_hash :e, 1, :c, 2
    expect(@hash.pop).to eql [:c, 2]
    expect(@hash.to_a).to eql [[:e, 1]]
  end

  it "has working pop_value" do
    make_hash :e, 1, :c, 2
    expect(@hash.pop_value).to equal 2
    expect(@hash.to_a).to eql [[:e, 1]]
  end

  it "has working pop_key" do
    make_hash :e, 1, :c, 2
    expect(@hash.pop_key).to equal :c
    expect(@hash.to_a).to eql [[:e, 1]]
  end

  it "has working include? and has_key?" do
    make_hash :e, 1, :c, 2
    expect(@hash.include? :e).to equal true
    expect(@hash.has_key? :c).to equal true
    expect(@hash.key? 6).to equal false
  end

  it "can use custom sorting function" do
    # compare reverses normal sort order
    @hash = RbLovely::SortedHash.new(compare: proc { |a, b| b <=> a })
    @hash[:e] = 1
    @hash[:c] = 2
    @hash[:j] = 3
    expect(@hash.to_a).to eql [[:j, 3], [:c, 2], [:e, 1]]
  end

  it "has working #replace" do
    make_hash :e, 1, :f, 4
    expect(@hash.replace :e, 9).to equal 1
    expect(@hash[:e]).to equal 9
    expect(@hash.replace :h, 10).to equal nil
    expect(@hash[:h]).to equal 10
  end

  it "can hash string values" do
    @hash["yeah"] = 14
    expect(@hash["yeah"]).to equal 14
    expect(@hash.replace("yeah", 15)).to equal 14
    expect(@hash["yeah"]).to equal 15
  end

  it "has working #clear" do
    make_hash :e, 2, :c, 5
    @hash.clear
    expect(@hash.to_a).to eql []
  end

  it "protects values from garbage collector" do
    @hash[:n] = 'val'
    GC.start
    expect(@hash[:n]).to eql 'val'
  end

  it "protects keys from garbage collector" do
    @hash['apple'] = 'val'
    GC.start
    # TODO: this is not a valid test as it doesn't access the existing key value
    expect(@hash['apple']).to eql 'val'
  end
end
