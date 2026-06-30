## DAC's
3 DACs to make the customisability of the dap have a purpose:

| model      | requirements                                                                                                                                    | Pins                                                         | price | Features                                                                             | pdf | url |
| ---------- | ----------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------ | ----- | ------------------------------------------------------------------------------------ | --- | --- |
| PCM5102A   | I2S and DMA                                                                                                                                     | Power, Ground, I2S Out (Data, Word Clock, Bit Clock)         |       | continuous data stream                                                               |     |     |
| TI PCM5242 | I2S, I2C and maybe ROTS?                                                                                                                        | I2S Out, I2C (`SDA`, `SCL`), Reset Pin                       |       | internal DSP, internal volume calculations and internal frequancy filters (bass ect) |     |     |
| WM8960     | full-duplex DMA,<br>simultaneously reading an I2S stream _from_ the microphone, saving it to the SD card, while maintaining the rest of the OS. | I2S Out (Playback), I2S In (Recording), I2C (Configuration). |       | internal microphone with - dictaphone                                                | Q   |     |

## M2 connector
**1. Power Delivery**
- **`3.3V_DIGITAL` & `GND`**
- **`5V_ANALOG`** DACs sound much better if their analog amplifiers run on a higher, cleaner voltage separated from the digital 3.3V line.

**2. The I2S Bus**
- **`I2S_BCLK` (Bit Clock)** 
- **`I2S_LRCK` (Left/Right Word Clock):** Tells the DAC which audio channel is currently being sent.
- **`I2S_DATA_OUT`:** Sends audio from the STM32 to the DAC.
- **`I2S_DATA_IN`:** Receives audio from a microphone (for Module 3).
- **`MCLK` (Master Clock)**

**3. The I2C Bus**
- **`I2C_SDA` (Data)** & **`I2C_SCL` (Clock)**

**4. Utility & Auto-Detection**
- **`HW_RESET`:** An output pin from the STM32 to reboot a frozen DAC module.
- **`ID_0` & `ID_1`:** Two input pins on the STM32 with internal pull-up resistors. The daughterboards connect these to Ground in different combinations (00, 01, 10, 11) so the STM32 knows exactly which of the 3 modules was just plugged in.

**5. Audio return path**
- ANALOG_LEFT
- ANALOG_RIGHT
- AGND - Analog Ground
must put the analog pins on the opposite side of the M.2 connector from the digital pins, and surround them with an Analog Ground plane.