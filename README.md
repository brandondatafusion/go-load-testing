# Getting Started

To build and run the go benchmark:
```
go build -ldflags="-w -s" -o benchmark.exe main.go
.\benchmark.exe
```

To build and run the c++ benchmark:
```
g++ -O3 -march=native main.cpp -o benchmark_cpp
.\benchmark_cpp.exe
```

# Results
Overall, minimal difference between go and c++ build as expected:
```
PS C:\Users\bg01\code\go-load-test> .\benchmark.exe

Benchmark Results:
Total time: 2.37 seconds
Files generated: 1000
Total data processed: 1000.00 MB
Performance: 3.53 Gbps
PS C:\Users\bg01\code\go-load-test> .\benchmark_cpp.exe

Benchmark Results:
Total time: 3.87305 seconds
Files generated: 1000
Total data processed: 1000 MB
Performance: 2.16589 Gbps
PS C:\Users\bg01\code\go-load-test> .\benchmark.exe

Benchmark Results:
Total time: 3.59 seconds
Files generated: 1000
Total data processed: 1000.00 MB
Performance: 2.34 Gbps
PS C:\Users\bg01\code\go-load-test> .\benchmark_cpp.exe

Benchmark Results:
Total time: 3.3508 seconds
Files generated: 1000
Total data processed: 1000 MB
Performance: 2.50347 Gbps
```
