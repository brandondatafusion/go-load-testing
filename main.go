package main

import (
	"crypto/rand"
	"fmt"
	"os"
	"path/filepath"
	"time"
)

const (
	dataSize   = 1024 * 1024
	iterations = 1000
	tempDir    = "benchmark_files"
)

// invertData inverts each byte in the given data
func invertData(data []byte) []byte {
	inverted := make([]byte, len(data))
	for i, b := range data {
		inverted[i] = ^b // Bitwise NOT operator
	}
	return inverted
}

// generateRandomData creates a byte slice filled with random data
func generateRandomData(size int) ([]byte, error) {
	data := make([]byte, size)
	_, err := rand.Read(data)
	if err != nil {
		return nil, fmt.Errorf("failed to generate random data: %v", err)
	}
	return data, nil
}

func main() {
	// Create temporary directory
	err := os.MkdirAll(tempDir, 0755)
	if err != nil {
		fmt.Printf("Failed to create temporary directory: %v\n", err)
		return
	}
	defer os.RemoveAll(tempDir) // Clean up after benchmark

	// Generate initial random data
	data, err := generateRandomData(dataSize)
	if err != nil {
		fmt.Printf("Error generating random data: %v\n", err)
		return
	}

	// Start benchmark
	startTime := time.Now()
	totalBytesProcessed := 0

	for i := 0; i < iterations; i++ {
		inverted := invertData(data)
		fileName := filepath.Join(tempDir, fmt.Sprintf("inverted_%d.bin", i))
		err := os.WriteFile(fileName, inverted, 0644)
		if err != nil {
			fmt.Printf("Error writing file %s: %v\n", fileName, err)
			return
		}
		totalBytesProcessed += len(inverted)
	}

	// Calculate performance metrics
	duration := time.Since(startTime)
	bytesPerSecond := float64(totalBytesProcessed) / duration.Seconds()
	gbitsPerSecond := (bytesPerSecond * 8) / 1000000000 // Convert to Gbps

	// Print results
	fmt.Printf("\nBenchmark Results:\n")
	fmt.Printf("Total time: %.2f seconds\n", duration.Seconds())
	fmt.Printf("Files generated: %d\n", iterations)
	fmt.Printf("Total data processed: %.2f MB\n", float64(totalBytesProcessed)/(1024*1024))
	fmt.Printf("Performance: %.2f Gbps\n", gbitsPerSecond)
}
