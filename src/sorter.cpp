#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <chrono>
#include "PagedArray.h"

// Declaración de algoritmos de ordenamiento
void quick_sort(PagedArray& arr, int low, int high);
void insertion_sort(PagedArray& arr, int start, int end);
void bubble_sort(PagedArray& arr, int start, int end);
int partition(PagedArray& arr, int low, int high);

void print_usage() {
    std::cerr << "Usage: sorter -input <INPUT FILE PATH> -output <OUTPUT FILE PATH> -alg <ALGORITHM>" << std::endl;
    std::cerr << "Algorithms: QS (Quick Sort), IS (Insertion Sort), BS (Bubble Sort)" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string input_file;
    std::string output_file;
    std::string algorithm;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-input") == 0 && i + 1 < argc) {
            input_file = argv[++i];
        } else if (std::strcmp(argv[i], "-output") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (std::strcmp(argv[i], "-alg") == 0 && i + 1 < argc) {
            algorithm = argv[++i];
        } else {
            print_usage();
            return 1;
        }
    }

    if (input_file.empty() || output_file.empty() || algorithm.empty()) {
        print_usage();
        return 1;
    }

    // Initialize PagedArray
    const size_t page_size = 4096; // Tamaño de página en enteros
    const size_t num_pages = 4; // Número máximo de páginas en memoria
    PagedArray paged_array(input_file, page_size, num_pages);

    auto start = std::chrono::high_resolution_clock::now();

    // Apply sorting algorithm
    if (algorithm == "QS") {
        quick_sort(paged_array, 0, paged_array.size() - 1);
    } else if (algorithm == "IS") {
        insertion_sort(paged_array, 0, paged_array.size() - 1);
    } else if (algorithm == "BS") {
        bubble_sort(paged_array, 0, paged_array.size() - 1);
    } else {
        std::cerr << "Unknown algorithm: " << algorithm << std::endl;
        return 1;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Write sorted data to output file
    std::ofstream outfile(output_file, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error opening file for writing: " << output_file << std::endl;
        return 1;
    }

    for (size_t i = 0; i < paged_array.size(); ++i) {
        int value = paged_array[i];
        outfile.write(reinterpret_cast<const char*>(&value), sizeof(int));
    }

    outfile.close();

    // Print statistics
    std::cout << "Time elapsed: " << elapsed.count() << " seconds" << std::endl;
    std::cout << "Algorithm used: " << algorithm << std::endl;
    std::cout << "Page faults: " << paged_array.getPageFaults() << std::endl;
    std::cout << "Page hits: " << paged_array.getPageHits() << std::endl;

    return 0;
}

// Implementaciones de los algoritmos de ordenamiento
int partition(PagedArray& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quick_sort(PagedArray& arr, int low, int high) {
    if (low < high) {
        int pivot_index = partition(arr, low, high);
        quick_sort(arr, low, pivot_index - 1);
        quick_sort(arr, pivot_index + 1, high);
    }
}

void insertion_sort(PagedArray& arr, int start, int end) {
    for (int i = start + 1; i <= end; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= start && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void bubble_sort(PagedArray& arr, int start, int end) {
    for (int i = start; i < end; ++i) {
        for (int j = start; j < end - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}
