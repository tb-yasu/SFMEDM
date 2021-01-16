/**
ESP.cpp

Copyright (c) 2021 Yasuo Tabei

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

#include "SFMEDM.hpp"

using namespace std;

double ESP::gettimeofday_sec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + (double)tv.tv_usec*1e-6;
}

var_t ESP::log2(var_t x) const{
  if (x == 0) return 0;
  x--;
  var_t bit_num = 0;
  while (x >> bit_num){
    ++bit_num;
  }
  return bit_num;
}

var_t ESP::logStar(var_t val) const{
  var_t newval = val;
  var_t iter = 0;
  while ((newval = log2(newval)) > 1)
    iter++;
  return iter;
}

void ESP::readFile(ifstream &ifs, vector<int> &labels, vector<vector<var_t> > &strs) {
  strs.clear();

  sigma_ = 0;
  string line;
  while (getline(ifs, line)) {
    labels.resize(labels.size() + 1);
    strs.resize(strs.size() + 1);
    int &label = labels[labels.size() - 1];
    vector<var_t> &str = strs[strs.size() - 1];

    stringstream ss(line);
    ss >> label;
    string dat;
    ss >> dat;

    for (size_t i = 0; i < dat.size(); ++i) {
      str.push_back(dat[i]);
      if ((uint32_t)dat[i] > sigma_)
	sigma_ = dat[i];
    }
    str.swap(str);
  }
  sigma_++;
}

void ESP::doLeftAlignedParsing(vector<var_t> &str, uint64_t &trackPos, uint64_t prev, uint64_t cpos) {
  uint64_t diff = 0;
  if (cpos - prev > 2 && (cpos - prev) % 2 == 1)
    diff = 3;
  uint64_t startPos = prev;
  uint64_t endPos   = cpos - diff;

  for (uint64_t i = startPos; i < endPos; i += 2) {
    uint64_t left  = str[i]; 
    uint64_t right = str[i + 1];
    if (hash_.getCode(left, right) == false)
      hash_.addRule(left, right);
    str[trackPos++] = hash_.replace(left, right);
  }
  if (diff == 0)
    return;

  uint64_t left  = str[endPos + 1];
  uint64_t right = str[endPos + 2];
  if (hash_.getCode(left, right) == false)
    hash_.addRule(left, right);
  uint64_t code2  = hash_.replace(left, right);
  left  = str[endPos];
  if (hash_.getCode(left, code2) == false)
    hash_.addRule(left, code2);
  uint64_t code1 = hash_.replace(left, code2);
  str[trackPos++] = code1;
}

uint64_t ESP::computeLeastSignificantBit(var_t ch1, var_t ch2) {
  for (size_t i = 0; i < bitlen; ++i) {
    var_t bit1 = (ch1 >> i) & 1ULL;
    var_t bit2 = (ch2 >> i) & 1ULL;
    if (bit1 != bit2)
      return i;
  }
  return bitlen;
}

uint64_t ESP::getBit(var_t ch, uint64_t p) {
  return (ch >> p) & 1ULL;
}

void ESP::replace(vector<var_t> &tmpstr, var_t ch) {
  for (size_t i = 0; i < tmpstr.size(); ++i) {
    if (tmpstr[i] == ch) {
      for (size_t c = 0; c < 3; ++c) {
	if ((i == 0 || tmpstr[i-1] != c) && (i + 1 == tmpstr.size() || tmpstr[i + 1] != c))
	  tmpstr[i] = c;
      }
    }
  }
}

void checkNonRun(vector<var_t> &str) {
  for (size_t i = 1; i < str.size(); ++i) {
    if (str[i-1] == str[i]) {
      cerr << str[i-1] << " " << str[i] << endl;
      cerr << "error run" << endl;
      exit(1);
    }
  }
}

void ESP::doAlphabetReduction(vector<var_t> &str, uint64_t &trackPos, uint64_t prev, uint64_t cpos) {
  uint64_t startPos = prev;
  uint64_t endPos   = cpos;

  //  cerr << "startPos endPos:" << startPos << " " << endPos << " " << str.size() << endl;
  //  for (size_t i = 0; i < str.size(); ++i)
  //    cerr << str[i] << " ";
  //  cerr << endl;
  
  vector<var_t> tmpstr;
  copy(str.begin() + startPos, str.begin() + endPos, back_inserter(tmpstr));
  //  for (size_t i = 0; i < tmpstr.size(); ++i) {
  //    cerr << tmpstr[i] << " ";
  //  }
  //  cerr << endl;
  
  tmpstr[0] = 0;
  while (true) {
    //   checkNonRun(tmpstr);
    vector<var_t> tmpstr2(tmpstr.size());
    //    tmpstr2[0] = kDummy32;
    uint64_t alphaSize = 0;
    for (size_t i = 1; i < tmpstr.size(); ++i) {
      uint64_t l = computeLeastSignificantBit(tmpstr[i-1], tmpstr[i]);
      uint64_t newch = (l << 1) + getBit(tmpstr[i], l);
      tmpstr2[i] = newch;
      if (newch > alphaSize)
	alphaSize = newch;
    }

    copy(tmpstr2.begin(), tmpstr2.end(), tmpstr.begin());
    if (alphaSize < 6)
      break;
  }
  //  cerr << endl;
  replace(tmpstr, 3);
  replace(tmpstr, 4);
  replace(tmpstr, 5);

  size_t i = startPos;
  while(i < endPos) {
    if ((i + 4 != endPos) && ((i + 3 == endPos) || ((i + 2 - startPos < tmpstr.size() && tmpstr[i - startPos] < tmpstr[i + 1 - startPos] && tmpstr[i + 1 - startPos] > tmpstr[i + 2 - startPos]) || (i + 2 - startPos < tmpstr.size() && tmpstr[i - startPos] > tmpstr[i + 1 - startPos] && tmpstr[i + 1 - startPos] < tmpstr[i + 2 - startPos])))) {
      var_t left  = str[i + 1];
      var_t right = str[i + 2];
      if (hash_.getCode(left, right) == false)
	hash_.addRule(left, right);
      var_t code2  = hash_.replace(left, right);
      left = str[i];
      if (hash_.getCode(left, code2) == false)
	hash_.addRule(left, code2);
      var_t code1 = hash_.replace(left, code2);
      str[trackPos++] = code1;
      i += 3;
    }
    else {
      var_t left  = str[i];
      var_t right = str[i + 1];
      if (hash_.getCode(left, right) == false)
	hash_.addRule(left, right);
      var_t code = hash_.replace(left, right);
      str[trackPos++] = code;
      i += 2;
    }
  }
}

bool ESP::buildGrammar(vector<vector<var_t> > &strs) {
  heights_.resize(strs.size());
  bool flag = true;
  for (size_t i = 0; i < strs.size(); ++i) {
    vector<var_t> &str = strs[i];
    if (str.size() <= 1) {
      continue;
    }
    heights_[i] += 1;    
    /*
    cerr << "i: " << i << endl;
    for (size_t j = 0; j < str.size(); ++j)
      cerr << str[j] << " ";
    cerr << endl;
    */
    
    uint64_t trackPos = 0;
    var_t prevChr = str[0];
    uint64_t prevPos = 0;
    uint64_t countCont = 1, countDisCount = 0;
    for (size_t j = 1; j < str.size() - 1; ++j) {
      if (prevChr == str[j]) {
	if (countDisCount >= 5) {
	  //	  cerr << "doAlphabetReduction: " << prevPos << " " << j - 2 << " countDisCount: " << countDisCount << " str.size(): "  << str.size() << endl;
	  doAlphabetReduction(str, trackPos, prevPos, j - 1);
	  prevPos = j - 1;
	}
	else if (countDisCount >= 3) {
	  //	  cerr << "doLeftAlignedParsing1: " << prevPos << " " << j - 2 << " countDisCount: " << countDisCount << " str.size(): " << str.size() << endl;
	  doLeftAlignedParsing(str, trackPos, prevPos, j - 1);
	  prevPos = j - 1;
	}
	countCont++;
	countDisCount = 0;
      }
      else {
	if (countCont >= 2) {
	  //	  cerr << "doLeftAlignedParsing2: " << prevPos << " " << j - 1 << " countCont: " << countCont << " str.size(): " << str.size() << endl;
	  doLeftAlignedParsing(str, trackPos, prevPos, j);
	  prevPos = j;
	}
	countDisCount++;
	countCont = 1;
      }
      prevChr = str[j];
    }
    
    if (countDisCount >= 5 && str.size() > 2) {
      if (str[str.size() - 2] != str[str.size() - 1]) 
      //      cerr << "doAlphabetReduction2: " << prevPos << " " << str.size() << endl;
	doAlphabetReduction(str, trackPos, prevPos, str.size());
      else {
	doAlphabetReduction(str, trackPos, prevPos, str.size() - 2);
	doLeftAlignedParsing(str, trackPos, str.size() - 2, str.size());
      }
    }
    else {
      //      cerr << "doLeftAlignedParsing3: " << prevPos << " " << str.size() << endl;
      doLeftAlignedParsing(str, trackPos, prevPos, str.size());
    }

    if (trackPos != 0)
      str.resize(trackPos);    
    
    if (str.size() != 1)
      flag = false;


    /*
    //    cerr << "end len: " << str.size() << endl;
    for (size_t k = 0; k < str.size(); ++k)
      cout << str[k] << " ";
    cout << endl;
    */
  }
  return flag;
}

