# SFMEDM
Space-efficient feature maps for string alignment kernels

## Overview
SFMEDM is a feature maps for string alignment kernels. SFMEDM takes a set of strings and outputs feature vectors for those strings. Those outputs can be used for solving string classification and regression problems using linear SVMs. Please see our papers for more details. 

## Demo
cd src

make

./sfmedm --input_file=../dat/dat.txt  --output_file=out.txt
