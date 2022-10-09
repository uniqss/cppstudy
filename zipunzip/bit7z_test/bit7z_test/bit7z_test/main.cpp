#include <iostream>
using std::cout;
using std::endl;
using std::wcout;
#include <filesystem>

#include "bitextractor.hpp"
#include "bitexception.hpp"
#include "bitcompressor.hpp"
#include "bitarchiveinfo.hpp"

using namespace bit7z;

void test_extract() {
    try {
        Bit7zLibrary lib{L"7z.dll"};
        BitExtractor extractor{lib, BitFormat::SevenZip};

        std::wstring inFile = L"test/archive.7z";
        std::wstring outDir = L"out/dir/";
        std::wstring outDirPartial = L"out_partial/";

        std::filesystem::create_directories(outDir);
        extractor.extract(inFile, outDir);  // extracting a simple archive

        std::filesystem::create_directories(outDirPartial);
        extractor.extractMatching(inFile, L"*main.cpp", outDirPartial);  // extracting a specific file

        //// extracting the first file of an archive to a buffer
        // std::vector<byte_t> buffer;
        // extractor.extract(L"path/to/archive.7z", buffer);

        //// extracting an encrypted archive
        // extractor.setPassword(L"password");
        // extractor.extract(L"path/to/another/archive.7z", L"out/dir/");
    } catch (const BitException& ex) {
        // do something with ex.what()...
        cout << ex.what() << endl;
        cout << ex.getErrorCode() << endl;
    }
}

void test_compress() {
    try {
        Bit7zLibrary lib{L"7z.dll"};
        BitCompressor compressor{lib, BitFormat::Zip};

        std::vector<std::wstring> files = {L"main.cpp", L"bit7z_test.vcxproj", L"x64/Debug/bit7z_test.log"};

        std::filesystem::remove(L"output_archive.zip");
        compressor.compress(files, L"output_archive.zip");  // creating a simple zip archive

        // creating a zip archive with a custom directory structure
        std::map<std::wstring, std::wstring> files_map = {{L"main.cpp", L"alias/path/main_alias.cpp"},
                                                          {L"x64/Debug/bit7z_test.log", L"Debug/bit7z_test.log"}};
        std::filesystem::remove(L"output_archive2.zip");
        compressor.compress(files_map, L"output_archive2.zip");

        // compressor.compressDirectory(L"dir/path/", L"dir_archive.zip");  // compressing a directory
        std::filesystem::remove(L"dir_archive.zip");
        compressor.compressDirectory(L"../", L"dir_archive.zip");  // compressing a directory

        // creating an encrypted zip archive of two files
        compressor.setPassword(L"password");
        std::filesystem::remove(L"protected_archive.zip");
        compressor.compressFiles(files, L"protected_archive.zip");

        // updating an existing zip archive
        compressor.setUpdateMode(true);
        compressor.compressFiles(files, L"existing_archive.zip");

        // compressing a single file into a buffer
        std::vector<byte_t> buffer;
        BitCompressor compressor2{lib, BitFormat::BZip2};
        compressor2.compressFile(files[0], buffer);
    } catch (const BitException& ex) {
        // do something with ex.what()...
        cout << ex.what() << endl;
        cout << ex.getErrorCode() << endl;
    }
}

void test_read_meta() {
    try {
        Bit7zLibrary lib{L"7z.dll"};
        BitArchiveInfo arc{lib, L"test/archive.7z", BitFormat::SevenZip};

        // printing archive metadata
        wcout << L"Archive properties" << endl;
        wcout << L" Items count: " << arc.itemsCount() << endl;
        wcout << L" Folders count: " << arc.foldersCount() << endl;
        wcout << L" Files count: " << arc.filesCount() << endl;
        wcout << L" Size: " << arc.size() << endl;
        wcout << L" Packed size: " << arc.packSize() << endl;
        wcout << endl;

        // printing archive items metadata
        wcout << L"Archive items";
        auto arc_items = arc.items();
        for (auto& item : arc_items) {
            wcout << endl;
            wcout << L" Item index: " << item.index() << endl;
            wcout << L"  Name: " << item.name() << endl;
            wcout << L"  Extension: " << item.extension() << endl;
            wcout << L"  Path: " << item.path() << endl;
            wcout << L"  IsDir: " << item.isDir() << endl;
            wcout << L"  Size: " << item.size() << endl;
            wcout << L"  Packed size: " << item.packSize() << endl;
        }
    } catch (const BitException& ex) {
        // do something with ex.what()...
        cout << ex.what() << endl;
        cout << ex.getErrorCode() << endl;
    }
}

int main() {
    test_extract();
    test_compress();
    test_read_meta();
    return 0;
}
