#!/bin/bash

gcc -o ./out/pc ./prod-cons.c -pedantic -Wextra -Wall -Werror -lpthread -O3

if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida!"
    clear
    ./out/pc
else
    echo "Erro durante a compilação."
fi
