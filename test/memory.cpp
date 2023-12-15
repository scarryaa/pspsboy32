#include <unity.h>
#include "../src/memory/memory.h"

void test_memory_read_write_byte(void)
{
    Memory memory;

    uint16_t address = 0x1234;
    uint8_t value = 0xAB;

    memory.writeByte(address, value);
    uint8_t readValue = memory.readByte(address);

    TEST_ASSERT_EQUAL_UINT8(value, readValue);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_memory_read_write_byte);
    UNITY_END();

    return 0;
}