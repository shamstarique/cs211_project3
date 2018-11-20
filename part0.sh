#!/bin/bash
#BATCH --job-name=part0
#SBATCH --output=part0.txt
#SBATCH -N 1
#SBATCH -t 09:00:00
mpirun -np 32 ./part0 10000000000
