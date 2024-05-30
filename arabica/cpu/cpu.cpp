#include <arabica/cpu/cpu.hpp>

int registers[REGISTER_COUNT];

void init_cpu()
{
    // TODO: Verify if the machine actually do that
    for (int i = 0; i < REGISTER_COUNT; i++)
    {
        registers[i] = 0;
    }
}

void run_cpu()
{
    // TODO: Implementation
}