#pragma once

#define REGISTER_COUNT 16

/**
 * Initialize the CPU, including the registers
 */
void init_cpu();

/**
 * Main loop for the machine.
 */
void run_cpu();