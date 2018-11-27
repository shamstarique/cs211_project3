#!/bin/bash
#BATCH --job-name=part3
#SBATCH --output=part3_1_node.txt
#SBATCH -N 1
#SBATCH -t 09:00:00
mpirun -np 32 ./part3 10000000000
