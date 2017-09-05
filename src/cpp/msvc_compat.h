#pragma once

// don't warn about multiple assignment operators
#pragma warning(disable:4522)

// replace the =default, =delete semantics
struct nocopy_volatile
{
private:
	nocopy_volatile(const nocopy_volatile&);
	nocopy_volatile& operator=(const nocopy_volatile&);
	nocopy_volatile& operator=(const nocopy_volatile&) volatile;

public:
	nocopy_volatile() {}
};

// use to emulate an enum class
#define ENUM_CLASS_BEGIN(x) struct x { enum vals {
#define ENUM_CLASS_END(x) \
	}; \
	x() {} \
	x(vals val) : m_val(val) {} \
	vals m_val; \
	}; \
	inline bool operator==(x lhs, x rhs) { return lhs.m_val == rhs.m_val; } \
	inline bool operator!=(x lhs, x rhs) { return !(lhs == rhs); }

#define noexcept throw()

// map the various msvc interlocked intrinsics to overloaded funcs

// x86 and x64 have no acquire or release semantics! Not much we can do...

#if !defined _M_IX86 && !defined _M_X64
#error No support for your architecture
#endif

#include <intrin.h>

// __sync_lock_test_and_set
__forceinline char __sync_lock_test_and_set(char volatile* addr, char val)
{ return _InterlockedExchange8(addr, val); }

__forceinline bool __sync_lock_test_and_set(bool volatile* addr, bool val)
{ return !!__sync_lock_test_and_set((char volatile *)addr, val); }

__forceinline short __sync_lock_test_and_set(short volatile* addr, short val)
{ return _InterlockedExchange16(addr, val); }

__forceinline long __sync_lock_test_and_set(long volatile* addr, long val)
{ return _InterlockedExchange(addr, val); }

#ifdef _M_X64
__forceinline int __sync_lock_test_and_set(int volatile* addr, int val)
{ return (int)_InterlockedExchange64((__int64 volatile *)addr, val); }

__forceinline __int64 __sync_lock_test_and_set(__int64 volatile* addr, __int64 val)
{ return _InterlockedExchange64(addr, val); }
#elif defined _M_IX86
__forceinline int __sync_lock_test_and_set(int volatile* addr, int val)
{ return _InterlockedExchange((long volatile*)addr, val); }

__forceinline __int64 __sync_lock_test_and_set(__int64 volatile* addr, __int64 val)
{ return InterlockedExchange64(addr, val); }
#endif

// __sync_lock_release
__forceinline char __sync_lock_release(char volatile* addr)
{ return _InterlockedExchange8(addr, 0); }

__forceinline bool __sync_lock_release(bool volatile* addr)
{ return !!__sync_lock_release((char volatile *)addr); }

__forceinline short __sync_lock_release(short volatile* addr)
{ return _InterlockedExchange16(addr, 0); }

__forceinline long __sync_lock_release(long volatile* addr)
{ return _InterlockedExchange(addr, 0); }

#ifdef _M_X64
__forceinline int __sync_lock_release(int volatile* addr)
{ return (int)_InterlockedExchange64((__int64 volatile *)addr, 0); }

__forceinline __int64 __sync_lock_release(__int64 volatile* addr)
{ return _InterlockedExchange64(addr, 0); }
#elif defined _M_IX86
__forceinline int __sync_lock_release(int volatile* addr)
{ return _InterlockedExchange((long volatile*)addr, 0); }

__forceinline __int64 __sync_lock_release(__int64 volatile* addr)
{ return InterlockedExchange64(addr, 0); }
#endif

// __sync_val_compare_and_swap
__forceinline char __sync_val_compare_and_swap(char volatile* addr, char old_val, char new_val)
{ return _InterlockedCompareExchange8(addr, new_val, old_val); }

__forceinline bool __sync_val_compare_and_swap(bool volatile* addr, bool old_val, bool new_val)
{ return !!__sync_val_compare_and_swap((char volatile *)addr, old_val, new_val); }

__forceinline short __sync_val_compare_and_swap(short volatile* addr, short old_val, short new_val)
{ return _InterlockedCompareExchange16(addr, new_val, old_val); }

__forceinline long __sync_val_compare_and_swap(long volatile* addr, long old_val, long new_val)
{ return _InterlockedCompareExchange(addr, new_val, old_val); }

__forceinline int __sync_val_compare_and_swap(int volatile* addr, int old_val, int new_val)
#ifdef _M_X64
{ return (int)_InterlockedCompareExchange64((__int64 volatile *)addr, new_val, old_val); }
#elif defined _M_IX86
{ return _InterlockedCompareExchange((long volatile*)addr, new_val, old_val); }
#endif

__forceinline __int64 __sync_val_compare_and_swap(__int64 volatile* addr, __int64 old_val, __int64 new_val)
{ return _InterlockedCompareExchange64(addr, new_val, old_val); }

// __sync_fetch_and_add
__forceinline char __sync_fetch_and_add(char volatile* addr, char val)
{ return _InterlockedExchangeAdd8(addr, val); }

__forceinline bool __sync_fetch_and_add(bool volatile* addr, bool val)
{ return !!__sync_fetch_and_add((char volatile *)addr, val); }

