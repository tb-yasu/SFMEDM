/**
   SFM.cpp

   Copyright (c) 2021 Yasuo Tabei

   This software is released under the MIT License.
   http://opensource.org/licenses/mit-license.php
*/

#include "SFMEDM.hpp"

using namespace::std;

double SFM::gettimeofday_sec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + (double)tv.tv_usec*1e-6;
}

void SFM::readfile(istream &is, vector<int> &labels, vector<vector<pair<var_t, float> > > &fvs) {
  size_t count = 0;
  string line;
  datdim_ = 0;
  while (getline(is, line)) {
    if (count++ % 10000 == 0) {
      cerr << count << endl;
    }
    labels.resize(labels.size() + 1);
    fvs.resize(fvs.size() + 1);
    int &label = labels[labels.size() - 1];
    vector<pair<var_t, float> > &fv = fvs[fvs.size() - 1];
    
    stringstream ss(line);
    ss >> label;
    
    char sep = 0;
    var_t fid;
    float val;
    while (ss >> fid >> sep >> val) {
      fv.push_back(make_pair(fid, val));
      if (datdim_ < fid)
	datdim_ = fid;
    }
    fv.swap(fv);
  }
  datdim_++;
}

void SFM::init_xor128(uint64_t s) {
  x_ = s = 1812433253UL*(s^(s>>30));
  y_ = s = 1812433253UL*(s^(s>>30)) + 1ULL;
  z_ = s = 1812433253UL*(s^(s>>30)) + 2ULL;
  w_ = s = 1812433253UL*(s^(s>>30)) + 3ULL;
} 
 
uint64_t SFM::xor128() {
  uint64_t t=(x_^(x_<<11));
  x_=y_; y_=z_; z_=w_;
  return ( w_=(w_^(w_>>19))^(t^(t>>8)) );
}

void SFM::initVectors() {
  static mt19937_64 gen(static_cast<unsigned long>(time(0)));

  avec_.resize(datdim_);  
  for (size_t i = 0; i < datdim_; ++i)  
    avec_[i] = gen();

  bvec_.resize(datdim_);
  for (size_t i = 0; i < datdim_; ++i) 
    bvec_[i] = gen();
}

uint32_t SFM::twistedtab32(uint32_t i, uint32_t j) {
  uint64_t tmp = avec_[i]*(uint64_t)j;
  return ((tmp + bvec_[i])>>32);
}

float SFM::compHashVal(uint32_t i, uint32_t j) {
  uint32_t val = twistedtab32(i, j);
  float   dval = (float)val/4294967296.f;
  
  return tan(M_PI * (dval - 0.5))*beta_;
}

void SFM::setParams(vector<vector<pair<var_t, float> > > &fvs) {
  datdim_ = 0;
  for (size_t i = 0; i < fvs.size(); ++i) {
    vector<pair<var_t, float> > &fv = fvs[i];
    for (size_t j = 0; j < fv.size(); ++j) {
      if (datdim_ < fv[j].first)
	datdim_ = fv[j].first;
    }
  }
}

void SFM::projectFVS(vector<vector<pair<var_t, float> > > &fvs, vector<int> &labels, ostream &os) {
  uint64_t hashdim2 = hashdim_ >> 1;
  
  initVectors();

  vector<float> pfv(hashdim_);
  float coef = sqrt(2.f/(float)hashdim_);
  for (size_t i = 0; i < fvs.size(); ++i) {
    if (i % 1000 == 0)
      cerr << i << endl;
    vector<pair<var_t, float> > &fv = fvs[i];

    vector<float> pfv(hashdim_);
    for (size_t k = 0; k < hashdim2; ++k) {
      double sum = 0.0;
      for (size_t j = 0; j < fv.size(); ++j) 
	sum += compHashVal(fv[j].first, k) * fv[j].second;

      pfv[k << 1]       = coef * sin(sum);
      pfv[(k << 1) + 1] = coef * cos(sum);
    }

    os << labels[i];
    for (size_t j = 0; j < pfv.size(); j++) {
      os << " " << (j+1) << ":" << pfv[j];
    }
    os << endl;

  }
}

void SFM::run(const char *outputname, vector<int> &labels, vector<vector<pair<var_t, float> > > &fvs, uint64_t hashdim, double beta) {
  hashdim_ = hashdim;
  beta_    = 1.0/beta;
  
  {
    ofstream ofs(outputname);
    if (!ofs) {
      cerr << "cannot open: " << outputname << endl;
      exit(1);
    }

    double stime = gettimeofday_sec();
    setParams(fvs);
    projectFVS(fvs, labels, ofs);
    double etime = gettimeofday_sec();
    
    cout << "CPU time for hashing vectors (sec): " << (etime - stime) << endl;
    cout << "random matrix size (bytes): " << sizeof(uint64_t) * datdim_ * 2 << endl;

    ofs.close();
  }
}
