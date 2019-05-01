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

try 0 0
try 42 42
try 21 '5+20-4'
try 41 ' 12 + 34 -5 '
try 4 "(3+5)/2"
try 6 "-4+10"
try 10 "+10"
try 201 "$(for i in {1..200}; do echo -n 1+; done && echo -n 1)"

echo OK
