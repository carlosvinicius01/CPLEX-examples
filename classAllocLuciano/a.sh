#!/bin/sh

declare -a ar=("A" "B" "C")

for p in "${ar[@]}"
    do
        for q in $(eval echo {$1..$2})
            do
                echo -e ${p}_${q} >> LOG.txt
                out=$(./cla ${p}_${q} 2 >> LOG.txt)
                echo $out
            done
    done