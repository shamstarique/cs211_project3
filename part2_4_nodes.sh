#!/bin/bash
#BATCH --job-name=part2
#SBATCH --output=part2_4_nodes.txt
#SBATCH -N 4
#SBATCH -t 09:00:00
mpirun -np 128 ./part2 10000000000
