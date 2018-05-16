CC = gcc
CFLAGS =  -Wall -lpthread
QP = sbatch QP.slurm
HP = sbatch HP.slurm
SP = sbatch SP.slurm
DP = sbatch DP.slurm


QP:
	gcc  -o main.exe MyCPUBench.c -lpthread
	$(QP)
HP:
	gcc  -o main.exe MyCPUBench.c -lpthread
	$(HP)
SP:
	gcc  -o main.exe MyCPUBench.c -lpthread
	$(SP)
DP:
	gcc  -o main.exe MyCPUBench.c -lpthread
	$(DP)

clean:
	rm -rf main.exe
