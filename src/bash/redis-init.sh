#!/bin/bash
#
# init redis schema data

# read all schema from file
readarray -t schemas < $1
for line in "${schemas[@]}"; do
    echo $line | redis-cli
done
