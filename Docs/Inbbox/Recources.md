M2 interface:
https://www.delock.com/infothek/M.2_2022/M.2_e.html
### 1. The Clock Tree
Your background as an electronics technician is your biggest advantage here. You already know about oscillators, crystals, and clock signals. On an STM32, the "Clock Tree" is just a complex network of frequency dividers and multipliers.
- **What to look for:** Search for the **STM32 H7 Reference Manual (RM0468)**. Go to the "Reset and Clock Control (RCC)" chapter.
- **The Resource:**
- **How to learn:** Open CubeMX in STM32CubeIDE. Go to the **Clock Configuration** tab. Click around and change the "Input Frequency." Watch how the frequencies change. It is essentially a visual calculator.
- **The "Pro" Concept:** Look for **PLL (Phase Locked Loop)**. Learn how it takes a low-frequency crystal (e.g., 25MHz) and multiplies it up to 550MHz.
### 2. DMA (Direct Memory Access)
DMA is just a "copy-paste" engine that lives inside the chip. When you move audio, the CPU says, "DMA, move 1000 bytes from this memory address to this I2S peripheral," and the CPU goes back to sleep while the DMA does the heavy lifting.

- **The Resource:** Search for **"STM32 DMA Controller AN4031"**. This is an official ST Application Note specifically for DMA. It is the "Bible" for this topic.
    
- **The Visualizer:** Think of it like a train station.
    - **Source:** The memory (SD card data).
    - **Destination:** The I2S peripheral.
    - **The Train:** The DMA controller.
    - **The Track:** The Bus Matrix (the internal highways of the chip).
- **Key Concept:** Research **"Circular Buffer"** or **"Ping-Pong Buffering."** This is how you keep audio playing forever without the CPU ever stopping.
### 3. Cache Coherency (The H7 specialty)
This is the most advanced topic. Because the H7 is so fast, it keeps a "copy" of the RAM inside its own internal cache (the L1 Cache).
- **The Problem:** The CPU might update data in its _internal_ cache, but the DMA is reading directly from the _external_ RAM. The DMA doesn't know the CPU changed the data, so it sends the "old" version to your headphones.
- **The Solution:** **Cache Invalidation** and **Cache Cleaning.** You have to tell the CPU: "I just finished writing to RAM, please push that to the physical memory so the DMA can see it."
- **The Resource:** Search for **"AN4838: STM32 microcontroller system memory access alignment and cache"**. This document explains exactly how to handle this.
- **The "Cheat Code":** In the H7, you can designate a specific area of memory as **"Non-Cacheable"** using the MPU (Memory Protection Unit). For your audio buffers, you can simply tell the MPU: "Do not cache this area." Then you never have to worry about cache coherency again.