#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
namespace fs = std::filesystem;

void help(int argc, const char** argv) {
    cout << "############################ usage:" << endl;
    cout << argv[0] << " from_str to_str [dir]" << endl;
}

void lineReplaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

int readFileList(const char* basePath, std::vector<std::pair<bool, std::string>>* vecRetNoPath,
                 std::vector<std::pair<bool, std::string>>* vecRetWithPath) {
    for (const fs::directory_entry& entry : fs::recursive_directory_iterator(basePath)) {
        // Is it a file / directory?
        bool isNormalFile = fs::is_regular_file(entry);
        bool isDirectory = fs::is_directory(entry);
        auto path = entry.path();
        // Path: my-directory/test.txt
        std::string pathString = path.string();
        if (vecRetWithPath) vecRetWithPath->push_back({isDirectory, pathString});
        // Filename: test.txt
        std::string filenameString = path.filename().string();
        if (vecRetNoPath) vecRetNoPath->push_back({isDirectory, filenameString});
        // Extension: txt
        std::string extensionString = path.extension().string();
        // NOTE: You can also "cout << path" directly
    }

    return 0;
}

int main(int argc, const char** argv) {
    int ret = 0;
    std::vector<std::pair<bool, std::string>> vecRetWithPath;

    std::string dir = "./";

    if (argc < 3) {
        help(argc, argv);
        return -1;
    }

    std::string from = argv[1];
    std::string to = argv[2];
    if (argc > 3) {
        dir = argv[3];
    }

    const std::string replaceTmp = "./.dirstrreplace_tmp";
    std::filesystem::remove(replaceTmp);

    ret = readFileList(dir.c_str(), nullptr, &vecRetWithPath);

    if (ret != 0) {
        return ret;
    }

    for (auto it : vecRetWithPath) {
        if (it.first) continue;
        std::vector<std::string> lines;
        std::filesystem::copy_file(it.second, replaceTmp, std::filesystem::copy_options::overwrite_existing);
        std::filesystem::remove(it.second);
        ifstream ifs(replaceTmp);
        std::string line;
        while (std::getline(ifs, line)) {
            lineReplaceAll(line, from, to);
            lines.emplace_back(line + '\n');
        }

        // fucking windows \r\n
        // ofstream ofs(it.second);
        ofstream ofs(it.second, std::ios::binary);
        for (auto it : lines) {
            ofs << it;
        }
    }

    std::filesystem::remove(replaceTmp);

    return 0;
}
