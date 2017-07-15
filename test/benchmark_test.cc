#include "benchmark/benchmark.h"

#include <map>
#include <unordered_map>

#if defined(__GNUC__)
#define BENCHMARK_NOINLINE __attribute__((noinline))
#else
#define BENCHMARK_NOINLINE
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components

#include <atlstr.h>
#include <atlcoll.h>

using namespace std;
const int nTotalElements = 100'000;

#define BENCHMARK_INSERT_IMPL(COLLECTION_TYPE)                    \
  static void COLLECTION_TYPE##_Insert(benchmark::State& state) { \
    while (state.KeepRunning()) {                                 \
      CString sBase;                                              \
      for (int i = 0; i < nTotalElements; i++) {                  \
        sBase.Format(_T("Test String %d"), i);                    \
        COLLECTION_TYPE##HashTable.SetAt(i, sBase);               \
      }                                                           \
    }                                                             \
  }                                                               \
  BENCHMARK(COLLECTION_TYPE##_Insert);

#define BENCHMARK_LOOKUP_IMPL(COLLECTION_TYPE)                    \
  static void COLLECTION_TYPE##_Lookup(benchmark::State& state) { \
    while (state.KeepRunning()) {                                 \
      CString sValue;                                             \
      for (int i = 0; i < nTotalElements; i++)                    \
        COLLECTION_TYPE##HashTable.Lookup(i, sValue);             \
    }                                                             \
  }                                                               \
  BENCHMARK(COLLECTION_TYPE##_Lookup);

#define BENCHMARK_LOOKUP_IMPL_SIMPLE(COLLECTION_TYPE)             \
  static void COLLECTION_TYPE##_Lookup(benchmark::State& state) { \
    while (state.KeepRunning()) {                                 \
      CString sValue;                                             \
      for (int i = 0; i < nTotalElements; i++)                    \
        sValue = COLLECTION_TYPE##HashTable.Lookup(i);            \
    }                                                             \
  }                                                               \
  BENCHMARK(COLLECTION_TYPE##_Lookup);

#define BENCHMARK_STD_INSERT_IMPL(COLLECTION_TYPE)                \
  static void COLLECTION_TYPE##_Insert(benchmark::State& state) { \
    while (state.KeepRunning()) {                                 \
      CString sBase;                                              \
      for (int i = 0; i < nTotalElements; i++) {                  \
        sBase.Format(_T("Test String %d"), i);                    \
        COLLECTION_TYPE##HashTable[i] = sBase;                    \
      }                                                           \
    }                                                             \
  }                                                               \
  BENCHMARK(COLLECTION_TYPE##_Insert);

#define BENCHMARK_STD_LOOKUP_IMPL(COLLECTION_TYPE)                \
  static void COLLECTION_TYPE##_Lookup(benchmark::State& state) { \
    while (state.KeepRunning()) {                                 \
      CString sValue;                                             \
      COLLECTION_TYPE## ::iterator it;                            \
      for (int i = 0; i < nTotalElements; i++) {                  \
        it = COLLECTION_TYPE##HashTable.find(i);                  \
        CString sBase = it->second;                               \
      }                                                           \
    }                                                             \
  }                                                               \
  BENCHMARK(COLLECTION_TYPE##_Lookup);

#define BENCHMARK_IMPL(COLLECTION_TYPE)  \
  \
COLLECTION_TYPE<int, CString>            \
      COLLECTION_TYPE##HashTable;        \
  BENCHMARK_INSERT_IMPL(COLLECTION_TYPE) \
  BENCHMARK_LOOKUP_IMPL(COLLECTION_TYPE)

#define BENCHMARK_IMPL_SIMPLE(COLLECTION_TYPE) \
  \
COLLECTION_TYPE<int, CString>                  \
      COLLECTION_TYPE##HashTable;              \
  BENCHMARK_INSERT_IMPL(COLLECTION_TYPE)       \
  BENCHMARK_LOOKUP_IMPL_SIMPLE(COLLECTION_TYPE)

#define BENCHMARK_IMPL_MFC(COLLECTION_TYPE) \
  \
COLLECTION_TYPE<int, int, CString, LPCTSTR> \
      COLLECTION_TYPE##HashTable;           \
  BENCHMARK_INSERT_IMPL(COLLECTION_TYPE)    \
  BENCHMARK_LOOKUP_IMPL(COLLECTION_TYPE)

#define BENCHMARK_STD_IMPL(COLLECTION_TYPE)  \
  \
COLLECTION_TYPE COLLECTION_TYPE##HashTable;  \
  BENCHMARK_STD_INSERT_IMPL(COLLECTION_TYPE) \
  BENCHMARK_STD_LOOKUP_IMPL(COLLECTION_TYPE)

// === REGISTER TESTS =======================================================================

BENCHMARK_IMPL_MFC(CMap);
BENCHMARK_IMPL_SIMPLE(CSimpleMap);
BENCHMARK_IMPL(CAtlMap);
BENCHMARK_IMPL(CRBMap);

typedef std::map<int, CString> std_map;
typedef std::unordered_map<int, CString> std_unordered_map;
BENCHMARK_STD_IMPL(std_map);
BENCHMARK_STD_IMPL(std_unordered_map);

// === RUN TEST =============================================================================

BENCHMARK_MAIN()
