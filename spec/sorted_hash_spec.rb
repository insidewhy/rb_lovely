require 'rb_lovely_sets'

describe RbLovelySets::SortedHash do
  before :each do
    @hash = RbLovelySets::SortedHash.new
  end

  def make_hash *vals
    @hash = RbLovelySets::SortedHash.new vals
  end

  it "can add elements to hash" do
    @hash[15] = 9
    expect(@hash.length).to equal 1
    @hash[19] = 2
    expect(@hash.length).to equal 2
  end

  it "iterates in value order according to <=> operator" do
    @hash[2] = 14
    @hash[3] = 1
    @hash[4] = 99
    expect { |b| @hash.each(&b) }.to yield_successive_args([3, 1], [2, 14], [4, 99])
  end

  it "does not add duplicates" do
    @hash[2] = 9
    @hash[4] = 10
    expect(@hash.length).to equal 2
    @hash[2] = 12
    expect(@hash.length).to equal 2
  end

  it "can access element by key" do
    @hash[2] = 9
    @hash[4] = 10
    expect(@hash[2]).to equal 9
    expect(@hash[3]).to equal nil
    expect(@hash[4]).to equal 10
  end

  it "can update element by key" do
    @hash[2] = 9
    @hash[2] = 14
    expect(@hash[2]).to equal 14
  end
end
