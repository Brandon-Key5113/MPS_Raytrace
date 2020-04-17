#!/bin/bash
#

# This is an example bash script that is used to submit a job
# to the cluster.
#
# Typcially, the # represents a comment.However, #SBATCH is
# interpreted by SLURM to give an option from above. As you 
# will see in the following lines, it is very useful to provide
# that information here, rather than the command line.

# Name of the job - You MUST use a unique name for the job
#SBATCH -J rt_mpi

# Standard out and Standard Error output files
# SBATCH -o rt_mpi.out
# SBATCH -e rt_mpi.err

# In order for this to send emails, you will need to remove the
# space between # and SBATCH for the following 2 commands.
# Specify the recipient of the email
# SBATCH --mail-user=abc1234@rit.edu

# Notify on state change: BEGIN, END, FAIL or ALL
# SBATCH --mail-type=ALL

# Multiple options can be used on the same line as shown below.
# Here, we set the partition, number of cores to use, and the
# number of nodes to spread the jobs over.
#SBATCH -p kgcoe-mps -n 6
#SBATCH --mem-per-cpu=2000M

#
# Your job script goes below this line.
#

# If the job that you are submitting is not sequential,
# then you MUST provide this line...it tells the node(s)
# that you want to use this implementation of MPI. If you
# omit this line, your results will indicate failure.
module load openmpi

# Place your srun command here
# Notice that you have to provide the number of processes that
# are needed. This number needs to match the number of cores
# indicated by the -n option. If these do not, your results will
# not be valid or you may have wasted resources that others could
# have used.

# The following commands can be used as a starting point for 
# timing. Each example below needs to be modified to fit your parameters
# accordingly. You will also need to modify the config to point to
# box.xml to render the complex scene.
# **********************************************************************
# MAKE SURE THAT YOU ONLY HAVE ONE OF THESE UNCOMMENTED AT A TIME!
# **********************************************************************

COMMON_ARGS="--mem-per-cpu=2000M raytrace_mpi -h 5000 -w 5000"
OUT_FILE="rt_mpi.out"

rm $OUT_FILE

# Sequential
#srun -n 6 raytrace_mpi -h 1000 -w 1000 -c configs/twhitted.xml -p none 

# Static Strips


#srun -n 1 $COMMON_ARGS -c configs/twhitted.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 2 $COMMON_ARGS -c configs/twhitted.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/twhitted.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 20 $COMMON_ARGS -c configs/twhitted.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 25 $COMMON_ARGS -c configs/twhitted.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/twhitted.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 49 $COMMON_ARGS -c configs/twhitted.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 55 $COMMON_ARGS -c configs/twhitted.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 64 $COMMON_ARGS -c configs/twhitted.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 1 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 2 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 20 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 25 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 49 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 55 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 64 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE

# Static Cycles
#srun -n 3 --mem-per-cpu=2000M raytrace_mpi -h 5000 -w 5000 -c configs/twhitted.xml -p static_cycles_horizontal -cs 2
#srun -n 6 --mem-per-cpu=2000M raytrace_mpi -h 1000 -w 1000 -c configs/twhitted.xml -p static_cycles_vertical -cs 1

#srun -n 4 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 1 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 5 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 10 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 20 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 80 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 320 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 640 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 1280 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 1 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 5 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 10 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 20 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 40 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 160 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 350 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 650 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 1 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 5 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 10 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 20 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 50 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 100 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 250 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 400 |& tee -a $OUT_FILE

#srun -n 4 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 1 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 5 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 10 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 20 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 80 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 320 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 640 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 1280 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 1 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 5 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 10 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 20 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 40 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 160 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 350 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 650 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 1 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 5 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 10 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 20 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 50 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 100 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 250 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 400 |& tee -a $OUT_FILE

#srun -n 1 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 2 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 20 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 25 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 49 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 55 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 64 $COMMON_ARGS -c configs/twhitted.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE

#srun -n 1 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 2 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 20 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 25 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 49 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 55 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE
#srun -n 64 $COMMON_ARGS -c configs/box.xml -p static_cycles_horizontal -cs 27 |& tee -a $OUT_FILE

# Static Blocks
#srun -n 9 --mem-per-cpu=2000M -o rt_mpi.out -e rt_mpi.err raytrace_mpi -h 1000 -w 1000 -c configs/twhitted.xml -p static_blocks 
#srun -n 9 --mem-per-cpu=2000M raytrace_mpi -h 1000 -w 1000 -c configs/twhitted.xml -p static_blocks 

#srun -n 1 $COMMON_ARGS -c configs/twhitted.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/twhitted.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 25 $COMMON_ARGS -c configs/twhitted.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/twhitted.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 49 $COMMON_ARGS -c configs/twhitted.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 64 $COMMON_ARGS -c configs/twhitted.xml -p static_blocks |& tee -a $OUT_FILE

#srun -n 1 $COMMON_ARGS -c configs/box.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 4 $COMMON_ARGS -c configs/box.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 25 $COMMON_ARGS -c configs/box.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/box.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 49 $COMMON_ARGS -c configs/box.xml -p static_blocks |& tee -a $OUT_FILE
#srun -n 64 $COMMON_ARGS -c configs/box.xml -p static_blocks |& tee -a $OUT_FILE


# Dynamic
# srun -n $SLURM_NPROCS raytrace_mpi -h 5000 -w 5000 -c configs/twhitted.xml -p dynamic -bh 1 -bw 1 

#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 1 -bw 1 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 15 -bw 15 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 25 -bw 25 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 50 -bw 50 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 75 -bw 75 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 100 -bw 100 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 1 -bw 1 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 15 -bw 15 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 25 -bw 25 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 50 -bw 50 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 75 -bw 75 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 100 -bw 100 |& tee -a $OUT_FILE

#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 11 -bw 1 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 1 -bw 10 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 7 -bw 13 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 5 -bw 29 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 9 -bw 110 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 70 -bw 1 |& tee -a $OUT_FILE

#srun -n 36 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 11 -bw 1 |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 1 -bw 10 |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 7 -bw 13 |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 5 -bw 29 |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 9 -bw 110 |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/twhitted.xml -p dynamic -bh 70 -bw 1 |& tee -a $OUT_FILE

#srun -n 9 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 1 -bw 1 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 15 -bw 15 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 25 -bw 25 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 50 -bw 50 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 75 -bw 75 |& tee -a $OUT_FILE
#srun -n 9 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 100 -bw 100 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 1 -bw 1 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 15 -bw 15 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 25 -bw 25 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 50 -bw 50 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 75 -bw 75 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 100 -bw 100 |& tee -a $OUT_FILE

#srun -n 16 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 11 -bw 1 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 1 -bw 10 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 7 -bw 13 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 5 -bw 29 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 9 -bw 110 |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 70 -bw 1 |& tee -a $OUT_FILE

#srun -n 36 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 11 -bw 1 |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 1 -bw 10 |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 7 -bw 13 |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 5 -bw 29 |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 9 -bw 110 |& tee -a $OUT_FILE
#srun -n 36 $COMMON_ARGS -c configs/box.xml -p dynamic -bh 70 -bw 1 |& tee -a $OUT_FILE


#srun -n 9 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 16 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 20 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
#srun -n 25 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
srun -n 36 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
srun -n 49 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
srun -n 55 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
srun -n 64 $COMMON_ARGS -c configs/box.xml -p static_strips_horizontal |& tee -a $OUT_FILE
