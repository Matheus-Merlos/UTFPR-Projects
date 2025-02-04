#!/bin/bash

gcc -o ./out/c-scan-simulator c-scan-graph.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida!"
    ./out/c-scan-simulator
    clear
else
    echo "Erro durante a compilação."
fi
