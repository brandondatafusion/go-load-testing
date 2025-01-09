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

// Generate random data using a cryptographically secure random number generator
std::vector<uint8_t> generateRandomData(size_t size) {
    std::vector<uint8_t> data(size);
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned short> dis(0, 255);
    
    for (size_t i = 0; i < size; ++i) {
        data[i] = static_cast<uint8_t>(dis(gen));
    }
    
    return data;
}

// Invert each byte in the data
std::vector<uint8_t> invertData(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> inverted(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        inverted[i] = ~data[i]; // Bitwise NOT operator
    }
    return inverted;
}

// Write data to file
bool writeToFile(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        return false;
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return file.good();
}

int main() {
    try {
        fs::create_directories(TEMP_DIR);
        
        std::vector<uint8_t> data = generateRandomData(DATA_SIZE);
        
        // Start benchmark
        auto startTime = std::chrono::high_resolution_clock::now();
        size_t totalBytesProcessed = 0;
        
        for (int i = 0; i < ITERATIONS; ++i) {
            auto inverted = invertData(data);
            std::string fileName = TEMP_DIR + "/inverted_" + std::to_string(i) + ".bin";
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