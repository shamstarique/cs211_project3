#!/bin/bash
#BATCH --job-name=part2
#SBATCH --output=part2_2_nodes.txt
#SBATCH -N 2
#SBATCH -t 09:00:00
mpirun -np 64 ./part2 10000000000
