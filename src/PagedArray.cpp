#include "PagedArray.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm> // Para std::swap

PagedArray::PagedArray(const std::string& file, size_t page_size, size_t num_pages)
    : filename(file), page_size(page_size), num_pages(num_pages), page_faults(0), page_hits(0) {
    infile.open(filename, std::ios::binary);
    if (!infile) {
        throw std::runtime_error("Error opening file for reading.");
    }
    file_size = std::ifstream(filename, std::ios::binary | std::ios::ate).tellg();
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

PagedArray::~PagedArray() {
    infile.close();
}

int PagedArray::load_page(int page_index) {
    if (pages.find(page_index) == pages.end()) {
        if (pages.size() >= num_pages) {
            // Descartar una página al azar si ya tenemos el máximo de páginas en memoria
            auto it = pages.begin();
            std::advance(it, std::rand() % pages.size());
            pages.erase(it);
            page_faults++; // Incrementa el número de page faults cuando se elimina una página
        }
        std::vector<int> page(page_size);
        infile.seekg(page_index * page_size * sizeof(int), std::ios::beg);
        infile.read(reinterpret_cast<char*>(page.data()), page_size * sizeof(int));
        if (infile.gcount() != static_cast<std::streamsize>(page_size * sizeof(int))) {
            // Si se lee menos de lo esperado,se puede ajustar el tamaño de la pagina
            page.resize(infile.gcount() / sizeof(int));
        }
        pages[page_index] = page;
        page_faults++;
    } else {
        page_hits++;
    }
    return page_index;
}

int& PagedArray::operator[](size_t index) {
    int page_index = index / page_size;
    int page_offset = index % page_size;
    load_page(page_index);
    return pages[page_index][page_offset];
}

size_t PagedArray::size() const {
    return file_size / sizeof(int);
}

int PagedArray::getPageFaults() const {
    return page_faults;
}

int PagedArray::getPageHits() const {
    return page_hits;
}
