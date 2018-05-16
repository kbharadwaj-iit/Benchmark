CC = gcc
CFLAGS =  -Wall -lpthread
seq = sbatch seq_mem.slurm
rand = sbatch random_mem.slurm


sequential:
	gcc  -o main.exe MyRAMBench.c -lpthread
	$(seq)

random:
	gcc  -o main.exe MyRAMBench.c -lpthread
	$(rand)

clean:
	rm -rf main.exe
