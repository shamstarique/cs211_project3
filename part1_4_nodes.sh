#!/bin/bash
#BATCH --job-name=part1
#SBATCH --output=part1_4_nodes.txt
#SBATCH -N 4
#SBATCH -t 09:00:00
mpirun -np 128 ./part1 10000000000
