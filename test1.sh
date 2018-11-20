#!/bin/bash
#SBATCH --job-name=test1
#SBATCH --output=test1.txt
#SBATCH -N 1 
#SBATCH -t 09:10:00
mpirun -np 32 ./test1 10000000000
