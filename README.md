# SFMEDM
Space-efficient feature maps for string alignment kernels

## Overview
SFMEDM is a feature maps for string alignment kernels. SFMEDM takes a set of strings and outputs feature vectors for those strings. Those outputs can be used for solving string classification and regression problems using linear SVMs. Please see our papers (Y.Tabei et al., ICDM'19) for more details. 

## Quick start
cd src  
make  
./sfmedm --input_file=../dat/dat.txt  --output_file=out.txt

## Input file format
    Each line of input file is a class label (integer) followed by a string as follows, 
    1 AAQFVAEHGDQVCPAKWTPGAETIVPSL
    1 AFQFTDKHGEVCPAGWKPGSDTIKPDVEKSKEYFSK
    1 AIHHTQVNANEVCPVDWQPGQKTITPSSKSARGFFET
    0 AFQYVETHGEVCPANWTPDSPTIKPSPAASKEYFQK
    0 SLQLTANYSVATPADWKEGEDVVVSNSIKTEDIPSKFPKG

Please see ./dat/dat.txt for an example.

## Output file format
    Each line of output file is a class label (integer) followed by a feature vector in the LIBSVM format where feature is represented as a pair of dimension and value as follows, 
    1 1:0.306572 2:0.176107 3:0.221853 4:-0.275284 5:0.0186437 6:-0.353061 7:-0.332309 8:0.12071 9:-0.241722 10:0.258013 11:0.0684519 12:-0.346864 13:-0.309075 14:-0.171676 15:0.150164 16:0.320079
    1 1:-0.0754622 2:0.345406 3:-0.353292 4:0.0135816 5:0.333295 6:-0.11796 7:0.280467 8:0.215264 9:0.330191 10:0.126387 11:0.263296 12:-0.235956 13:0.0213408 14:0.352909 15:0.195604 16:0.294515
    1 1:0.0436585 2:-0.350847 3:0.345786 4:0.0737002 5:0.339729 6:0.0978983 7:-0.219476 8:0.277182 9:-0.351346 10:-0.0394432 11:0.272613 12:-0.225127 13:0.323418 14:-0.142831 15:0.0170619 16:-0.353141
    0 1:-0.347702 2:-0.0640569 3:0.0686054 4:-0.346833 5:-0.265711 6:-0.233233 7:0.344078 8:-0.0813051 9:-0.347059 10:0.0674517 11:-0.189836 12:0.298266 13:-0.0890129 14:-0.342165 15:0.0791635 16:0.344577
    0 1:0.308909 2:0.171974 3:-0.113286 4:-0.334912 5:0.188422 6:-0.299161 7:-0.35237 8:-0.0289045 9:-0.15044 10:0.31995 11:0.153554 12:-0.318467 13:0.0312697 14:0.352168 15:0.201521 16:0.290498

The order of class labels and feature vectors in the output file is the same as that of class calbels and strings in the input file. 
The output file can be used for an input for LIBLINEAR (https://www.csie.ntu.edu.tw/~cjlin/liblinear/).
  
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
      
 ## License
For academic use case, MIT License is applied to the codes. For industrial use case, please contact yasuo.tabei@gmail.com.
For both academic and inductrial use cases, please refere to the following papers and URL for the code reposity.

Tabei, Y., Yamanishi, Y. & Pagh, R. Space-Efficient Feature Maps for String Alignment Kernels. Data Sci. Eng. 5, 168–179 (2020). https://doi.org/10.1007/s41019-020-00120-6

Tabei, Y., Yamanishi, Y. & Pagh, R. Space-Efficient Feature Maps for String Alignment Kernels. In Proceedings of the 19th IEEE International Conference on Data Mining (ICDM), 2019.

https://github.com/tb-yasu/SFMEDM


Copyright (c) 2021 Yasuo Tabei

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
