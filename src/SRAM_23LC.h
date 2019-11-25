/*
 * Driver for Microchip Technology Inc. 23LC (23LCV) SPI SRAM chips for
 * AVR, SAM3X (Due), and SAM M0+ (SAMD, SAML, SAMC) microcontrollers
 * 
 * Copyright (c) 2017-2019, Justin Mattair (justin@mattair.net)
 * 
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for any purpose and without fee is hereby
 * granted, provided that the above copyright notice appear in all
 * copies and that both that the copyright notice and this
 * permission notice and warranty disclaimer appear in supporting
 * documentation, and that the name of the author not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 *
 * The author disclaim all warranties with regard to this
 * software, including all implied warranties of merchantability
 * and fitness.  In no event shall the author be liable for any
 * special, indirect or consequential damages or any damages
 * whatsoever resulting from loss of use, data or profits, whether
 * in an action of contract, negligence or other tortious action,
 * arising out of or in connection with the use or performance of
 * this software.
 */

#ifndef SRAM_23LC_h_
#define SRAM_23LC_h_

#include "Arduino.h"
#include <SPI.h>

#if (!defined(SPI_HAS_TRANSACTION) || SPI_HAS_TRANSACTION == 0)
#error "SRAM_23LC library requires SPI library transaction support (SPI_HAS_TRANSACTION = 1)"
#endif


typedef enum _SRAM_23LC_Device
{
// 128KB
  SRAM_23LCV1024=0,
  SRAM_23LC1024,
  SRAM_23A1024,
// 64KB
  SRAM_23LCV512,
  SRAM_23LC512,
  SRAM_23A512,
// 32KB
  SRAM_23A256,
  SRAM_23K256,
// 8KB
  SRAM_23A640,
  SRAM_23K640,
} SRAM_23LC_Device;

#define SRAM_CAPACITY_23LCV1024                 0x20000
#define SRAM_CAPACITY_23LC1024                  0x20000
#define SRAM_CAPACITY_23A1024                   0x20000
#define SRAM_CAPACITY_23LCV512                  0x10000
#define SRAM_CAPACITY_23LC512                   0x10000
#define SRAM_CAPACITY_23A512                    0x10000
#define SRAM_CAPACITY_23A256                    0x8000
#define SRAM_CAPACITY_23K256                    0x8000
#define SRAM_CAPACITY_23A640                    0x2000
#define SRAM_CAPACITY_23K640                    0x2000

#define SRAM_23LC_ADDRESS_16BIT			0
#define SRAM_23LC_ADDRESS_24BIT			1

#define SRAM_23LC_DUMMY_BYTE			0xFF

// Commands supported by all chips
#define SRAM_23LC_COMMAND_READ			0x03
#define SRAM_23LC_COMMAND_WRITE			0x02
#define SRAM_23LC_COMMAND_RDMR			0x05
#define SRAM_23LC_COMMAND_WRMR			0x01

// Additional commands supported by some chips
#define SRAM_23LC_COMMAND_EDIO			0x3B
#define SRAM_23LC_COMMAND_EQIO			0x38
#define SRAM_23LC_COMMAND_RSTIO			0xFF


typedef union {
    struct {
        uint8_t  MODE:2;
        uint8_t  :6;		// reserved
    } bit;
    uint32_t word;
} SRAM_23LC_Mode_Register;

#define SRAM_23LC_MODE_Pos			6
#define SRAM_23LC_MODE_Msk			(0x3ul << SRAM_23LC_MODE_Pos)
#define SRAM_23LC_MODE(value)			(SRAM_23LC_MODE_Msk & ((value) << SRAM_23LC_MODE_Pos))
#define   SRAM_23LC_MODE_BYTE_Val		0x0ul
#define   SRAM_23LC_MODE_SEQUENTIAL_Val		0x1ul
#define   SRAM_23LC_MODE_PAGE_Val		0x2ul
#define SRAM_23LC_MODE_BYTE			(SRAM_23LC_MODE_BYTE_Val << SRAM_23LC_MODE_Pos)
#define SRAM_23LC_MODE_SEQUENTIAL		(SRAM_23LC_MODE_SEQUENTIAL_Val << SRAM_23LC_MODE_Pos)
#define SRAM_23LC_MODE_PAGE			(SRAM_23LC_MODE_PAGE_Val << SRAM_23LC_MODE_Pos)


class SRAM_23LC
{
  public:
    SRAM_23LC(SPIClass * spi, const uint8_t chipSelect, const SRAM_23LC_Device device);

    void begin(void);
    void begin(const uint32_t clkSpeed);
    void end(void);

    //uint8_t getMode(void);
    //void setMode(uint8_t mode);

    uint8_t readByte(const uint32_t address);
    size_t writeByte(const uint32_t address, const uint8_t byte);

    size_t readBlock(const uint32_t address, const size_t length, void * buffer);
    size_t writeBlock(const uint32_t address, const size_t length, void * buffer);

  protected:
    void startCommand(const uint8_t command, const uint32_t address);
    void endCommand(void);

  private:
    SPIClass * _spi;
    SPISettings _spiSettings;
    SRAM_23LC_Device _device;
    uint32_t _capacity;
    uint8_t _chipSelect;
    uint32_t _clkSpeed;

    void sendAddressBytes(const uint32_t address);
};

#endif

