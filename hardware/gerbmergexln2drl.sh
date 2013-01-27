#!/bin/bash
#
# Converts the xln drill produced by gerbmerge to a more acceptable .txt drill file as produced by Kicad. 
# Output is tested with Seeedstudio's Fusion Service and the online gerber viewer http://mayhewlabs.com/3dpcb
#
# Usage: gerbmergexln2drl.sh BASENAME
#
# Licensed under the CC-BY-SA 3.0 license
# Dirkjan Krijnders 2013 <dirkjan@krijnders.net>

BASENAME=$1
OUTPUT=$1.txt

# Write proper header
echo "M48" > $OUTPUT
echo "INCH,TZ" >> $OUTPUT

# Extract toollist and add to header
cat $1.xln | grep ^T >> $OUTPUT

# This G-code seems to be necessary as well
echo "%" >> $OUTPUT
echo "G90" >> $OUTPUT
echo "G05" >> $OUTPUT

# Add the drill file contents, filtering the tool commands to use just the number as the drill diameter is specified 
# in the header
cat $1.xln | sed -e 's/T\([0-9][0-9]\).*/T\1/' >> $OUTPUT

