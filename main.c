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
    }},
    { "fp", {
      "MAP_SIMD_UOP",
    }},
    { "int", {
      "MAP_INT_UOP",
    }}
};

float sqrt_serial_10(void) {
  float x = 1000.0;
  for (int i = 0; i < 100000; i++) {
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
  }
  return x;
}

float sqrt_serial_20(void) {
  float x = 1000.0;
  for (int i = 0; i < 100000; i++) {
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
    x = sqrt(x);
  }
  return x;
}

float sqrt_parallel(void) {
  float a = 1000.0;
  float b = 1000.0;
  float c = 1000.0;
  float d = 1000.0;
  float e = 1000.0;
  float f = 1000.0;
  float g = 1000.0;
  float h = 1000.0;

  for (int i=0; i < 100000; i++) {
    a = sqrt(a);
    b = sqrt(b);
    c = sqrt(c);
    d = sqrt(d);
    e = sqrt(e);
    f = sqrt(f);
    g = sqrt(g);
    h = sqrt(h);
  }

  return a + b + c + d + e + f + g + h;
}

float sqrt_parallel_wint(void) {
  float a = 1000.0;
  float b = 1000.0;
  float c = 1000.0;
  float d = 1000.0;
  float e = 1000.0;
  float f = 1000.0;
  float g = 1000.0;
  float h = 1000.0;

  int x = 5;
  int x1 = 5;
  int x2 = 5;
  int x3 = 5;
  int x4 = 5;
  int y = 5;
  int z = 5;

  for (int i=0; i < 100000; i++) {
    a = sqrt(a);
    b = sqrt(b);
    c = sqrt(c);
    d = sqrt(d);
    e = sqrt(e);
    f = sqrt(f);
    g = sqrt(g);
    h = sqrt(h);
    x = x+x;
    x1 = x1+x1;
    x2 = x2 + x2;
    x3 = x3 + x3;
    x4 = x4 + x4;
    y = y+y;
    z = z+z;
  }

  return a + b + c + d + e + f + g + h + (float)x + (float)y + (float)z + (float)x1 + (float)x2 + (float)x3 + (float)x4;
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
  float x = sqrt_serial_10();

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
  float y = sqrt_serial_20();

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

  /* PROFILE 20 */

  // get counters before
  if ((ret = get_thread_counters(0, KPC_MAX_COUNTERS, counters_0))) {
      printf("Failed get thread counters before: %d.\n", ret);
      return 1;
  }

  // code to be measured
  float z = sqrt_parallel();

  // get counters after
  if ((ret = get_thread_counters(0, KPC_MAX_COUNTERS, counters_1))) {
      printf("Failed get thread counters after: %d.\n", ret);
      return 1;
  }

  // result
  printf("For 100,000 loops of 8-wide:\n");
  for (usize i = 0; i < ev_count; i++) {
      const event_alias *alias = profile_events + i;
      usize idx = counter_map[i];
      u64 val = counters_1[idx] - counters_0[idx];
      printf("%14s: %llu\n", alias->alias, val);
  }

  // get counters before
  if ((ret = get_thread_counters(0, KPC_MAX_COUNTERS, counters_0))) {
      printf("Failed get thread counters before: %d.\n", ret);
      return 1;
  }

  // code to be measured
  float z1 = sqrt_parallel_wint();

  // get counters after
  if ((ret = get_thread_counters(0, KPC_MAX_COUNTERS, counters_1))) {
      printf("Failed get thread counters after: %d.\n", ret);
      return 1;
  }

  // result
  printf("For 100,000 loops of 8-wide:\n");
  for (usize i = 0; i < ev_count; i++) {
      const event_alias *alias = profile_events + i;
      usize idx = counter_map[i];
      u64 val = counters_1[idx] - counters_0[idx];
      printf("%14s: %llu\n", alias->alias, val);
  }

  // stop counting
  stop_counting();

  printf("%3.3f %3.3f %3.3f %3.3f\n", x, y, z, z1);

  // TODO: free memory
  return 0;
}