__forceinline short __sync_fetch_and_add(short volatile* addr, short val)
{ return _InterlockedExchangeAdd16(addr, val); }

__forceinline long __sync_fetch_and_add(long volatile* addr, long val)
{ return _InterlockedExchangeAdd(addr, val); }

#ifdef _M_X64
__forceinline int __sync_fetch_and_add(int volatile* addr, int val)
{ return (int)_InterlockedExchangeAdd64((__int64 volatile *)addr, val); }

__forceinline __int64 __sync_fetch_and_add(__int64 volatile* addr, __int64 val)
{ return _InterlockedExchangeAdd64(addr, val); }
#elif defined _M_IX86
__forceinline int __sync_fetch_and_add(int volatile* addr, int val)
{ return _InterlockedExchangeAdd((long volatile*)addr, val); }

__forceinline __int64 __sync_fetch_and_add(__int64 volatile* addr, __int64 val)
{ return InterlockedExchangeAdd((volatile unsigned __int64 *)addr, val); }
#endif

// __sync_fetch_and_sub
__forceinline char __sync_fetch_and_sub(char volatile* addr, char val)
{ return _InterlockedExchangeAdd8(addr, -val); }

__forceinline bool __sync_fetch_and_sub(bool volatile* addr, bool val)
{ return !!__sync_fetch_and_sub((char volatile *)addr, val); }

__forceinline short __sync_fetch_and_sub(short volatile* addr, short val)
{ return _InterlockedExchangeAdd16(addr, -val); }

__forceinline long __sync_fetch_and_sub(long volatile* addr, long val)
{ return _InterlockedExchangeAdd(addr, -val); }

#ifdef _M_X64
__forceinline __int64 __sync_fetch_and_sub(__int64 volatile* addr, __int64 val)
{ return _InterlockedExchangeAdd64(addr, -val); }
#elif defined _M_IX86
__forceinline __int64 __sync_fetch_and_sub(__int64 volatile* addr, __int64 val)
{ return InterlockedExchangeAdd((volatile unsigned __int64 *)addr, -val); }
#endif

// __sync_fetch_and_and
__forceinline char __sync_fetch_and_and(char volatile* addr, char val)
{ return _InterlockedAnd8(addr, val); }

__forceinline bool __sync_fetch_and_and(bool volatile* addr, bool val)
{ return !!__sync_fetch_and_and((char volatile *)addr, val); }

__forceinline short __sync_fetch_and_and(short volatile* addr, short val)
{ return _InterlockedAnd16(addr, val); }

__forceinline long __sync_fetch_and_and(long volatile* addr, long val)
{ return _InterlockedAnd(addr, val); }

#ifdef _M_X64
__forceinline __int64 __sync_fetch_and_and(__int64 volatile* addr, __int64 val)
{ return _InterlockedAnd64(addr, val); }
#elif defined _M_IX86
__forceinline __int64 __sync_fetch_and_and(__int64 volatile* addr, __int64 val)
{ return InterlockedAnd((volatile unsigned __int64 *)addr, val); }
#endif

// __sync_fetch_and_or
__forceinline char __sync_fetch_and_or(char volatile* addr, char val)
{ return _InterlockedOr8(addr, val); }

__forceinline bool __sync_fetch_and_or(bool volatile* addr, bool val)
{ return !!__sync_fetch_and_or((char volatile *)addr, val); }

__forceinline short __sync_fetch_and_or(short volatile* addr, short val)
{ return _InterlockedAnd16(addr, val); }

__forceinline long __sync_fetch_and_or(long volatile* addr, long val)
{ return _InterlockedOr(addr, val); }

#ifdef _M_X64
__forceinline __int64 __sync_fetch_and_or(__int64 volatile* addr, __int64 val)
{ return _InterlockedOr64(addr, val); }
#elif defined _M_IX86
__forceinline __int64 __sync_fetch_and_or(__int64 volatile* addr, __int64 val)
{ return InterlockedOr((volatile unsigned __int64 *)addr, val); }
#endif

// __sync_fetch_and_xor
__forceinline char __sync_fetch_and_xor(char volatile* addr, char val)
{ return _InterlockedXor8(addr, val); }

__forceinline bool __sync_fetch_and_xor(bool volatile* addr, bool val)
{ return !!__sync_fetch_and_xor((char volatile *)addr, val); }

__forceinline short __sync_fetch_and_xor(short volatile* addr, short val)
{ return _InterlockedXor16(addr, val); }

__forceinline long __sync_fetch_and_xor(long volatile* addr, long val)
{ return _InterlockedXor(addr, val); }

#ifdef _M_X64
__forceinline __int64 __sync_fetch_and_xor(__int64 volatile* addr, __int64 val)
{ return _InterlockedXor64(addr, val); }
#elif defined _M_IX86
__forceinline __int64 __sync_fetch_and_xor(__int64 volatile* addr, __int64 val)
{ return InterlockedXor((volatile unsigned __int64 *)addr, val); }
#endif

// __sync_add_and_fetch
__forceinline char __sync_add_and_fetch(char volatile* addr, char val)
{ return _InterlockedExchangeAdd8(addr, val) + val; }

