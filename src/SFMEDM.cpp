/**
   SFMEDM.cpp

   Copyright (c) 2021 Yasuo Tabei

   This software is released under the MIT License.
   http://opensource.org/licenses/mit-license.php
*/


#include "SFMEDM.hpp"

using namespace std;

void SFMEDM::run(const char *inputname, const char *outputname, uint64_t max_height, double load_factor,bool tfidf, uint64_t hashdim, double beta) {

  vector<int> labels;
  vector<vector<pair<var_t, float> > > fvs;
  {
    ESP esp;
    esp.run(inputname, max_height, load_factor, tfidf, labels, fvs);
  }

  /*
  for (size_t i = 0; i < fvs.size(); ++i) {
    for (size_t j = 0; j < fvs[i].size(); ++j) {
      cout << fvs[i][j].first << ":" << fvs[i][j].second << " ";
    }

  }
  */

  {
    SFM sfm;
    sfm.run(outputname, labels, fvs, hashdim, beta);
  }
}
