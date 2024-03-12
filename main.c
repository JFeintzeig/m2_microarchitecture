#include <math.h>

#include "kpc.h"

/// Event names from /usr/share/kpep/<name>.plist
static const event_alias profile_events[] = {
    {   "cycles", {
            "FIXED_CYCLES",                 // Apple A7-A15
    }},
    {   "instructions", {
            "FIXED_INSTRUCTIONS",           // Apple A7-A15
    }},
    { "stalls", {
      "MAP_STALL",
    }}
};

void profile_10(void) {
  volatile double x = 1000.0;
  for (int i = 0; i < 100000; i++) {
    x = sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(x))))))))));
  }
}

void profile_20(void) {
  volatile double x = 1000.0;
  for (int i = 0; i < 100000; i++) {
    x = sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(sqrt(x))))))))))))))))))));
  }
}
int main(int argc, const char * argv[]) {
  int ret = 0;

  // init and start counters
  uint32_t classes = 0;
  const usize ev_count = sizeof(profile_events) / sizeof(profile_events[0]);
  size_t counter_map[KPC_MAX_COUNTERS] = { 0 };
  uint64_t counters_0[KPC_MAX_COUNTERS] = { 0 };
  uint64_t counters_1[KPC_MAX_COUNTERS] = { 0 };
  if ((ret = counters_init(&profile_events, ev_count, &classes, &counter_map))) {
    printf("problem w/counters_init(): %d", ret);
    return 1;
  }
  if ((ret = start_counters(classes, counters_0))) {
    printf("problem starting counters: %d\n", ret);
    return 1;
  }

  /* PROFILE 10 */

  // get counters before
  if ((ret = get_thread_counters(0, KPC_MAX_COUNTERS, counters_0))) {
      printf("Failed get thread counters before: %d.\n", ret);
      return 1;
  }
  // code to be measured
  profile_10();

  // get counters after
  if ((ret = get_thread_counters(0, KPC_MAX_COUNTERS, counters_1))) {
      printf("Failed get thread counters after: %d.\n", ret);
      return 1;
  }

  // result
  printf("For 100,000 loops of 10x:\n");
  for (usize i = 0; i < ev_count; i++) {
      const event_alias *alias = profile_events + i;
      usize idx = counter_map[i];
      u64 val = counters_1[idx] - counters_0[idx];
      printf("%14s: %llu\n", alias->alias, val);
  }

  /* PROFILE 20 */

  // get counters before
  if ((ret = get_thread_counters(0, KPC_MAX_COUNTERS, counters_0))) {
      printf("Failed get thread counters before: %d.\n", ret);
      return 1;
  }
  // code to be measured
  profile_20();

  // get counters after
  if ((ret = get_thread_counters(0, KPC_MAX_COUNTERS, counters_1))) {
      printf("Failed get thread counters after: %d.\n", ret);
      return 1;
  }

  // result
  printf("For 100,000 loops of 20x:\n");
  for (usize i = 0; i < ev_count; i++) {
      const event_alias *alias = profile_events + i;
      usize idx = counter_map[i];
      u64 val = counters_1[idx] - counters_0[idx];
      printf("%14s: %llu\n", alias->alias, val);
  }
  // stop counting
  stop_counting();

  // TODO: free memory
  return 0;
}
