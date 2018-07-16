// EOS Blox - Pseudo-Random Number Generator
#ifndef EOS_BLOX_RANDOM_H
#define EOS_BLOX_RANDOM_H

#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <vector>

#include <eosiolib/transaction.h>
#include <eosiolib/types.h>

namespace eosblox {

/// Pseudo-random number generator.
class Random {
public:
  /// Initialize generator with a seed derived from the Transactions as Proof-of-Stake (TAPOS)
  /// block number and prefix.
  Random()
  {
    accumSeedRange({std::abs(tapos_block_num()), std::abs(tapos_block_prefix())});
  }

  Random(const uint64_t seed) : seed_(seed)
  {
  }

  uint64_t seed() const
  {
    return seed_;
  }

  /// Accumulate seed with \p extra multiplied with the next internal prime.
  void accumSeed(const uint64_t extra)
  {
    seed_ *= extra * nextPrime();
  }

  void accumSeed(const checksum160 &extra)
  {
    accumSeedArray(extra.hash);
  }

  void accumSeed(const checksum256 &extra)
  {
    accumSeedArray(extra.hash);
  }

  void accumSeed(const checksum512 &extra)
  {
    accumSeedArray(extra.hash);
  }

  void accumSeed(const signature &extra)
  {
    accumSeedArray(extra.data);
  }

  void accumSeed(const public_key &extra)
  {
    accumSeedArray(extra.data);
  }

  template <typename T>
  void accumSeedRange(std::initializer_list<T> &&extra)
  {
    accumSeedRange(extra);
  }

  template <typename Range>
  void accumSeedRange(Range &&extra)
  {
    for (auto it = extra.begin(); it != extra.end(); ++it) {
      accumSeed(*it);
    }
  }

  template <typename T, std::size_t N>
  void accumSeedArray(const T (&extra)[N])
  {
    for (int i = 0; i < N; ++i) {
      accumSeed(extra[i]);
    }
  }

  /// Next pseudo-random number.
  /** next() is based upon xoroshiro128+ at http://xoshiro.di.unimi.it */
  uint64_t next()
  {
    // If state is {0, 0} then use seed to initialize.
    if (state[0] == 0 && state[1] == 0) {
      // TODO: Use splitmix64 with input of seed_ instead to expand seed to 128 bits.
      state[0] = seed_;
      state[1] = seed_ * nextPrime();
    }

    const auto s0 = state[0];
    auto s1 = state[1];
    const auto res = s0 + s1;
    s1 ^= s0;
    state[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16);
    state[1] = rotl(s1, 37);
    return res;
  }

private:
  uint64_t nextPrime()
  {
    return primes[primeIndex++ % primes.size()];
  }

  inline uint64_t rotl(const uint64_t x, int k)
  {
    return (x << k) | (x >> (64 - k));
  }

  uint64_t seed_ = 1;
  uint64_t state[2] = {0, 0};
  int primeIndex = 0;
  const std::vector<uint64_t> primes = {
    7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283, 7297, 7307, 7309, 7321, 7331, 7333,
    7349, 7351, 7369, 7393, 7411, 7417, 7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499,
    7507, 7517, 7523, 7529, 7537, 7541, 7547, 7549, 7559, 7561, 7573, 7577, 7583, 7589, 7591,
    7603, 7607, 7621, 7639, 7643, 7649, 7669, 7673, 7681, 7687, 7691, 7699, 7703, 7717, 7723,
    7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823, 7829, 7841, 7853, 7867, 7873, 7877,
    7879, 7883, 7901, 7907, 7919, 7927, 7933, 7937, 7949, 7951, 7963, 7993, 8009, 8011, 8017,
    8039, 8053, 8059, 8069, 8081, 8087, 8089, 8093, 8101, 8111};
};

} // eosblox

#endif // EOS_BLOX_RANDOM_H
