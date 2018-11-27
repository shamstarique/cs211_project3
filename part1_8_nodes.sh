#!/bin/bash
#BATCH --job-name=part1
#SBATCH --output=part1_8_nodes.txt
#SBATCH -N 8
#SBATCH -t 09:00:00
mpirun -np 256 ./part1 10000000000
