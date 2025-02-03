#!/bin/bash

gcc -o ./out/lru_simulator least-recently-used-graph.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida!"
    ./out/lru_simulator
    clear
else
    echo "Erro durante a compilação."
fi
