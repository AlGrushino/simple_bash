#!/bin/bash

patterns="long_patterns.txt"
sample="a"
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


check_long_line() {
    # флаги e и f отдельно
    # флаг e
    
    ./s21_grep -e $sample "test_long.txt" > $s21
    grep -e $sample "test_long.txt" > $orig

    if cmp -s $orig $s21;
    then
        echo "test_long: files are same, flag: -e"
    else
        echo "test_long: files are differ, flag: -e"
    fi

    rm -rf $orig
    rm -rf $s21

    # флаг f

    ./s21_grep -f $patterns "test_long.txt" > $s21
    grep -f $patterns "test_long.txt" > $orig

    if cmp -s $orig $s21;
    then
        echo "test_long: files are same, flag: -f"
    else
        echo "test_long: files are differ, flag: -f"
    fi

    rm -rf $orig
    rm -rf $s21    
}


check_long_line_together_e(){
    # флаг e

    for flag in ${flags[@]}; do

        ./s21_grep -e "a" $flag "test_long.txt" > $s21
        grep -e "a" $flag "test_long.txt" > $orig

        if cmp -s $orig $s21;
        then
            echo "test_long: files are same, flags: -e" $flag
        else
            echo "test_long: files are differ, flags: -e" $flag
        fi

        rm -rf $orig
        rm -rf $s21
    done 
}


check_long_line_together_f(){
    # флаг f

    for flag in ${flags[@]}; do

        ./s21_grep -f $patterns $flag "test_long.txt" > $s21
        grep -f $patterns $flag "test_long.txt" > $orig

        if cmp -s $orig $s21;
        then
            echo "test_long: files are same, flags: -f" $flag
        else
            echo "test_long: files are differ, flags: -f" $flag
        fi

        rm -rf $orig
        rm -rf $s21
    done 
}


check_two_flags(){
    # два флага вместе, явная передача паттерна

    for flag in ${flags[@]}; do
        for flag_two in ${flags[@]}; do

        ./s21_grep "a" $flag $flag_two "test_long.txt" > $s21
        grep "a" $flag $flag_two "test_long.txt" > $orig

        if cmp -s $orig $s21;
        then
            echo "test_long: files are same, flags:" $flag $flag_two
        else
            echo "test_long: files are differ, flags:" $flag $flag_two
        fi

        rm -rf $orig
        rm -rf $s21
        done
    done 
}

check_no_flags(){
    # без флагов 
    
    ./s21_grep "a" "test_long.txt" > $s21
    grep "a" "test_long.txt" > $orig

    if cmp -s $orig $s21;
    then
        echo "test_long: files are same, no flags"
    else
        echo "test_long: files are differ, no flags"
    fi

    rm -rf $orig
    rm -rf $s21
}


check_long_line
check_long_line_together_e
check_long_line_together_f
check_two_flags
check_no_flags
