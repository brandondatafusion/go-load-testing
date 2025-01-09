#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdint>

namespace fs = std::filesystem;

constexpr size_t DATA_SIZE = 1024 * 1024; // 1MB in bytes
constexpr int ITERATIONS = 1000;
const std::string TEMP_DIR = "benchmark_files_cpp";

// Pre-allocate a buffer for inverted data to avoid repeated allocations
std::vector<uint8_t> invertData(const std::vector<uint8_t>& data, std::vector<uint8_t>& buffer) {
    for (size_t i = 0; i < data.size(); ++i) {
        buffer[i] = ~data[i];
    }
    return buffer;
}

// Use buffered writing for better I/O performance
bool writeToFile(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    if (!file) {
        return false;
    }
    
    // Set buffer size for file operations
    constexpr size_t BUFFER_SIZE = 64 * 1024; // 64KB buffer
    char buffer[BUFFER_SIZE];
    file.rdbuf()->pubsetbuf(buffer, BUFFER_SIZE);
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return file.good();
}

int main() {
    try {
        // Create temporary directory
        fs::create_directories(TEMP_DIR);
        
        // Generate initial random data
        std::vector<uint8_t> data(DATA_SIZE);
        {
            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::uniform_int_distribution<unsigned short> dis(0, 255);
            for (size_t i = 0; i < DATA_SIZE; ++i) {
                data[i] = static_cast<uint8_t>(dis(gen));
            }
        }
        
        // Pre-allocate buffer for inverted data
        std::vector<uint8_t> invertBuffer(DATA_SIZE);
        
        // Start benchmark
        auto startTime = std::chrono::high_resolution_clock::now();
        size_t totalBytesProcessed = 0;
        
        for (int i = 0; i < ITERATIONS; ++i) {
            // Invert data using pre-allocated buffer
            auto& inverted = invertData(data, invertBuffer);
            
            // Create filename
            std::string fileName = TEMP_DIR + "/inverted_" + std::to_string(i) + ".bin";
            
            // Write to file with buffering
            if (!writeToFile(fileName, inverted)) {
                std::cerr << "Error writing file: " << fileName << std::endl;
                return 1;
            }
            
            totalBytesProcessed += inverted.size();
        }
        
        // Calculate performance metrics
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        double durationSeconds = duration.count() / 1000000.0;
        double bytesPerSecond = totalBytesProcessed / durationSeconds;
        double gbitsPerSecond = (bytesPerSecond * 8) / 1000000000; // Convert to Gbps
        
        // Print results
        std::cout << "\nBenchmark Results:" << std::endl;
        std::cout << "Total time: " << durationSeconds << " seconds" << std::endl;
        std::cout << "Files generated: " << ITERATIONS << std::endl;
        std::cout << "Total data processed: " << (totalBytesProcessed / (1024.0 * 1024.0)) << " MB" << std::endl;
        std::cout << "Performance: " << gbitsPerSecond << " Gbps" << std::endl;
        
        // Cleanup
        fs::remove_all(TEMP_DIR);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}