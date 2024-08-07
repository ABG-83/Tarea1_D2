#ifndef PAGEDARRAY_H
#define PAGEDARRAY_H

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream> // Añadir esto para std::ifstream

class PagedArray {
private:
    std::string filename;
    std::ifstream infile;
    std::unordered_map<int, std::vector<int>> pages;
    size_t page_size;
    size_t num_pages;
    size_t file_size;
    int page_faults;
    int page_hits;

    int load_page(int page_index);

public:
    PagedArray(const std::string& file, size_t page_size, size_t num_pages);
    ~PagedArray();

    int& operator[](size_t index);
    size_t size() const;
    int getPageFaults() const;
    int getPageHits() const;
};

#endif // PAGEDARRAY_H
