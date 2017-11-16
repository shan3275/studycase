#!/bin/sh
while getopts "a:bc" arg #选项后面的冒号表示该选项需要参数
do
    case $arg in
        a)
            echo "a's arg:$OPTARG" #参数存在$optarg中
            ;;
        b)
            echo "b"
            ;;
        c)
            echo "c"
            ;;
        ?)  #当有不认识的选项的时候arg为?
            echo "argument: a:bc"
            echo "input like this: xxx.sh -a love"
            echo "input like this: xxx.sh -b"
            echo "input like this: xxx.sh -c"
            echo "input like this: xxx.sh -a pig -b -c"
            exit 1
        esac
done
