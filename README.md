# SFMEDM
Space-efficient feature maps for string alignment kernels

## Overview
SFMEDM is a feature maps for string alignment kernels. SFMEDM takes a set of strings and outputs feature vectors for those strings. Those outputs can be used for solving string classification and regression problems using linear SVMs. Please see our papers for more details. 

## Quick start
cd src  
make  
./sfmedm --input_file=../dat/dat.txt  --output_file=out.txt

## Commandline options
usage: ./sfmedm --input_file=string --output_file=string [options] ...  
options:  
  -i, --input_file     input file name (string)  
  -o, --output_file    output file name (string)  
  -m, --max_height     maxmum height of parse trees (unsigned long long [=18446744073709551615])  
  -a, --load_factor    load factor (double [=0.5])  
  -t, --tfidf          compute TF-IDF (bool [=0])  
  -h, --hashdim        hashdim (unsigned long long [=16])  
  -b, --beta           b (double [=1])  
  -?, --help           print this message  
