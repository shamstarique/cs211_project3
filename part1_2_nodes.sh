#!/bin/bash
#BATCH --job-name=part1
#SBATCH --output=part1_2_nodes.txt
#SBATCH -N 2
#SBATCH -t 09:00:00
mpirun -np 64 ./part1 10000000000
