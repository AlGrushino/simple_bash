#!/bin/bash

test="test.txt"
test2="test2.txt"
patterns="patterns.txt"
sample="hello"
s21="s21_res.txt"
orig="orig_res.txt"


flags=(
    "-i"
    "-v"
    "-c"
    "-l"
    "-n"
    "-h"
    "-s"
    "-o"
)

flags_ef=(
    "-e"
    "-f"
)


check_flags(){
    # все одиночные флаги кроме e и f
    for flag in ${flags[@]}; do

        ./s21_grep $sample $flag $test $test2 > $s21
        grep $sample $flag $test $test2 > $orig

        if cmp -s $orig $s21;
        then
            echo "test short: files are same, flag:" $flag
        else
            echo "test short: files are differ, flag:" $flag
        fi

        rm -rf $orig
        rm -rf $s21
    done

    # флаг e
    ./s21_grep "-e" $sample $test $test2 > $s21
    grep "-e" $sample $test $test2 > $orig

    if cmp -s $orig $s21;
    then
        echo "test short: files are same, flag: -e"
    else
        echo "test short: files are differ, flag: -e"
    fi

    rm -rf $orig
    rm -rf $s21

    # флаг f
    ./s21_grep "-f" $patterns $test $test2 > $s21
    grep "-f" $patterns $test $test2 > $orig

    if cmp -s $orig $s21;
    then
        echo "test short: files are same, flag: -f"
    else
        echo "test short: files are differ, flag: -f"
    fi

    rm -rf $orig
    rm -rf $s21
}

check_flags


check_flags_together(){
    # все флаги попрано, кроме e и f

    for flag_one in ${flags[@]}; do
        for flag_two in ${flags[@]}; do
            if [ "$flag_one" != "$flag_two" ]; then
                ./s21_grep $flag_one $flag_two $sample $test $test2 > $s21
                grep $flag_one $flag_two $sample $test $test2 > $orig
                if cmp -s $orig $s21;
                then
                    echo "test short: files are same, flags:" $flag_one $flag_two
                else
                    echo "test short: files are differ, flags:" $flag_one $flag_two
                fi
            fi

            rm -rf $orig
            rm -rf $s21
        done
    done

    # флаг e со всеми флагами
    for flag in ${flags[@]}; do
        ./s21_grep "-e" $sample $flag $test $test2 > $s21
        grep "-e" $sample $flag $test $test2 > $orig

        if cmp -s $orig $s21;
        then
            echo "test short: files are same, flags: -e," $flag
        else
            echo "test short: files are differ, flags: -e" $flag
        fi

        rm -rf $orig
        rm -rf $s21
    done

    # флаг f со всеми флагами
    for flag in ${flags[@]}; do
        ./s21_grep "-f" $patterns $flag $test $test2 > $s21
        grep "-f" $patterns $flag $test $test2 > $orig

        if cmp -s $orig $s21;
        then
            echo "test short: files are same, flags: -f," $flag
        else
            echo "test short: files are differ, flags: -f" $flag
        fi

        rm -rf $orig
        rm -rf $s21
    done
}


check_flags_together


check_without_flags(){
    ./s21_grep  $sample $flag $test $test2 > $s21
    grep $sample $flag $test $test2 > $orig

    if cmp -s $orig $s21;
    then
        echo "test short: files are same, no flags"
    else
        echo "test short: files are differ, no flags"
    fi

    rm -rf $orig
    rm -rf $s21
}


check_without_flags