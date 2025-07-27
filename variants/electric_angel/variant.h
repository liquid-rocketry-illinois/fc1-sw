/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VARIANT_FEATHER_M4_
#define _VARIANT_FEATHER_M4_

// The definitions here needs a SAMD core >=1.6.10
#define ARDUINO_SAMD_VARIANT_COMPLIANCE 10610

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Frequency of the board main oscillator */
// #define VARIANT_MAINOSC		(32768ul)

/** Master clock frequency */
#define VARIANT_MCK        (F_CPU)
#define VARIANT_GCLK1_FREQ (48000000UL)

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "WVariant.h"

#ifdef __cplusplus
#include "SERCOM.h"
#include "Uart.h"
#endif // __cplusplus

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

// Number of pins defined in PinDescription array
#define PINS_COUNT           (34u)

/*
 * Analog pins
 * These must be defined for compilation
 */
#define PIN_A0               (0)
#define PIN_A1               (PIN_A0)
#define PIN_A2               (PIN_A0)
#define PIN_A3               (PIN_A0)
#define PIN_A4               (PIN_A0)
#define PIN_A5               (PIN_A0)
#define PIN_A6               (PIN_A0)

#define PIN_DAC0             (0)
#define PIN_DAC1             PIN_A1

static const uint8_t A0  = PIN_A0;
static const uint8_t A1  = PIN_A1;
static const uint8_t A2  = PIN_A2;
static const uint8_t A3  = PIN_A3;
static const uint8_t A4  = PIN_A4;
static const uint8_t A5  = PIN_A5;
static const uint8_t A6  = PIN_A6;

/*
 * SPI Interfaces
 * Unfortunately all of these have to be defined for stuff to compile, even though
 * this program doesnt use the default SPI variable.
 */
#define SPI_INTERFACES_COUNT 1

#define PIN_SPI_MISO         (0u)
#define PIN_SPI_MOSI         (0u)
#define PIN_SPI_SCK          (0u)
#define PERIPH_SPI           sercom1
#define PAD_SPI_TX           SPI_PAD_3_SCK_1
#define PAD_SPI_RX           SERCOM_RX_PAD_2

static const uint8_t SS	  = 0;	// SERCOM1 last PAD is present on d9 but HW SS isn't used. Set here only for reference.
static const uint8_t MOSI = 0;
static const uint8_t MISO = 0;
static const uint8_t SCK  = 0;

/*
 * Wire Interfaces
 * Same deal as with SPI
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA         (0u)
#define PIN_WIRE_SCL         (0u)
#define PERIPH_WIRE          sercom2

static const uint8_t SDA = 0;
static const uint8_t SCL = 0;

/*
 * USB
 */
#define PIN_USB_HOST_ENABLE (0ul)
#define PIN_USB_DM          (29ul)
#define PIN_USB_DP          (30ul)

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus

/*	=========================
 *	===== SERCOM DEFINITION
 *	=========================
*/
extern SERCOM sercom0;
extern SERCOM sercom1;
extern SERCOM sercom2;
extern SERCOM sercom3;
extern SERCOM sercom4;
extern SERCOM sercom5;

#endif

#define SERIAL_PORT_USBVIRTUAL      Serial
#define SERIAL_PORT_MONITOR         Serial

#endif /* _VARIANT_FEATHER_M4_ */

