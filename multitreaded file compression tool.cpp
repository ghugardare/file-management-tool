#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <zlib.h>

#define CHUNK_SIZE 262144 // 256 KB

std::mutex file_mutex;

void compress_chunk(const std::vector<unsigned char>& input, std::vector<unsigned char>& output) {
    uLongf compressed_size = compressBound(input.size());
    output.resize(compressed_size);
    if (compress(output.data(), &compressed_size, input.data(), input.size()) != Z_OK) {
        std::cerr << "Compression failed!\n";
    }
    output.resize(compressed_size);
}

void process_file(const std::string& input_filename, const std::string& output_filename, int num_threads) {
    std::ifstream input_file(input_filename, std::ios::binary);
    std::ofstream output_file(output_filename, std::ios::binary);
    if (!input_file || !output_file) {
        std::cerr << "Error opening files!\n";
        return;
    }

    std::vector<std::thread> threads;
    std::vector<std::vector<unsigned char>> compressed_chunks(num_threads);

    while (!input_file.eof()) {
        std::vector<std::vector<unsigned char>> input_chunks(num_threads);
        int chunk_count = 0;
        
        for (int i = 0; i < num_threads && !input_file.eof(); ++i) {
            input_chunks[i].resize(CHUNK_SIZE);
            input_file.read(reinterpret_cast<char*>(input_chunks[i].data()), CHUNK_SIZE);
            input_chunks[i].resize(input_file.gcount());
            if (!input_chunks[i].empty()) {
                chunk_count++;
            }
        }

        for (int i = 0; i < chunk_count; ++i) {
            threads.emplace_back(compress_chunk, std::ref(input_chunks[i]), std::ref(compressed_chunks[i]));
        }

        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();

        for (int i = 0; i < chunk_count; ++i) {
            std::lock_guard<std::mutex> lock(file_mutex);
            uint32_t size = compressed_chunks[i].size();
            output_file.write(reinterpret_cast<char*>(&size), sizeof(size));
            output_file.write(reinterpret_cast<char*>(compressed_chunks[i].data()), size);
        }
    }

    input_file.close();
    output_file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <num_threads>\n";
        return 1;
    }

    std::string input_filename = argv[1];
    std::string output_filename = argv[2];
    int num_threads = std::stoi(argv[3]);

    process_file(input_filename, output_filename, num_threads);

    return 0;
}