void ESP::decode(vector<vector<var_t> > &strs, vector<int> &labels, ostream &os) {
  vector<pair<uint64_t, uint64_t> > dictionary;
  hash_.setDictionary(dictionary, sigma_);
  
  for (size_t i = 0; i < strs.size(); ++i) {
    vector<var_t> &str = strs[i];
    int label = labels[i];

    os << label << " ";
    
    var_t code = str[0];
    stack<var_t> stk;
    stk.push(code);
    while (!stk.empty()) {
      code = stk.top(); stk.pop();

      if (code < sigma_) {
	os << (char)code;
	continue;
      }

      uint64_t left  = dictionary[code - sigma_].first;
      uint64_t right = dictionary[code - sigma_].second;
      stk.push(right);
      stk.push(left);
    }
    os << endl;
  }
}

void ESP::computeFeatureVectors(vector<vector<var_t> > &strs, vector<vector<pair<var_t, float> > > &fvs) {
  
  vector<pair<uint64_t, uint64_t> > dictionary;
  hash_.setDictionary(dictionary, sigma_);
  
  for (size_t i = 0; i < strs.size(); ++i) {
    vector<var_t> &str = strs[i];

    map<var_t, uint32_t> fc;
    for (size_t j = 0; j < str.size(); ++j){
      var_t code = str[j];
      stack<pair<var_t, uint64_t> >  stk;
      uint64_t height = heights_[j];
      stk.push(make_pair(code, height));
      while (!stk.empty()) {
	code = stk.top().first; height = stk.top().second; stk.pop();
	
	if (fc.find(code) == fc.end()) {
	  fc[code] = 1;
	  //	  fc[code] = pow(2, height);
	}
	else {
	  fc[code] += 1;
	  //	  fc[code] += pow(2, height);
	}
	
	if (code < sigma_) 
	  continue;
	
	uint64_t left  = dictionary[code - sigma_].first;
	uint64_t right = dictionary[code - sigma_].second;
	stk.push(make_pair(right, height - 1));
	stk.push(make_pair(left, height - 1));
      }
    }

    fvs.resize(fvs.size() + 1);
    vector<pair<uint32_t, float> > &fv = fvs[fvs.size() - 1];
    for (map<var_t, uint32_t>::iterator it = fc.begin(); it != fc.end(); ++it)
      fv.push_back(make_pair(it->first, it->second));
    sort(fv.begin(), fv.end(), Cmp());
    fv.swap(fv);
  }
}

