#!/bin/bash

gcc -o ./out/pd philosophers-dinner.c -O3 -lpthread -pedantic -Wextra -Wall -Werror

if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida!"
    clear
    ./out/pd
else
    echo "Erro durante a compilação."
fi
