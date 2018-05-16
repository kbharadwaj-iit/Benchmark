CC = gcc
CFLAGS =  -Wall -lpthread
READ_SEQ_TP= sbatch rstp.slurm
READ_RAND_TP = sbatch rrtp.slurm
WRITE_SEQ_TP = sbatch wstp.slurm
WRITE_RAND_TP = sbatch wrtp.slurm
READ_LATENCY = sbatch read_lat.slurm
WRITE_LATENCY = sbatch write_lat.slurm


READ_SEQ_TP:
	gcc  -o disk.exe MyDiskBench.c -lpthread
	$(READ_SEQ_TP)
READ_RAND_TP:
	gcc  -o disk.exe MyDiskBench.c -lpthread
	$(READ_RAND_TP)
WRITE_SEQ_TP:
	gcc  -o disk.exe MyDiskBench.c -lpthread
	$(WRITE_SEQ_TP)
WRITE_RAND_TP:
	gcc  -o disk.exe MyDiskBench.c -lpthread
	$(WRITE_RAND_TP)
READ_LATENCY:
	gcc  -o disk.exe MyDiskBench.c -lpthread
	$(READ_LATENCY)
WRITE_LATENCY:
	gcc  -o disk.exe MyDiskBench.c -lpthread
	$(WRITE_LATENCY)

clean:
	rm -rf disk.exe
	
