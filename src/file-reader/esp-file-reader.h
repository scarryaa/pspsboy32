#include "FS.h"
#include "file-reader.h"
#include "SPIFFS.h"

class ESPFileReader : public FileReader
{
    File file;

public:
    bool open(const std::string &path) override
    {
        file = SPIFFS.open(path.c_str(), "r");
        return file;
    }

    size_t read(char *buffer, size_t size) override
    {
        return file.readBytes(buffer, size);
    }

    void close() override
    {
        if (file)
        {
            file.close();
        }
    }
};
