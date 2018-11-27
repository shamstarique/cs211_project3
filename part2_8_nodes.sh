#!/bin/bash
#BATCH --job-name=part2
#SBATCH --output=part2_8_nodes.txt
#SBATCH -N 8
#SBATCH -t 09:00:00
mpirun -np 256 ./part2 10000000000
