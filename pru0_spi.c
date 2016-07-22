/*
 *Written by Vaibhav Choudhary under GSOC-2016 for BeagleBoard.org
 *Copyright (C) 2016 Vaibhav Choudhary -www.vaibhavchoudhary.com
 *
 *This code may be copied and/or modified freely according to GNU General Public  
 *License version 2 as published by the Free Software Foundation, provided   
 *the following conditions are also met:
 *1) Redistributions/adaptions of source code must retain this copyright
 *   notice on the top, giving credit to the original author, along with 
 *   this list of conditions.
 *
 *2) Redistributions in binary form, compiled from this source code and/or 
 *   modified/adapted versions of this source code, must include this copyright 
 *   notice giving credit to the original author, along with this list of conditions 
 *   in the documentation and other materials provided with the
 *   distribution.
 *
 *3) The original author shall not held for any loss arising from using this code.
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
 */
#include <stdint.h>
#include "resource_table_empty.h"
#include <pru_cfg.h>
#define   P8_11  15
#define   CLK  14
volatile register uint32_t __R30;
//volatile register uuint8_t32_t __R31
uint8_t CPOL = 0, CPHA = 0;
uint8_t i = 0;

void main()
{
	//Declare a pointer to the mem location in the shared mem
	volatile uint8_t *mosi_transfer = (volatile uint8_t *)0x00010000;
	//volatile uint8_t *miso_transfer;
	volatile uint8_t mosi, miso;
	//Set the CFG Register to direct output
	CT_CFG.GPCFG0 = 0;
	//clear the r30 register
	__R30 = 0x0000;
	//setmosi to test data

	while (1) {
		mosi = *mosi_transfer;
		CPOL = 0;
		CPHA = 0;
		//set value of the clock_pol_pha variable between 0 and 3
		uint8_t clock_pol_pha = (CPOL * 2) + CPHA;
		if (clock_pol_pha >= 2) {
			//set the clock 1 to be in its idle state
			__R30 |= (1 << CLK);
		}
		switch (clock_pol_pha) {
		case 0:

			for (i = 0; i < 8; i++) {
				//Write Data on MOSI pin with LSB being first transferred
				if ((mosi << (7 - i)) & 0x80) {
					__R30 |= (1 << P8_11);
				} else {	//This generates 0 at the specified bit and 1 at all others
					__R30 &= ~(1 << P8_11);
				}

				//set clk 1
				__R30 |= (1 << CLK);

				//set clk 0
				__R30 &= ~(1 << CLK);
			}
			break;
		case 1:
			for (i = 0; i < 8; i++) {
				//Write Data on MOSI pin with LSB being first transferred
				if ((mosi << (7 - i)) & 0x80) {
					__R30 |= (1 << P8_11);
				} else {
					__R30 &= ~(1 << P8_11);
				}

				//set clk 1
				__R30 |= (1 << CLK);
				//set clk 0
				__R30 &= ~(1 << CLK);

			}
			break;

		case 2:
			for (i = 0; i < 8; i++) {

				//Write Data on MOSI pin with LSB being first transferred
				if ((mosi << (7 - i)) & 0x80) {
					__R30 |= (1 << P8_11);
				} else {
					__R30 &= ~(1 << P8_11);
				}

				//set clk 0 to set it in active state
				__R30 &= ~(1 << CLK);
				//set clk 1 to idle state
				__R30 |= (1 << CLK);

			}
			break;
		case 3:
			for (i = 0; i < 8; i++) {
				//Write Data on MOSI pin with LSB being first transferred
				if ((mosi << (7 - i)) & 0x80) {
					__R30 |= (1 << P8_11);
				} else {
					__R30 &= ~(1 << P8_11);
				}
				//set clk 0 to set it in active state
				__R30 &= ~(1 << CLK);
				//set clk 1 to idle state
				__R30 |= (1 << CLK);

			}

			break;
		default:
			break;

		}
	}
	__halt();
}