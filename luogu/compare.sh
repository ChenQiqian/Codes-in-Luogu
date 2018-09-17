#!/bin/sh

path="Documents/Codes/luogu/"
gen="gen"
name1="p4867"
name2="p4867_1"
input_file="in.txt"
output_file1="out1.txt"
output_file2="out2.txt"

cd 
cd $path
g++-7 $name1.cpp -o $name1 -std=c++11 -O2
g++-7 $name2.cpp -o $name2 -std=c++11 -O2
g++-7 $gen.cpp -o $gen  -std=c++11 -O2
echo "\\033[1;33mFinish building.\\033[0m"

cnt=0

while true
do
    ((cnt++))
    ./$gen > $input_file
    let time1=`gdate +%s%N`
    ./$name1 < $input_file > $output_file1
    let time2=`gdate +%s%N`
    ./$name2 < $input_file > $output_file2
    let time3=`gdate +%s%N`
    if diff $output_file1 $output_file2 -w > /dev/null 2>&1
    then
        echo "\\033[1;32mTask$cnt:Accepted. $name1:$(((time2-time1)/1000000))ms,$name2:$(((time3-time2)/1000000))ms\\033[0m"
        # if [ $((cnt%100)) -eq 0 ]
        # then
        #     echo $((cnt))
        # fi
    else 
        echo "\\033[1;31mTask$cnt:Wrong Answer.\\033[0m"
        break
    fi
    # sleep 0.03
done
