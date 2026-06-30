| Component       | Model         | Price (EUR)      | Datasheet                                                         |
| --------------- | ------------- | ---------------- | ----------------------------------------------------------------- |
| Microcontroller | Nucleo-H723ZG | (already had it) | [[um2407-stm32h7-nucleo144-boards-mb1364-stmicroelectronics.pdf]] |
| DAC             | PCM5102A      | ~11              | [[pcm5102a_dac_schematic.pdf]]                                    |
## Nucleo-H723ZG
The Microcontroller used to make the DAC work with the clock configuration, I2S communication and DMA. I use it for now because it is the only STM32 board that I had.
## PCM5102A
A breakout board with already a jack, used to test the audio for now - will replace it with a SMD later.