void ESP::convertFeaturevectors2Tfidf(vector<vector<pair<var_t, float> > > &fvs) {
  vector<float> idf;
  for (size_t i = 0; i < fvs.size(); ++i) {
    vector<pair<var_t, float> > &fv = fvs[i];
    for (size_t j = 0; j < fv.size(); ++j) {
      if (idf.size() <= fv[j].first)
	idf.resize(fv[j].first + 1);
      idf[fv[j].first] += 1;
    }
  }

  float num = fvs.size();
  for (size_t i = 0; i < idf.size(); ++i) 
    idf[i] = log((num+1.f)/(idf[i]+1.f));

  for (size_t i = 0; i < fvs.size(); ++i) {
    vector<pair<var_t, float> > &fv = fvs[i];
    float totalCount = 0;
    for (size_t j = 0; j < fv.size(); ++j) 
      totalCount += fv[j].second;
    for (size_t j = 0; j < fv.size(); ++j) {
      fv[j].second = (fv[j].second/totalCount)*idf[fv[j].first] ;
      //      fv[j].second = fv[j].second*idf[fv[j].first] ;
    }
  }
}

void ESP::normalize(vector<vector<pair<var_t, float> > > &fvs) {
  for (size_t i = 0; i < fvs.size(); ++i) {
    vector<pair<var_t, float> > &fv = fvs[i];
    float den = 0.f;
    for (size_t j = 0; j < fv.size(); ++j) {
      den += fv[j].second * fv[j].second;
    }
    den = sqrt(den);
    for (size_t j = 0; j < fv.size(); ++j) 
      fv[j].second /= den;
  }
}

