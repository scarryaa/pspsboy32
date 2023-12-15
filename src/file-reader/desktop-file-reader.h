#include <fstream>
#include "file-reader.h"

class DesktopFileReader : public FileReader
{
    std::ifstream file;

public:
    bool open(const std::string &path) override
    {
        file.open(path, std::ios::binary);
        return file.is_open();
    }

    size_t read(char *buffer, size_t size) override
    {
        if (!file.read(buffer, size))
        {
            return file.gcount();
        }
        return size;
    }

    void close() override
    {
        if (file.is_open())
        {
            file.close();
        }
    }
};
