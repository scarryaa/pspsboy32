#include <iostream>

#ifndef _FILE_H_
#define _FILE_H_

class File
{
public:
    size_t readBytes(char *buffer, size_t length);

    void seek(uint16_t address);

    void close();

    operator bool();

    void println(const char *message);
};

#endif // _FILE_H_