__forceinline bool __sync_add_and_fetch(bool volatile* addr, bool val)
{ return !!__sync_add_and_fetch((char volatile *)addr, val); }

__forceinline short __sync_add_and_fetch(short volatile* addr, short val)
{ return _InterlockedExchangeAdd16(addr, val) + val; }

__forceinline long __sync_add_and_fetch(long volatile* addr, long val)
{ return _InterlockedExchangeAdd(addr, val) + val; }

#ifdef _M_X64
__forceinline __int64 __sync_add_and_fetch(__int64 volatile* addr, __int64 val)
{ return _InterlockedExchangeAdd64(addr, val) + val; }
#elif defined _M_IX86
__forceinline __int64 __sync_add_and_fetch(__int64 volatile* addr, __int64 val)
{ return InterlockedExchangeAdd((volatile unsigned __int64 *)addr, val) + val; }
#endif

// __sync_sub_and_fetch
__forceinline char __sync_sub_and_fetch(char volatile* addr, char val)
{ return _InterlockedExchangeAdd8(addr, -val) - val; }

__forceinline bool __sync_sub_and_fetch(bool volatile* addr, bool val)
{ return !!__sync_sub_and_fetch((char volatile *)addr, val); }

__forceinline short __sync_sub_and_fetch(short volatile* addr, short val)
{ return _InterlockedExchangeAdd16(addr, -val) - val; }

__forceinline long __sync_sub_and_fetch(long volatile* addr, long val)
{ return _InterlockedExchangeAdd(addr, -val) - val; }

#ifdef _M_X64
__forceinline __int64 __sync_sub_and_fetch(__int64 volatile* addr, __int64 val)
{ return _InterlockedExchangeAdd64(addr, -val) - val; }
#elif defined _M_IX86
__forceinline __int64 __sync_sub_and_fetch(__int64 volatile* addr, __int64 val)
{ return InterlockedExchangeAdd((volatile unsigned __int64 *)addr, -val) - val; }
#endif

// __sync_and_and_fetch
__forceinline char __sync_and_and_fetch(char volatile* addr, char val)
{ return _InterlockedAnd8(addr, val) & val; }

__forceinline bool __sync_and_and_fetch(bool volatile* addr, bool val)
{ return !!__sync_and_and_fetch((char volatile *)addr, val) & val; }

__forceinline short __sync_and_and_fetch(short volatile* addr, short val)
{ return _InterlockedAnd16(addr, val) & val; }

__forceinline long __sync_and_and_fetch(long volatile* addr, long val)
{ return _InterlockedAnd(addr, val) & val; }

#ifdef _M_X64
__forceinline __int64 __sync_and_and_fetch(__int64 volatile* addr, __int64 val)
{ return _InterlockedAnd64(addr, val) & val; }
#elif defined _M_IX86
__forceinline __int64 __sync_and_and_fetch(__int64 volatile* addr, __int64 val)
{ return InterlockedAnd((volatile unsigned __int64 *)addr, val) & val; }
#endif

// __sync_or_and_fetch
__forceinline char __sync_or_and_fetch(char volatile* addr, char val)
{ return _InterlockedOr8(addr, val) | val; }

__forceinline bool __sync_or_and_fetch(bool volatile* addr, bool val)
{ return !!__sync_or_and_fetch((char volatile *)addr, val) | val; }

__forceinline short __sync_or_and_fetch(short volatile* addr, short val)
{ return _InterlockedAnd16(addr, val) | val; }

__forceinline long __sync_or_and_fetch(long volatile* addr, long val)
{ return _InterlockedOr(addr, val) | val; }

#ifdef _M_X64
__forceinline __int64 __sync_or_and_fetch(__int64 volatile* addr, __int64 val)
{ return _InterlockedOr64(addr, val) | val; }
#elif defined _M_IX86
__forceinline __int64 __sync_or_and_fetch(__int64 volatile* addr, __int64 val)
{ return InterlockedOr((volatile unsigned __int64 *)addr, val) | val; }
#endif

// __sync_xor_and_fetch
__forceinline char __sync_xor_and_fetch(char volatile* addr, char val)
{ return _InterlockedXor8(addr, val) ^ val; }

__forceinline bool __sync_xor_and_fetch(bool volatile* addr, bool val)
{ return !!__sync_xor_and_fetch((char volatile *)addr, val) ^ val; }

__forceinline short __sync_xor_and_fetch(short volatile* addr, short val)
{ return _InterlockedXor16(addr, val) ^ val; }

__forceinline long __sync_xor_and_fetch(long volatile* addr, long val)
{ return _InterlockedXor(addr, val) ^ val; }

#ifdef _M_X64
__forceinline __int64 __sync_xor_and_fetch(__int64 volatile* addr, __int64 val)
{ return _InterlockedXor64(addr, val) ^ val; }
#elif defined _M_IX86
__forceinline __int64 __sync_xor_and_fetch(__int64 volatile* addr, __int64 val)
{ return InterlockedXor((volatile unsigned __int64 *)addr, val) ^ val; }
#endif
