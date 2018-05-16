CC = gcc
CFLAGS =  -Wall -lpthread
TCP_SERVER = sbatch tcp_server.slurm
UDP_SERVER = sbatch udp_server.slurm
TCP_THROUGHPUT_1000 = sbatch tcp_tp_1000.slurm
TCP_THROUGHPUT_32000 = sbatch tcp_tp_32000.slurm
TCP_LATENCY = sbatch tcp_latency.slurm
UDP_THROUGHPUT_1000 = sbatch udp_tp_1000.slurm
UDP_THROUGHPUT_32000 = sbatch udp_tp_32000.slurm
UDP_LATENCY = sbatch udp_latency.slurm

TCP_SERVER:
	gcc  -o testtcp.exe MyNETBench-TCP.c -lpthread
	$(TCP_SERVER)
UDP_SERVER:
	gcc  -o udp_t.exe MyNETBench-UDP.c -lpthread
	$(UDP_SERVER)
TCP_THROUGHPUT_1000:
	gcc  -o testtcp.exe MyNETBench-TCP.c -lpthread
	$(TCP_THROUGHPUT_1000)
TCP_THROUGHPUT_32000:
	gcc  -o testtcp.exe MyNETBench-TCP.c -lpthread
	$(TCP_THROUGHPUT_32000)
TCP_LATENCY:
	gcc  -o testtcp.exe MyNETBench-TCP.c -lpthread
	$(TCP_LATENCY)
UDP_THROUGHPUT_1000:
	gcc  -o udp_t.exe MyNETBench-UDP.c -lpthread
	$(UDP_THROUGHPUT_1000)
UDP_THROUGHPUT_32000:
	gcc  -o udp_t.exe MyNETBench-UDP.c -lpthread
	$(UDP_THROUGHPUT_32000)
UDP_LATENCY:
	gcc  -o udp_t.exe MyNETBench-UDP.c -lpthread
	$(UDP_LATENCY)

clean:
	rm -rf testtcp.exe
	rm -rf udp_t.exe
