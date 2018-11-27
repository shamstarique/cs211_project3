#!/bin/bash
#BATCH --job-name=part3
#SBATCH --output=part3_4_nodes.txt
#SBATCH -N 4
#SBATCH -t 09:00:00
mpirun -np 128 ./part3 10000000000
