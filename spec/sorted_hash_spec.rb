require 'rb_lovely'

describe RbLovely::SortedHash do
  before :each do
    @hash = RbLovely::SortedHash.new
  end

  def make_hash *vals
    @hash = RbLovely::SortedHash.new vals
  end

  it "can add elements to hash" do
    @hash[15] = 9
    expect(@hash.length).to equal 1
    @hash[19] = 2
    expect(@hash.length).to equal 2
  end

  it "constructs from array" do
    expect { @hash = RbLovely::SortedHash.new("string should be array") }.to raise_error
    make_hash 9, 5, 4, 5, 14, 1
    expect(@hash.to_a).to eql [[14, 1], [9, 5], [4, 5]]
  end

  it "iterates in value order according to <=> operator" do
    make_hash 2, 14, 3, 1, 4, 99
    expect { |b| @hash.each(&b) }.to yield_successive_args([3, 1], [2, 14], [4, 99])
  end

  it "does not add duplicates" do
    make_hash 2, 9, 4, 10
    expect(@hash.length).to equal 2
    @hash[2] = 12
    expect(@hash.length).to equal 2
  end

  it "can access element by key" do
    make_hash 2, 9, 4, 10
    expect(@hash[2]).to equal 9
    expect(@hash[3]).to equal nil
    expect(@hash[4]).to equal 10
  end

  it "can update element by key" do
    @hash[2] = 9
    @hash[2] = 14
    expect(@hash[2]).to equal 14
  end

  it "mixes in method from Enumerable" do
    make_hash 2, 9, 15, 2, 16, 4, 3, 1
    not_rejected = @hash.reject { |k, v| v.odd? }
    expect(not_rejected).to eql([[15, 2], [16, 4]])
  end

  it "has nice #to_s return value" do
    make_hash 2, 5, 5, 2
    @hash[2] = 5
    @hash[5] = 2
    expect(@hash.to_s).to eql "RbLovely::SortedHash { 5 => 2, 2 => 5 }"
  end

  it "has working last and first methods" do
    expect(@hash.first).to eql nil
    expect(@hash.last).to eql nil
    make_hash 2, "a", 1, "z", 2000, "b"
    expect(@hash.first).to eql "a"
    expect(@hash.last).to eql "z"
  end

  it "has working delete" do
    make_hash 8, 3, 9, 1, 2, 7
    expect(@hash.delete 8).to equal 3
    expect(@hash.delete 19).to equal nil
    expect(@hash.to_a).to eql [[9, 1], [2,7]]
  end

  it "has working pop" do
    make_hash 5, 1, 3, 2
    expect(@hash.pop).to equal 2
    expect(@hash.to_a).to eql [[5,1]]
  end

  it "has working shift" do
    make_hash 5, 1, 3, 2
    expect(@hash.shift).to equal 1
    expect(@hash.to_a).to eql [[3,2]]
  end

  it "has working include? and has_key?" do
    make_hash 5, 1, 3, 2
    expect(@hash.include? 5).to equal true
    expect(@hash.has_key? 3).to equal true
    expect(@hash.has_key? 6).to equal false
  end

  it "can use custom sorting function" do
    # compare reverses normal sort order
    @hash = RbLovely::SortedHash.new(compare: proc { |a, b| b <=> a })
    @hash[5] = 1
    @hash[3] = 2
    @hash[10] = 3
    expect { |b| @hash.each(&b) }.to yield_successive_args([10, 3], [3, 2], [5, 1])
  end

  it "can update and return previous with update" do
    make_hash 5, 1, 6, 4
    expect(@hash.replace 5, 9).to equal 1
    expect(@hash[5]).to equal 9
    expect(@hash.replace 8, 10).to equal nil
    expect(@hash[8]).to equal 10
  end

  it "has working #clear" do
    make_hash 5, 2, 3, 5
    @hash.clear
    expect(@hash.to_a).to eql []
  end
end
