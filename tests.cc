#include "Random.h"
using namespace eosblox;

#include <cassert>
#include <cmath>
#include <string>
#include <vector>

static inline void assertSeedEq(const Random &gen, const uint64_t seed)
{
  assert(gen.seed() == seed);
}

static inline void assertNextEq(Random &gen, const uint64_t value)
{
  assert(gen.next() == value);
}

static inline bool doubleEqual(const double a, const double b)
{
  constexpr double epsilon = 0.0000001;
  return std::abs(a - b) <= ((std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
}

static inline void assertNextDoubleEq(Random &gen, const double value)
{
  assert(doubleEqual(gen.nextDouble(), value));
}

void testSeedAccumulation()
{
  Random gen(1);
  assertSeedEq(gen, 1);

  // Integer.
  gen.accumSeed(1);
  assertSeedEq(gen, 11295943761408656531LLU);

  // Initializer list.
  gen.accumSeedRange({1, 2, 3});
  assertSeedEq(gen, 18083823154816788002LLU);

  // General ranges.
  gen.accumSeedRange(std::string("Hello, World!"));
  assertSeedEq(gen, 7880214006551886600LLU);

  const std::string str("Hello, World!");
  gen.accumSeedRange(str);
  assertSeedEq(gen, 11498166657420254716LLU);

  gen.accumSeedRange(std::vector<int>{1, 2, 3});
  assertSeedEq(gen, 4175012130207295590LLU);

  // Arrays.
  const int arr[3]{1, 2, 3};
  gen.accumSeedArray(arr);
  assertSeedEq(gen, 4326221722869819695LLU);
}

void testNext()
{
  {
    Random gen(1);
    assertNextEq(gen, 17243114145050246623LLU);
    assertNextEq(gen, 5516405404208600209LLU);
    assertNextEq(gen, 4897608421861629041LLU);
    assertNextEq(gen, 8669024630748878125LLU);
    assertNextEq(gen, 13562114715725036114LLU);
  }
  {
    Random gen(111970);
    assertNextEq(gen, 8987405948927848382LLU);
    assertNextEq(gen, 4621043010224079033LLU);
    assertNextEq(gen, 15658672667220659145LLU);
    assertNextEq(gen, 14814678247671488241LLU);
    assertNextEq(gen, 849456367364724273LLU);
  }
  {
    Random gen(1);
    gen.accumSeedRange(std::string("Hello, World!"));
    assertNextEq(gen, 14607242223934345243LLU);
    assertNextEq(gen, 12308690566605765243LLU);
    assertNextEq(gen, 6585550730876598598LLU);
    assertNextEq(gen, 5507355692446769180LLU);
    assertNextEq(gen, 16383900445848122088LLU);
  }
}

void testNextDouble()
{
  Random gen(42);
  assertNextDoubleEq(gen, 8.485680e-02);
  assertNextDoubleEq(gen, 3.902738e-01);
  assertNextDoubleEq(gen, 2.926740e-01);
  assertNextDoubleEq(gen, 7.088247e-01);
  assertNextDoubleEq(gen, 8.789325e-01);
}

int main(int argc, char **argv)
{
  testSeedAccumulation();
  testNext();
  testNextDouble();
  return 0;
}
