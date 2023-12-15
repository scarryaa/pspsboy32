#include <string>

#ifndef FILE_READER_H
#define FILE_READER_H

class FileReader
{
public:
    virtual bool open(const std::string &path) = 0;
    virtual size_t read(char *buffer, size_t size) = 0;
    virtual void close() = 0;
    virtual ~FileReader() {}
};

#endif // FILE_READER_H