#include "File.h"
#include <iostream>
#include <cstring> // For std::strlen

// Implementation of readBytes
size_t File::readBytes(char *buffer, size_t length)
{
    std::cout << "Mock File readBytes called, length: " << length << std::endl;
    // Mock implementation: Just returning the requested length
    return length;
}

// Implementation of seek
void File::seek(uint16_t address)
{
    std::cout << "Mock File seek called, address: " << address << std::endl;
    // Mock implementation does nothing
}

// Implementation of close
void File::close()
{
    std::cout << "Mock File close called" << std::endl;
    // Mock implementation does nothing
}

// Implementation of operator bool
File::operator bool()
{
    std::cout << "Mock File operator bool called" << std::endl;
    return true; // Always return true for mock
}

// Implementation of println
void File::println(const char *message)
{
    std::cout << "Mock File println called, message: " << message << std::endl;
    // Mock implementation just prints the message
}
