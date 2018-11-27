#!/bin/bash
#BATCH --job-name=part3
#SBATCH --output=part3_8_nodes.txt
#SBATCH -N 8
#SBATCH -t 09:00:00
mpirun -np 256 ./part3 10000000000
