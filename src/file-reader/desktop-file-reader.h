#include <fstream>
#include "file-reader.h"

#ifndef __DESKTOP_FILE_READER_H__
#define __DESKTOP_FILE_READER_H__

class DesktopFileReader : public FileReader
{
    std::fstream file;

public:
    bool open(const std::string &path) override
    {
        file.open(path, std::ios::binary | std::ios::in | std::ios::out);
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

    void write(std::string value) override
    {
        if (file.is_open())
        {
            file << value;
        }
    }

    void close() override
    {
        if (file.is_open())
        {
            file.close();
        }
    }
};

#endif // __DESKTOP_FILE_READER_H__