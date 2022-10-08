#include "zip.h"
#include <string>
#include <vector>

void test_simple() {
    struct zip_t *zip = zip_open("foo.zip", ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
    {
        zip_entry_open(zip, "foo-1.txt");
        {
            const char *buf = "Some data here...\0";
            zip_entry_write(zip, buf, strlen(buf));
        }
        zip_entry_close(zip);

        zip_entry_open(zip, "foo-2.txt");
        {
            // merge 3 files into one entry and compress them on-the-fly.
            zip_entry_fwrite(zip, "foo-2.1.txt");
            zip_entry_fwrite(zip, "foo-2.2.txt");
            zip_entry_fwrite(zip, "foo-2.3.txt");
        }
        zip_entry_close(zip);
    }
    zip_close(zip);
}

int compress_multi_local_files(const std::string &zipName, const std::vector<std::string> &files,
                               int compressionLevel = ZIP_DEFAULT_COMPRESSION_LEVEL) {
    struct zip_t *zip = zip_open(zipName.c_str(), compressionLevel, 'w');
    if (!zip) {
        return -1;
    }

    for (auto file : files) {
        zip_entry_open(zip, file.c_str());
        zip_entry_fwrite(zip, file.c_str());
        zip_entry_close(zip);
    }

    zip_close(zip);

    return 0;
}

int main() {
    test_simple();

    compress_multi_local_files("test.zip", {"./main.cpp", "./zip.h"});

    return 0;
}
