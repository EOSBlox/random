# Random.h
Pseudo-random number generator for EOS.IO via a single C++ header file.

Since `<random>` is not available in EOS.IO, and they don't have good randomization primitives, we propose a simple pseudo-random number generator based on [xoroshiro128+](http://xoshiro.di.unimi.it) for generation and [splitmix64](https://dl.acm.org/citation.cfm?doid=2714064.2660195) for seed expansion.

* [Usage](#usage)
  * [Seeding the generator](#seeding-the-generator)
  * [Generating values](#generating-values)
  * [Shuffling containers](#shuffling-containers)
  * [Sampling from a population](#sampling-from-a-population)
* [Testing](#testing)

## Usage
To access the generator, all that is needed is to copy "Random.h" into the source folder and include it in the smart contract.

### Seeding the generator
The pseudo-random number generator operates on a 64-bit seed, which must be supplied manually for best randomness. It could be via a random number or cryptographic hash value retrieved via smart contract parameters.

If the constructor `Random::Random()` is used then the seed will be accumulated from the EOS.IO Transactions as Proof-of-Stake (TAPOS) block number and prefix in order to provide randomness of some kind. However, it is _not_ recommended to rely solely on this!

Examples:
```cpp
Random gen; // Seeded with TAPOS block number and prefix.
gen.accumSeed(42); // Provide extra seed info via number 42.

// Seed using EOS.IO types.
checksum256 hash; // Assume already constructed.
gem.accumSeed(hash);

// Seed using initializer lists and general containers.
gem.accumSeedRange({1, 2, 3});
gem.accumSeedRange(std::string("Hello, World!"));
gem.accumSeedRange(std::vector<int>{1, 2, 3});

// Seed using arrays.
const int arr[3]{1, 2, 3};
gen.accumSeedArray(arr);
```

The following EOS.IO types are directly supported via `Random::accumSeed()`:
* `checksum160`
* `checksum256`
* `checksum512`
* `signature`
* `public_key`

They are defined in `<eosiolib/types.h>`.

### Generating values
Once the generator has been seeded, there are three ways of generating random values:
* `uint64_t Random::next()`
* `double Random::nextDouble()`
* `uint64_t Random::nextInRange(const uint64_t min, const uint64_t max)`

Examples:
```cpp
Random gen;
gen.next();              // Value in [0, 18446744073709551615]
gen.nextDouble();        // Value in [0.0, 1.0[
gen.nextInRange(20, 50); // Value in [20, 50[
```

### Shuffling containers
General containers can be shuffled by using `Random::shuffle()`:
```cpp
Random gen;
std::string str("Hello, World!");
gen.shuffle(str);
// str has been reordered at this point.
```

Every element of the container is shuffled exactly once.

### Sampling from a population
General containers can be used for sampling a number of values from:
```cpp
Random gen;
const std::string pop("abcdef");
const sample = gen.sample(10, pop);
// sample contains 10 randomly selected values from pop at this point.
```

## Testing
A simple test suite can be compiled and executed like this:
```
% make test
c++ -Ofast -std=c++11 -DNO_EOSIO tests.cc -o tests
./tests
```

If a test fails it might look like the following:
```
% ./tests
Assertion failed in file tests.cc, line 120, func testNext():
  expected 849456367364724273
  got      7364724273
make: *** [test] Abort trap: 6
```
