#!/bin/bash

red='\e[0;31m'
green='\e[0;32m'
nc='\e[0m'     

base=$(dirname $0)


function tobase(){
    cd $(dirname $0)
}


function utest( ){

    if [ "$1" == "$2" ]
    then
	echo -ne "$green pass $nc"
    else
	echo -ne "$red FAIL $nc" 
    fi
    echo -ne "\t\t "
    echo "$3"
}

function utest_ne( ){

    if [ "$1" != "$2" ]
    then
	echo -ne "$green pass $nc"
    else
	echo -ne "$red FAIL $nc" 
    fi

    echo -ne "\t\t "
    echo "$3"
}

function utest_nz( ){
    if [ ! -z "$1" ]
    then
	echo -ne "$green pass $nc" 
    else
	echo -ne "$red FAIL $nc" 
    fi
    echo -ne "\t\t "
    echo "$2"

}

function utest_z( ){
    if [ -z "$1" ]
    then
	echo -ne "$green pass $nc" 
    else
	echo -ne "$red FAIL $nc" 
    fi
    echo -ne "\t\t "
    echo "$2"

}


function test_valgrind(){
    echo "--- TEST valgrind ---"

    rm -f memleak memviolation
    gcc -g memleak.c -o memleak
    gcc -g memviolation.c -o memviolation

    cmd="valgrind --leak-check=full ./memleak  2>&1 | grep \"definitely lost\""
    res=$(eval $cmd)
    utest_z "$res" "$cmd"


    cmd="valgrind --leak-check=full ./memleak  2>&1 | grep \"0 errors\""
    res=$(eval $cmd)
    utest_nz "$res" "$cmd"
    

    cmd="valgrind --leak-check=full ./memviolation 2>&1 | grep  \"0 errors\""
    res=$(eval $cmd)
    utest_nz "$res" "$cmd"

    cd ..
    echo
}

function ctime(){
    date +"%a %b %e %H:%M:%S %Y"
}



if [ ! -z $1 ]
then
    cd $1
else
    cd $(dirname $0)
fi

echo

test_valgrind
