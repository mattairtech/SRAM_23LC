/*
 * Driver for Microchip Technology Inc. 23LC (23LCV) SPI SRAM chips for
 * AVR, SAM3X (Due), and SAM M0+ (SAMD, SAML, SAMC) microcontrollers
 * 
 * Copyright (c) 2017, Justin Mattair (justin@mattair.net)
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

#include "SRAM_23LC.h"

SRAM_23LC::SRAM_23LC(SPIClass * spi, const uint8_t chipSelect, const SRAM_23LC_Device device)
{
  if ((device == SRAM_23LCV1024) || (device == SRAM_23LC1024) || (device == SRAM_23A1024)) {
    _capacity = 0x20000;
  } else if ((device == SRAM_23LCV512) || (device == SRAM_23LC512) || (device == SRAM_23A512)) {
    _capacity = 0x10000;
  } else if ((device == SRAM_23A256) || (device == SRAM_23K256)) {
    _capacity = 0x8000;
  } else if ((device == SRAM_23A640) || (device == SRAM_23K640)) {
    _capacity = 0x2000;
  }

  _device = device;
  _spi = spi;
  _chipSelect = chipSelect;
  _spiSettings = SPISettings();	// use default settings
}

void SRAM_23LC::begin(void)
{
#if defined(ARDUINO_ARCH_SAMD)
  begin(12000000UL);
#elif defined(ARDUINO_ARCH_AVR)
  begin(4000000UL);
#elif defined(ARDUINO_ARCH_SAM)
  begin(14000000UL);
#else
  begin(4000000UL);
#endif
}

void SRAM_23LC::begin(const uint32_t clkSpeed)
{
  digitalWrite(_chipSelect, HIGH);
  pinMode(_chipSelect, OUTPUT);

  _clkSpeed = clkSpeed;
  _spiSettings = SPISettings(_clkSpeed, MSBFIRST, SPI_MODE0);

  _spi->begin();

  startCommand(SRAM_23LC_COMMAND_WRMR, 0);
  _spi->transfer(SRAM_23LC_MODE_SEQUENTIAL);
  endCommand();
}

void SRAM_23LC::end(void)
{
  pinMode(_chipSelect, INPUT);
}

uint8_t SRAM_23LC::readByte(const uint32_t address)
{
  if (address >= _capacity) {
    return(0);
  }

  startCommand(SRAM_23LC_COMMAND_READ, address);
  uint8_t ret = _spi->transfer(SRAM_23LC_DUMMY_BYTE);
  endCommand();

  return(ret);
}

size_t SRAM_23LC::writeByte(const uint32_t address, const uint8_t byte)
{
  if (address >= _capacity) {
    return(0);
  }

  startCommand(SRAM_23LC_COMMAND_WRITE, address);
  _spi->transfer(byte);
  endCommand();
  return(1);
}

size_t SRAM_23LC::readBlock(const uint32_t address, const size_t length, void * buffer)
{
  if (address >= _capacity || length == 0) {
    return(0);
  }

  startCommand(SRAM_23LC_COMMAND_READ, address);
  _spi->transfer(buffer, length);
  endCommand();

  return(length);
}

size_t SRAM_23LC::writeBlock(const uint32_t address, const size_t length, void * buffer)
{
  uint8_t *buf = reinterpret_cast<uint8_t *>(buffer);
  size_t len = length;

  if (address >= _capacity || length == 0) {
    return(0);
  }

  startCommand(SRAM_23LC_COMMAND_WRITE, address);
  while (len--) {
    _spi->transfer(*buf);
    buf++;
  }
  endCommand();

  return(length);
}

void SRAM_23LC::startCommand(const uint8_t command, const uint32_t address)
{
  _spi->beginTransaction(_spiSettings);
  digitalWrite(_chipSelect, LOW);

  _spi->transfer(command);
  if (command == SRAM_23LC_COMMAND_READ || command == SRAM_23LC_COMMAND_WRITE) {
    sendAddressBytes(address);
  }
}

void SRAM_23LC::sendAddressBytes(const uint32_t address)
{
  if (_capacity > 0x10000) {
    _spi->transfer((uint8_t)((address >> 16) & 0xFF));
  }
  _spi->transfer((uint8_t)((address >> 8) & 0xFF));
  _spi->transfer((uint8_t)(address & 0xFF));
}

void SRAM_23LC::endCommand(void)
{
  digitalWrite(_chipSelect, HIGH);
  _spi->endTransaction();
}
