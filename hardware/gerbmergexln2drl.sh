#!/bin/bash
#
# 

BASENAME=$1
OUTPUT=$1.txt

echo "M48" > $OUTPUT
echo "INCH,TZ" >> $OUTPUT

cat $1.xln | grep ^T >> $OUTPUT

echo "%" >> $OUTPUT
echo "G90" >> $OUTPUT
echo "G05" >> $OUTPUT

cat $1.xln | sed -e 's/T\([0-9][0-9]\).*/T\1/' >> $OUTPUT

