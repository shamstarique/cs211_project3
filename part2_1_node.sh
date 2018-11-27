#!/bin/bash
#BATCH --job-name=part2
#SBATCH --output=part2_1_node.txt
#SBATCH -N 1
#SBATCH -t 09:00:00
mpirun -np 32 ./part2 10000000000
