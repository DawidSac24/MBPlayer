## DAC pinout
![[DAC_pinout_datasheet.png]]
From the [[pcm5102a_dac_schematic.pdf]] datasheet, the H3L should be soldered to 3v3 in order to connect the `XSMT` pin to 3V3 (allowing to soft un-mute the output). I had to solder these H1L - H4L and SCK jumper since none of theme where actually soldered (every pin on GND except for H3L).
## DAC to microcontroller connection
Following the [H7 datasheet](um2407-stm32h7-nucleo144-boards-mb1364-stmicroelectronics.pdf) and using the Cube Ide, I chose the i2S1 Connection with Half-Duplex Master Transmit.

| dac pin | nucleo pin name                           | nucleo pin |
| ------- | ----------------------------------------- | ---------- |
| LRCK    | I2S1_WS                                   | PA4        |
| BCK     | I2S1_CK                                   | PA5        |
| DIN     | I2S1_SDO                                  | PB5        |
| SCK     | 'GND' - already connected with the jumper | -          |
## Code
[Commit: DAC tests](https://github.com/DawidSac24/MBPlayer/commit/889a6f0ee48fb1a38c644f4c80249f3afa6b31bf)
Made simple test program, added a converted 5sec wav sample into code. The DAC outputs the audio but it seems accelerated event though the audio frequency should match the one from original. file haven't found any other good sample files to test it correctly (not enough flash size for bigger wav files and I didn't found small enough ones). Because of that, I cannot test correctly the DMA so I decided to just go to the next iteration, getting the flash connector to test it directly on wav files instead of hardcoded samples.