/**
   Util.hpp

   Copyright (c) 2021 Yasuo Tabei

   This software is released under the MIT License.
   http://opensource.org/licenses/mit-license.php
*/

#pragma once

#include <stdint.h>
#include <limits.h>
#include <unordered_map>

#ifndef UINT_MAX
#define UINT_MAX 0xffffffff
#endif

#ifndef UINT64_MAX
#define UINT64_MAX 0xffffffffffffffff
#endif


const uint32_t kDummy32 = 0xffffffff;

const uint32_t kDummy = 0xffffffff;

//const uint64_t kDummy = 0xffffffffffffffff;

const uint64_t primes[] = {
  /* 0*/  8 + 3,
  /* 1*/  16 + 3,
  /* 2*/  32 + 5,
  /* 3*/  64 + 3,
  /* 4*/  128 + 3,
  /* 5*/  256 + 27,
  /* 6*/  512 + 9,
  /* 7*/  1024 + 9,
  /* 8*/  2048 + 5,
  /* 9*/  4096 + 3,
  /*10*/  8192 + 27,
  /*11*/  16384 + 43,
  /*12*/  32768 + 3,
  /*13*/  65536 + 45,
  /*14*/  131072 + 29,
  /*15*/  262144 + 3,
  /*16*/  524288 + 21,
  /*17*/  1048576 + 7,
  /*18*/  2097152 + 17,
  /*19*/  4194304 + 15,
  /*20*/  8388608 + 9,
  /*21*/  16777216 + 43,
  /*22*/  33554432 + 35,
  /*23*/  67108864 + 15,
  /*24*/  134217728 + 29,
  /*25*/  268435456 + 3,
  /*26*/  536870912 + 11,
  /*27*/  1073741824 + 85,
  /*28*/  17129402307ULL,
  /*29*/  2110461401779ULL,
  /*30*/  24282437925089ULL,
  /*31*/  0
};

typedef uint32_t var_t;

typedef uint32_t len_t;

#define bitlen 32

//#define bitlen 64

//const var_t kDummy = (18446744073709551615ULL);

//const uint32_t kDummy = 0xffffffff;

//const var_t kAlphabetSize = 256;


