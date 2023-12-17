#include "file-reader.h"
#include <cstdio>
#include <cstring>
#include <iostream>

#ifndef __DESKTOP_FILE_READER_H__
#define __DESKTOP_FILE_READER_H__

class DesktopFileReader : public FileReader
{
    FILE *file;

public:
    DesktopFileReader() : file(nullptr) {}

    ~DesktopFileReader()
    {
        if (file)
        {
            fclose(file);
        }
    }

    bool open(const std::string &path) override
    {
        // Open for reading and writing in binary mode
        file = fopen(path.c_str(), "rb+");
        if (!file)
        {
            std::cerr << "Failed to open file: " << path << std::endl;
        }
        return file != nullptr;
    }

    size_t read(char *buffer, size_t size) override
    {
        if (!file)
        {
            std::cerr << "File not open for reading." << std::endl;
            return 0;
        }

        size_t bytesRead = fread(buffer, 1, size, file);
        if (bytesRead != size && ferror(file))
        {
            std::cerr << "Read error occurred." << std::endl;
        }
        return bytesRead;
    }

    void write(std::string value) override
    {
        if (!file)
        {
            std::cerr << "File not open for writing." << std::endl;
            return;
        }

        size_t bytesWritten = fwrite(value.c_str(), 1, value.size(), file);
        if (bytesWritten != value.size())
        {
            std::cerr << "Write error occurred." << std::endl;
        }
    }

    void close() override
    {
        if (file)
        {
            fclose(file);
            file = nullptr;
        }
    }
};

#endif // __DESKTOP_FILE_READER_H__
