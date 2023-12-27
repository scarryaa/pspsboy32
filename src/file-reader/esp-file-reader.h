#ifndef __ESP_FILE_READER_H__
#define __ESP_FILE_READER_H__

#include "FS.h"
#include "file-reader.h"
#include "SD.h"
#include <string>

class ESPFileReader : public FileReader
{
    File file;

public:
    bool open(const std::string &path) override
    {
        file = SD.open(path.c_str());
        return file;
    }

    size_t read(char *buffer, size_t size) override
    {
        Serial.print("read: ");
        Serial.println(size);
        return file.readBytes(buffer, size);
    }

    void write(std::string value) override
    {
        Serial.print(value.c_str());
        if (file)
        {
            file.print(value.c_str());
        }
    }

    void writeLine(std::string value) override
    {
        Serial.println(value.c_str());
        if (file)
        {
            file.println(value.c_str());
        }
    }

    void close() override
    {
        if (file)
        {
            file.close();
        }
    }
};

#endif // __ESP_FILE_READER_H__
