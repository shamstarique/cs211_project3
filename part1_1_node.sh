#!/bin/bash
#BATCH --job-name=part1
#SBATCH --output=part1_1_node.txt
#SBATCH -N 1
#SBATCH -t 09:00:00
mpirun -np 32 ./part1 10000000000
