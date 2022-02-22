#!/bin/bash


PROJECT_DIR=("code/counter" "code/interlacement" "code/order" "code/timestamps")
PROJECT_FILE=("counter.pro" "interlacement.pro" "order.pro" "timestamps.pro")
ARCHIVE=rendu.tar.gz


for i in "${!PROJECT_DIR[@]}"
do
    if [ ! -d "${PROJECT_DIR[i]}" ]
     then
        echo "Could not find  directory in ${PROJECT_DIR[i]}" >&2
       exit 1
    fi

    if [ ! -f "${PROJECT_DIR[i]}/${PROJECT_FILE[i]}" ]
    then
        echo "Could not find project file : ${PROJECT_FILE[i]} in $(realpath ${PROJECT_DIR[i]})" >&2
        exit 1
    fi
done

echo "The following files are archived in $ARCHIVE : "
tar --exclude='rendu.tar.gz' --exclude='*.o' --exclude='*.user' -czvf $ARCHIVE code
