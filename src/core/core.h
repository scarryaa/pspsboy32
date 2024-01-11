#ifndef CORE_H
#define CORE_H

#include "./cpu/cpu.h"
#include "./ppu/ppu.h"
#include "./memory/memory.h"
#include "./timer/timer.h"
#include "./cartridge/cartridge.hpp"

const int CPU_FREQUENCY = 4194304; // 4.194304 MHz
const int FRAME_RATE = 60;         // 60 Hz
const int CYCLES_PER_FRAME = CPU_FREQUENCY / FRAME_RATE;

struct Core
{
public:
    typedef enum
    {
        Right = 0,
        Left = 1,
        Up = 2,
        Down = 3,
        A = 4,
        B = 5,
        Select = 6,
        Start = 7
    } Button;

    Core();
    ~Core();

    int getCpuTime();
    void setCpuTime(int time);
    double elapsedTime();
    void init();
    uint8_t run();
    void shutdown();
    uint8_t *getFrameBuffer();
    uint8_t *getDebugFrameBuffer();

    bool isFrameReady();
    void resetFrameReady();
    bool isGameLoaded();
    bool isRunning();

    void reset();
    bool loadRom(const std::string &path);

    uint8_t getButtonState();
    void setButtonState(Button button, bool pressed);
    uint8_t getJoypadState() const;

private:
    PPU ppu;
    Memory memory;
    CPU cpu;
    Timer timer;
    uint8_t joypadState;
    Cartridge cartridge;
};

#endif // CORE_H