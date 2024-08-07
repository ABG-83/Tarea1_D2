#include <iostream>
#include <fstream>
#include <cstring>
#include <random>
#include "PagedArray.h"

void generate_file(const std::string& filename, size_t size_in_bytes) {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis;

    size_t num_integers = size_in_bytes / sizeof(int);
    for (size_t i = 0; i < num_integers; ++i) {
        int num = dis(gen);
        outfile.write(reinterpret_cast<const char*>(&num), sizeof(int));
    }
}

int main(int argc, char* argv[]) {
    std::string size_str;
    std::string output_file;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-size") == 0 && i + 1 < argc) {
            size_str = argv[++i];
        } else if (std::strcmp(argv[i], "-output") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        }
    }

    size_t size_in_bytes = 0;
    if (size_str == "SMALL") {
        size_in_bytes = 512 * 1024 * 1024; // 512 MB
    } else if (size_str == "MEDIUM") {
        size_in_bytes = 1024 * 1024 * 1024; // 1 GB
    } else if (size_str == "LARGE") {
        size_in_bytes = 2 * 1024 * 1024 * 1024; // 2 GB
    } else {
        std::cerr << "Invalid size argument: " << size_str << std::endl;
        return 1;
    }

    generate_file(output_file, size_in_bytes);
    return 0;
}
