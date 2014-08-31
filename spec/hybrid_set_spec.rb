require 'rb_lovely_sets'

describe RbLovelySets::HybridSet do
  before :each do
    @set = RbLovelySets::HybridSet.new
  end

  def make_set *vals
    @set = RbLovelySets::HybridSet.new vals
  end

  it "can add elements to set" do
    @set[15] = 9
    expect(@set.length).to equal 1
    @set[19] = 2
    expect(@set.length).to equal 2
  end

  it "iterates in value order according to <=> operator" do
    @set[2] = 14
    @set[3] = 1
    @set[4] = 99
    expect { |b| @set.each(&b) }.to yield_successive_args([3, 1], [2, 14], [4, 99])
  end

  it "does not add duplicates" do
    @set[2] = 9
    @set[4] = 10
    expect(@set.length).to equal 2
    @set[2] = 12
    expect(@set.length).to equal 2
  end

  it "can access element by key" do
    @set[2] = 9
    @set[4] = 10
    expect(@set[2]).to equal 9
    expect(@set[3]).to equal nil
    expect(@set[4]).to equal 10
  end

  it "can update element by key" do
    @set[2] = 9
    @set[2] = 14
    expect(@set[2]).to equal 14
  end
end
