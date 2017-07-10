#!/bin/bash

WKFILE=$1
for wk in `cat $WKFILE`
do
    awk -v wk=$wk '$2==wk{print $3,$4}' flu_res_.txt > res_"$wk"
done
