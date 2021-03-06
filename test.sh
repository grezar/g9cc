#!/bin/bash

try() {
  expected="$1"
  input="$2"

  ./g9cc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$expected expected, but got $actual"
    exit 1
  fi
}

try 0 '0;'
try 42 '42;'
try 21 '5+20-4;'
try 41 ' 12 + 34 -5 ;'
try 4 '(3+5)/2;'
try 6 '-4+10;'
try 10 '+10;'
try 201 "$(for i in {1..200}; do echo -n 1+; done && echo -n 1\;)"
try 41 '41;'
try 100 '81 + 19;'
try 3 'a=1;b=2;a+b;'
try 18 'y=19; p=1; y-p;'
try 100 'a=10; c=10; a * c;'
try 10 'a = 100; b = 10; a / b;'
try 15 'a = 3; b = 2; c = a + b; a * c;'
try 2 'return 2;'
try 5 'return 5; return 8;'

echo OK
