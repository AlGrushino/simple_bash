#!/bin/bash

test="test.txt"
test2="test2.txt"
orig="orig_res.txt"
s21="s21_res.txt"
orig_v="orig_v.txt"
s21_v="s21_v.txt"
orig_files="orig_files.txt"
s21_files="s21_files.txt"

flags=(
    "-b"
    "-e"
    "-n"
    "-s"
    "-t"
    "-E"
    "-T"
    "--number"
    "--squeeze-blank"
    "--number-nonblank"
)

check_flags(){
    for flag in ${flags[@]}; do

        ./s21_cat $flag $test > $s21
        cat $flag $test > $orig

        if cmp -s $orig $s21;
        then
            echo "files are same, flag:" $flag
        else
            echo "files are differ, flag:" $flag
        fi

        rm -rf $orig
        rm -rf $s21
    done
}

check_flags

check_v_flag(){
    v_flags=("-e" "-t")
    unprintable="unprintable.txt"

    for flag in ${v_flags[@]}; do

        ./s21_cat $flag $unprintable > $s21_v
        cat $flag $unprintable > $orig_v

        if cmp -s $orig_v $s21_v;
        then
            echo "files are same, flag v in flag:" $flag
        else
            echo "files are differ, flag v in flag:" $flag
        fi

        rm -rf $orig_v
        rm -rf $s21_v
    done
}

check_v_flag

check_two_files(){
    for flag in ${flags[@]}; do

        ./s21_cat $flag $test $test2 > $s21_files
        cat $flag $test $test2 > $orig_files

        if cmp -s $orig_files $s21_files;
        then
            echo "files are same, two files check, flag:" $flag
        else
            echo "files are differ, two files check, flag:" $flag
        fi

        rm -rf $orig_files
        rm -rf $s21_files
    done
}

check_two_files
