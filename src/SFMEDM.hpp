/**
   SFMEDM.hpp

   Copyright (c) 2021 Yasuo Tabei

   This software is released under the MIT License.
   http://opensource.org/licenses/mit-license.php
*/

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include <stdint.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <sys/time.h>
#include <random>

#include "Util.hpp"
#include "Hash.hpp"


class Cmp {
public:
  bool operator()(const std::pair<var_t, float> &left, const std::pair<var_t, float> &right) const {
    return (left.first < right.first);
  }
};

class ESP {
private:
  double gettimeofday_sec();
  var_t log2(var_t x) const;
  var_t logStar(var_t val) const;
  void readFile(std::ifstream &ifs, std::vector<int> &labels, std::vector<std::vector<var_t> > &strs);
  void runESP(std::vector<std::vector<var_t> > &strs);
  bool buildGrammar(std::vector<std::vector<var_t> > &strs);
  void doLeftAlignedParsing(std::vector<var_t> &str, uint64_t &trackPos, uint64_t prev, uint64_t cpos);
  void doAlphabetReduction(std::vector<var_t> &str, uint64_t &trackPos, uint64_t prev, uint64_t cpos);
  uint64_t computeLeastSignificantBit(var_t ch1, var_t ch2);
  uint64_t getBit(var_t ch, uint64_t p);
  void replace(std::vector<var_t> &tmpstr, var_t ch);
  void decode(std::vector<std::vector<var_t> > &strs, std::vector<int> &labels, std::ostream &os);
  void computeFeatureVectors(std::vector<std::vector<var_t> > &strs, std::vector<std::vector<std::pair<var_t, float> > > &fvs);
  void convertFeaturevectors2Tfidf(std::vector<std::vector<std::pair<var_t, float> > > &fvs);
  void normalize(std::vector<std::vector<std::pair<var_t, float> > > &fvs);
  void outputvector(std::vector<std::vector<std::pair<var_t, float> > > &fvs, std::vector<int> &labels, std::ostream &os);
public:
  void run(const char *inputname, uint64_t max_height, double load_factor, bool tfidf, std::vector<int> &labels, std::vector<std::vector<std::pair<var_t, float> > > &fvs);

  uint64_t getHashMem() {
    return hash_.spaceInBytes();
  }
  
  uint64_t getMemSeq(std::vector<std::vector<uint32_t> > &strs) {
    uint64_t bytes = 0;
    for (size_t i = 0; i < strs.size(); ++i) 
      bytes += strs[i].size() * sizeof(uint32_t);
    return bytes;
  }
  
  uint64_t getDatMem(std::vector<std::vector<std::pair<var_t, float> > > &fvs) {
    uint64_t bytes = 0;
    for (size_t i = 0; i < fvs.size(); ++i) 
      bytes += fvs[i].size() * (sizeof(var_t) + sizeof(float));
    return bytes;
  }
  
private:
  uint32_t sigma_;
  HASH hash_;
  uint64_t max_height_;

  std::vector<uint64_t> heights_;
};

class SFM {
private:
  double gettimeofday_sec();
  void readfile(std::istream &is, std::vector<int> &labels, std::vector<std::vector<std::pair<var_t, float> > > &fvs);
  void setParams(std::vector<std::vector<std::pair<var_t, float> > > &fvs);
  void projectFVS(std::vector<std::vector<std::pair<var_t, float> > > &fvs, std::vector<int> &labels, std::ostream &os);
  void initVectors();
  uint32_t twistedtab32(uint32_t i, uint32_t j);
  float compHashVal(uint32_t i, uint32_t j);
  void init_xor128(uint64_t s);
  uint64_t xor128();
public:
  void run(const char *outputname, std::vector<int> &labels, std::vector<std::vector<std::pair<var_t, float> > > &fvs, uint64_t hashdim, double beta);
private:
  uint64_t hashdim_;
  uint64_t datdim_;
  float    beta_;

  std::vector<uint64_t> avec_;
  std::vector<uint64_t> bvec_;

  uint64_t x_, y_, z_, w_;
};

class SFMEDM {
public:
  void run(const char *inputname, const char *outputname, uint64_t max_height, double load_factor, bool tfidf, uint64_t hashdim, double beta);
};
