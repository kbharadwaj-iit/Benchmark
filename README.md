# Benchmark
benchmark system components


1. CPU BENCHMARK: 

    a. Open the terminal and navigate to /exports/home/kbharadwaj/cs553-pa1/cpu on Hyperion
    b. Run "make QP" for All the benchmarks related to quarter precision
    
    c. Run "make HP" for All the benchmarks related to Half precision
    
    d. Run "make SP" for All the benchmarks related to Single precision
    
    e. Run "make DP" for All the benchmarks related to Double precision
    
    f. Each execution will generate an output file in /exports/home/kbharadwaj/cs553-pa1/cpu/output folder


2. MEMORY BENCHMARK:

  
    a. Open the terminal and navigate to /exports/home/kbharadwaj/cs553-pa1/memory on Hyperion
    
    b. Run "make sequential" for All the benchmarks related to sequential access pattern
    
    c. Run "make random" for All the benchmarks related to random access pattern
    
    d. Each execution will generate an output file in /exports/home/kbharadwaj/cs553-pa1/memory/output folder



    3.DISK BENCHMARK: 
	
 
    a. Open the terminal and navigate to /exports/home/kbharadwaj/cs553-pa1/disk on Prometheus
    
    b. Run "make READ_SEQ_TP" for All the benchmarks related to sequential reads
    
    c. Run "make READ_RAND_TP" for All the benchmarks related to random reads
    
    d. Run "make WRITE_SEQ_TP" for All the benchmarks related to sequential writes 
    
    e. Run "make WRITE_RAND_TP" for All the benchmarks related to random writes 
    
    f. Run "make READ_LATENCY" for All the benchmarks related to read latency
    
    g. Run "make WRITE_LATENCY" for All the benchmarks related to write latency
    
    h. Each execution will generate an output file in /exports/home/kbharadwaj/cs553-pa1/disk/output folder




4.NETWORK BENCHMARK:

    a. Open the terminal and navigate to /exports/home/kbharadwaj/cs553-pa1/network on Hyperion
    
    b. Run "make TCP_SERVER" to initialize the TCP server. Run one of the following commands below to run the TCP benchmarks
    
          i. Run "make TCP_THROUGHPUT_1000" for All the benchmarks related to TCP Throughput of 1kb block size
        
         ii. Run "make TCP_THROUGHPUT_32000" for All the benchmarks related to TCP Throughput of 32kb block size
        
        iii. Run "make TCP_LATENCY" for All the benchmarks related to TCP Latency 
    
    c. Run "make UDP_SERVER" to initialize the UDP server. Run one of the following commands below to run the UDP benchmarks
    
          i. Run "make UDP_THROUGHPUT_32000" for All the benchmarks related to UDP Throughput of 1kb block size
          
         ii. Run "make UDP_THROUGHPUT_1000" for All the benchmarks related to UDP Throughput of 32kb block size
         
        iii. Run "make UDP_LATENCY" for All the benchmarks related to UDP latency
    
    h. Each execution will generate an output file in /exports/home/kbharadwaj/cs553-pa1/network/output folder

    NOTE: Please run the server commands(TCP & UDP) only once.