void ESP::outputvector(vector<vector<pair<var_t, float> > > &fvs, vector<int> &labels, ostream &os) {
  for (size_t i = 0; i < fvs.size(); ++i) {
    vector<pair<var_t, float> > &fv = fvs[i];
    int label = labels[i];
    os << label;
    for (size_t j = 0; j < fv.size(); ++j)
      os << " " << fv[j].first + 1 << ":" << fv[j].second;
    os << endl;
  }
}

void ESP::runESP(vector<vector<var_t> > &strs) {
  for (size_t h = 0; h < max_height_; ++h) {
    bool flag = buildGrammar(strs);
    if (flag)
      break;
  }
}

void ESP::run(const char *inputname, uint64_t max_height, double load_factor, bool tfidf, vector<int> &labels, vector<vector<pair<var_t, float> > > &fvs) {
  max_height_ = max_height;

  vector<vector<uint32_t> > strs;
  cerr << "readfile: " << inputname << endl;
  {
    ifstream ifs(inputname);
    if (!ifs) {
      cerr << "cannot open: " << inputname << endl;
      exit(1);
    }
    readFile(ifs, labels, strs);
    ifs.close();
  }

  cout << "Input sequence size (bytes): " << getMemSeq(strs) << endl;
  
  double stime = gettimeofday_sec();
  
  hash_.init(load_factor, sigma_);
  
  cerr << "start ESP" << endl;
  runESP(strs);

  computeFeatureVectors(strs, fvs);

  if (tfidf) {
    convertFeaturevectors2Tfidf(fvs);
  }
  
  double etime = gettimeofday_sec();
  cout << "execution time for ESP (sec): " << (etime - stime) << endl;

  /*  
  {
    ofstream ofs(outputname);
    if (!ofs) {
      cerr << "cannot open: " << outputname << endl;
      exit(1);
    }
    //    decode(strs, labels, cout);
    
    outputvector(fvs, labels, ofs);

    ofs.close();
  }
  */

  cout << "Hash size (bytes): " << getHashMem() << endl;
  cout << "Sequence size (bytes): " << getMemSeq(strs) << endl;
  cout << "Vector size (bytes): " << getDatMem(fvs) << endl;
  cout << "Numrules: " << hash_.dictNumRules() << endl;
}
