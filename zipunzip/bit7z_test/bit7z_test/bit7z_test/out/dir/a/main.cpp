#include <iostream>
using std::cout;
using std::endl;


#include "bitextractor.hpp"
#include "bitexception.hpp"

using namespace bit7z;

void test_extract() {
    try {
        Bit7zLibrary lib{L"7z.dll"};
        BitExtractor extractor{lib, BitFormat::SevenZip};

        extractor.extract(L"path/to/archive.7z", L"out/dir/");  // extracting a simple archive

        extractor.extractMatching(L"path/to/arc.7z", L"file.pdf", L"out/dir/");  // extracting a specific file

        // extracting the first file of an archive to a buffer
        std::vector<byte_t> buffer;
        extractor.extract(L"path/to/archive.7z", buffer);

        // extracting an encrypted archive
        extractor.setPassword(L"password");
        extractor.extract(L"path/to/another/archive.7z", L"out/dir/");
    } catch (const BitException& ex) {
        // do something with ex.what()...
        cout << ex.what() << endl;
        cout << ex.getErrorCode() << endl;
    }
}

int main() {
    test_extract();
    return 0;
}
