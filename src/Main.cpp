/**
   Main.cpp

   Copyright (c) 2021 Yasuo Tabei

   This software is released under the MIT License.
   http://opensource.org/licenses/mit-license.php
*/

#include <iostream>
#include <string>
#include <stdint.h>

#include "SFMEDM.hpp"
#include "cmdline.h"

using namespace std;

int main(int argc, char **argv) {
  cmdline::parser p;
  p.add<string>("input_file",  'i', "input file name",  true);
  p.add<string>("output_file", 'o', "output file name", true);
  p.add<uint64_t>("max_height", 'm', "maxmum height of parse trees", false, 0xffffffffffffffff);
  p.add<double>("load_factor", 'a', "load factor", false, 0.5);
  p.add<bool>("tfidf", 't', "compute TF-IDF", false, false);
  p.add<uint64_t>("hashdim", 'h', "hashdim", false, 16);
  p.add<double>("beta", 'b', "b", false, 1.0);
  p.parse_check(argc, argv);
  const string   input_file  = p.get<string>("input_file");
  const string   output_file = p.get<string>("output_file");
  const uint64_t max_height  = p.get<uint64_t>("max_height");
  const double   load_factor = p.get<double>("load_factor");
  const bool     tfidf       = p.get<bool>("tfidf");
  const uint64_t hashdim     = p.get<uint64_t>("hashdim");
  const double   beta        = p.get<double>("beta");
  
  SFMEDM sfmedm;
  sfmedm.run(input_file.c_str(), output_file.c_str(), max_height, load_factor, tfidf, hashdim, beta);
  
  return 0;
}
