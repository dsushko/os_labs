#!/bin/bash
g++ m.cpp -o m
g++ a.cpp -o a
g++ p.cpp -o p
g++ s.cpp -o s
g++ main.cpp -o main
./main  ./p ./m ./a ./s