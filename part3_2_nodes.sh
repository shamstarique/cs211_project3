#!/bin/bash
#BATCH --job-name=part3
#SBATCH --output=part3_2_nodes.txt
#SBATCH -N 2
#SBATCH -t 09:00:00
mpirun -np 64 ./part3 10000000000
