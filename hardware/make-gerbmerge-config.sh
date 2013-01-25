#!/bin/sh
cat template-main
for pcb in $*;do
  echo "[$pcb]"
  echo "Prefix=%(ProjDir)s/$pcb"
  cat template-proj
  echo
done
