#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>         // for usleep()
#include <dlfcn.h>          // for dlopen() and dlsym()
#include <sys/sysctl.h>     // for sysctl()
#include <sys/kdebug.h>     // for kdebug trace decode
#include <mach/mach_time.h> // for mach_absolute_time()

#define KPC_MAX_COUNTERS 32

typedef float       f32;
typedef double      f64;
typedef int8_t      i8;
typedef uint8_t     u8;
typedef int16_t     i16;
typedef uint16_t    u16;
typedef int32_t     i32;
typedef uint32_t    u32;
typedef int64_t     i64;
typedef uint64_t    u64;
typedef size_t      usize;

#define EVENT_NAME_MAX 8
typedef struct {
    const char *alias; /// name for print
    const char *names[EVENT_NAME_MAX]; /// name from pmc db
} event_alias;

int counters_init(const event_alias *profile_events, const usize ev_count, uint32_t *classes, usize *counter_map);

int start_counters(uint32_t classes, uint64_t *counters_0);

void stop_counting(void);

int get_thread_counters(u32 tid, u32 buf_count, u64 *buf);
