#include "Random.h"
using namespace eosblox;

#include <chrono>
#include <functional>
#include <iostream>
#include <string>

using steady_clock = std::chrono::steady_clock;
using time_point = steady_clock::time_point;
using ms = std::chrono::milliseconds;

time_point now()
{
  return steady_clock::now();
}

double durationSecs(const time_point &start, const time_point &end)
{
  return static_cast<double>(std::chrono::duration_cast<ms>(end - start).count()) / 1000.0;
}

double timeit(const std::function<void()> &func, const std::string &msg, const int runs = 3)
{
  using namespace std;
  cout << msg << ".." << endl;

  double avg = 0;
  for (int i = 0; i < runs; ++i) {
    const auto start = now();
    func();
    const auto end = now();
    const auto seconds = durationSecs(start, end);
    cout << "  " << seconds << "s";
    cout.flush();
    avg += seconds;
  }
  avg /= static_cast<double>(runs);
  cout << "  [~" << avg << "s]";
  cout << endl << endl;
  return avg;
}

void seeding()
{
  Random gen(1);
  const std::string range("abcdefghijklmnopqrstuvwxyz");
  for (uint64_t i = 0; i < 5'000'000; ++i) {
    gen.accumSeedRange(range);
  }
}

void next()
{
  Random gen(42);
  for (uint64_t i = 0; i < 1'000'000'000; ++i) {
    gen.next();
  }
}

void shuffle()
{
  Random gen(42);
  std::string str("abcdefghijklmnopqrstuvwxyz");
  for (uint64_t i = 0; i < 5'000'000; ++i) {
    gen.shuffle(str);
  }
}

void sample()
{
  Random gen(42);
  std::string str("abcdefghijklmnopqrstuvwxyz");
  for (uint64_t i = 0; i < 10'000'000; ++i) {
    gen.sample(10, str);
  }
}

int main(int argc, char **argv)
{
  double total = 0;
  const auto timeitAccum = [&total](const auto &func, const auto &msg) {
    total += timeit(func, msg);
  };
  timeitAccum(seeding, "Seeding 26 chars 5'000'000 times");
  timeitAccum(next, "1'000'000'000 nexts");
  timeitAccum(shuffle, "Shuffling 26 char string 5'000'000 times");
  timeitAccum(sample, "Sampling 10 chars 10'000'000 times from 26 char population");
  std::cout << "Total avg duration: " << total << "s" << std::endl;
  return 0;
}
