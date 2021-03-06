
#include <benchmark/benchmark.h>
#include <iostream>
#include <x86intrin.h>
constexpr size_t kCacheLineBytes = 64;
constexpr size_t kPageBytes = 4 * 1024;
constexpr size_t MAX_ROWS = 8 << 12;

inline void MemoryAndSpeculationBarrier() {
  // See docs/fencing.md
  _mm_mfence();
  _mm_lfence();
}

const void *StartOfNextCacheLine(const void *addr) {
  auto addr_n = reinterpret_cast<uintptr_t>(addr);

  // Create an address on the next cache line, then mask it to round it down to
  // cache line alignment.
  auto next_n = (addr_n + kCacheLineBytes) & ~(kCacheLineBytes - 1);
  return reinterpret_cast<const void *>(next_n);
}

inline void FlushDataCacheLineNoBarrier(const void *address) {
  _mm_clflush(address);
}

void FlushFromDataCache(const void *begin, const void *end) {
  for (; begin < end; begin = StartOfNextCacheLine(begin)) {
    FlushDataCacheLineNoBarrier(begin);
  }
  MemoryAndSpeculationBarrier();
}

using namespace std;
class Setup {
  Setup(benchmark::State &state) {
    // call your setup function
    a = (int *)(malloc((MAX_ROWS)*256 * sizeof(int)));
    std::cout << "singleton ctor called only once in the whole program"
              << state.range() << std::endl;
  }

public:
  static int *a;
  static int *PerformSetup(benchmark::State &state) {
    static Setup setup(state);
    return a;
  }
};

int *Setup::a = nullptr;

static void BM_Cache(benchmark::State &state) {
  int *a = Setup::PerformSetup(state);
  int local;
  //  a = (int *)malloc(state.range(0) * state.range(0) * sizeof(int));
  for (auto _ : state) {
    for (int i = 0; i < 10000; i++)
    {
      for (int j = 0; j < state.range(0)*256; j++)
      {
        local += a[j];
      }
    }
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(local);
  }
  //  if (a != nullptr) {
  // free(a);
  // }
 // FlushFromDataCache(a, a + ((MAX_ROWS)-1) * 255 + 255);
}

BENCHMARK(BM_Cache)->RangeMultiplier(2)->Range(8, 8 << 8);

BENCHMARK_MAIN();
