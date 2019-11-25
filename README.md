# SRAM_23LC

This is a driver for Microchip Technology Inc. 23LC (23LCV) SPI SRAM chips for any architecture on the
Arduino platform, including AVR, SAM3X (Due), SAM M0+ (SAMD, SAML, SAMC), and STM32 microcontrollers.

## Supported Chips

* 128KB
  * 23LCV1024
  * 23LC1024, 23A1024
* 64KB
  * 23LCV512
  * 23LC512, 23A512
* 32KB
  * 23A256, 23K256
* 8KB
  * 23A640, 23K640

## Startup

Call the constructor with the SPI bus, chip select, and device. Devices are defined
in SRAM_23LC.h and have names like SRAM_23LCV1024. More than one device can be instantiated.

```
SRAM_23LC SRAM(&SPI, 27, SRAM_23LCV1024);
SRAM_23LC SRAM1(&SPI, 23, SRAM_23LC512);
```

Call begin(). Without parameters, begin() uses the default speed for this library (12MHz
for samd, 14MHz for sam, and 4MHz for avr). Note that SPI transaction support is required.

```
SRAM.begin();
SRAM1.begin(8000000UL);      // or specify SPI clock speed in Hz
```

Call end() to set the chip select pin back to INPUT.

```
SRAM.end();
```

## Byte Transfers

Chips with 1024Kbits use 24-bit addresses. Chips with 512Kbits or less use 16-bit addresses.

Call readByte with the address to read one byte. Returns 0 if address >= capacity.
Because valid data could also be 0, use readBlock() to catch this error.

```
uint32_t address = 0x000A;
uint8_t byte = SRAM.readByte(address);
```

Call writeByte with the address and the byte to write. Returns 0 if address >= capacity,
otherwise, returns 1.

```
uint32_t address = 0x000A;
uint8_t byte = 0x1F;
size_t ret = SRAM.writeByte(address, byte);
```

## Block Transfers

Chips with 1024Kbits use 24-bit addresses. Chips with 512Kbits or less use 16-bit addresses.

Call readBlock() with the address, length to read, and a pointer to the buffer used to
store the data read. Any length can be used, limited by the size of the buffer. If the length
exceeds the last address, it will wrap to address 0. Note that this method knows nothing about
the size of the buffer used, so ensure length is not set too high or a buffer overflow will
occur. Returns 0 if address >= capacity or length = 0.

```
#define START_ADDRESS   0x001A
#define BUFFER_SIZE     64
uint8_t buffer[BUFFER_SIZE];
size_t ret = SRAM.readBlock(START_ADDRESS, BUFFER_SIZE, buffer);        // entire buffer
size_t ret1 = SRAM1.readBlock(START_ADDRESS, 4, &buffer[16]);           // partial buffer
```

Call writeBlock() with the address, length to write, and a pointer to the source buffer used
for the data to be written. Any length can be used, limited by the size of the buffer. If the
length exceeds the last address, it will wrap to address 0. Note that this method knows nothing
about the size of the buffer used, so ensure length is not set too high or data will be read
past the end of the buffer. Returns 0 if address >= capacity or length = 0.

```
size_t ret = SRAM.writeBlock(START_ADDRESS, BUFFER_SIZE, buffer);
```

## Possible Future Additions/Changes

* Optimizations
* DMA support
* DSPI, QSPI? Would need to do some of this in software.
* Page mode or Byte mode?
* Use Stream class? It probably doesn't make sense with a storage device (with addresses).

## Changelog

v1.1.2 - Fixed bad datatype on avr architecture (thanks @mataide). Changed architectures supported to match all (thanks @matthijskooijman). Fixed keywords.txt highlighting (thanks @per1234).
v1.1.1 - Fixed compilation error on avr architecture.
v1.1.0 - Fixed inability to set addresses higher than 8-bits. Updated example sketchfor better testing.
v1.0.0 - Initial release.

## License

Copyright (c) 2017-2019, Justin Mattair (justin@mattair.net)

Permission to use, copy, modify, and distribute this software
and its documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appear in all
copies and that both that the copyright notice and this
permission notice and warranty disclaimer appear in supporting
documentation, and that the name of the author not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

The author disclaim all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall the author be liable for any
special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.
