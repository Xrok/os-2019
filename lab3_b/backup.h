static inline bool casX(volatile unsigned long long* ptr,
                        const unsigned long long* expected_value,
                        const unsigned long long* new_value)
{
    bool success = false;

    asm volatile("ldx   [%1], %%o4;"
                 "ldx   [%2], %%o5;"
                 "casx  [%3], %%o4, %%o5;"
                 "cmp   %%o4, %%o5;"
                 "mov   %%g0, %0;"
                 "move  %%xcc, 1, %0"   // predicated move... should do this
                                        // for bool_cas too
                 : "=r"(success)
                 : "r"(expected_value), "r"(new_value), "r"(ptr)
                 : "o4", "o5", "memory");
    return success;
}

// When casX is dealing with packed structs, it is convenient to pass each word
// directly
static inline bool volatile casX(volatile unsigned long long* ptr,
                                 unsigned long expected_high,
                                 unsigned long expected_low,
                                 unsigned long new_high,
                                 unsigned long new_low)
{
    bool success = false;
    asm volatile("sllx %1, 32, %%o4;"
                 "or   %%o4, %2, %%o4;"
                 "sllx %3, 32, %%o5;"
                 "or   %%o5, %4, %%o5;"
                 "casx [%5], %%o4, %%o5;"
                 "cmp  %%o4, %%o5;"
                 "be,pt %%xcc,1f;"
                 "mov  1, %0;"
                 "mov  %%g0, %0;"
                 "1:"
                 : "=r"(success)
                 : "r"(expected_high), "r"(expected_low), "r"(new_high),
                   "r"(new_low), "r"(ptr)
                 : "o4", "o5", "memory");
    return success;
}

static inline void
mvx(const volatile unsigned long long* from, volatile unsigned long long* to)
{
    asm volatile("ldx  [%0], %%o4;"
                 "stx  %%o4, [%1];"
                 :
                 : "r"(from), "r"(to)
                 : "o4", "memory");
}

static inline void nop()
{
    asm volatile("nop");
}

#else
#error Your CPU/compiler combination is not supported
#endif

static inline bool
bool_cas(volatile unsigned long* ptr, unsigned long old, unsigned long _new)
{
    return cas(ptr, old, _new) == old;
}

static inline unsigned long fai(volatile unsigned long* ptr)
{
    unsigned long found = *ptr;
    unsigned long expected;
    do {
        expected = found;
    } while ((found = cas(ptr, expected, expected + 1)) != expected);
    return found;
}

static inline unsigned long faa(volatile unsigned long* ptr, int amnt)
{
  unsigned long found = *ptr;
  unsigned long expected;
  do {
    expected = found;
  } while ((found = cas(ptr, expected, expected + amnt)) != expected);
  return found;
}


// exponential backoff
static inline void backoff(int *b)
{
    for (int i = *b; i; i--)
        nop();

    if (*b < 4096)
        *b <<= 1;
}

// issue 64 nops to provide a little busy waiting
static inline void spin64()
{
    for (int i = 0; i < 64; i++)
        nop();
}

// issue 128 nops to provide a little busy waiting
static inline void spin128()
{
    for (int i = 0; i < 128; i++)
        nop();
}

////////////////////////////////////////
// tatas lock

typedef volatile unsigned long tatas_lock_t;

static inline void tatas_acquire_slowpath(tatas_lock_t* L)
{
    int b = 64;

    do
    {
        backoff(&b);
    }
    while (tas(L));
}

static inline void tatas_acquire(tatas_lock_t* L)
{
    if (tas(L))
        tatas_acquire_slowpath(L);
}

static inline void tatas_release(tatas_lock_t* L)
{
    *L = 0;
}

////////////////////////////////////////
/// tatas sin backoff

static inline void tatas_acquire_slowpath_without_backoff(tatas_lock_t* L)
{
    int b = 64;

    do
    {
        
    }
    while (tas(L));
}

static inline void tatas_acquire_without_backoff(tatas_lock_t* L)
{
    if (tas(L))
        tatas_acquire_slowpath_without_backoff(L);
}

static inline void tatas_release(tatas_lock_t* L)
{
    *L = 0;
}


// ticket lock

extern "C"
{
    typedef struct
    {
        volatile unsigned long next_ticket;
        volatile unsigned long now_serving;
    } ticket_lock_t;
}

static inline void ticket_acquire(ticket_lock_t* L)
{
    unsigned long my_ticket = fai(&L->next_ticket);
    while (L->now_serving != my_ticket);
}

static inline void ticket_release(ticket_lock_t* L)
{
    L->now_serving += 1;
}

////////////////////////////////////////
// MCS lock

extern "C"
{
    typedef volatile struct _mcs_qnode_t
    {
        bool flag;
        volatile struct _mcs_qnode_t* next;
    } mcs_qnode_t;
}

static inline void mcs_acquire(mcs_qnode_t** L, mcs_qnode_t* I)
{
    I->next = 0;
    mcs_qnode_t* pred =
        (mcs_qnode_t*)swap((volatile unsigned long*)L, (unsigned long)I);

    if (pred != 0) {
        I->flag = true;
        pred->next = I;
        while (I->flag) { } // spin
    }
}

static inline void mcs_release(mcs_qnode_t** L, mcs_qnode_t* I)
{
    if (I->next == 0) {
        if (bool_cas((volatile unsigned long*)L, (unsigned long)I, 0))
            return;
        while (I->next == 0) { } // spin
    }
    I->next->flag = false;
}

#endif // ATOMIC_OPS_H__
