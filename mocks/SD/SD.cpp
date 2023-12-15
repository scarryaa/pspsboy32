#include "SD.h" // Include the header file for the class

// Define the global SD object
SDClass SD;

// Implement the 'begin' method for SDClass
bool SDClass::begin(uint8_t csPin)
{
    std::cout << "SD.begin called with csPin: " << (int)csPin << std::endl;
    return true; // Mock implementation always succeeds
}

// Implement the overloaded 'begin' method
bool SDClass::begin(int a, SPIClass &spi, int b, const char c[1], int d)
{
    std::cout << "Overloaded SD.begin called" << std::endl;
    // Mock implementation does nothing
    return true; // Assuming success for the mock
}

// Implement the 'exists' method
bool SDClass::exists(const char *filepath)
{
    std::cout << "SD.exists called for filepath: " << filepath << std::endl;
    return true; // Mock implementation always returns true
}

// Implement the 'open' method
File SDClass::open(const char *filepath)
{
    std::cout << "SD.open called for filepath: " << filepath << std::endl;
    return File(); // Return a mock File object
}

// Implement the overloaded 'open' method with mode
File SDClass::open(const char *filepath, uint8_t mode)
{
    std::cout << "Overloaded SD.open called for filepath: " << filepath << ", mode: " << (int)mode << std::endl;
    return File(); // Return a mock File object
}

// Implement the 'end' method
void SDClass::end()
{
    std::cout << "SD.end called" << std::endl;
    // Mock implementation does nothing
}
