/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
	Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
	Device            :  PIC18F14Q41
	Driver Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */

#pragma warning disable 520
#pragma warning disable 1498

#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "mcc_generated_files/mcc.h"

volatile uint32_t ticks = 0;
const char build_version[] = "MQTT Test Q41";
const char *build_date = __DATE__, *build_time = __TIME__;

char buffer[512], opbuffer[24];

static void tmr0isr(void);
static void delayisr(void);
uint32_t get_ticks(void);
void clear_ticks(void);
void wdtdelay(const uint32_t);

/*
			 Main application
 */
void main(void)
{
	uint32_t count = 0;
	// Initialize the device
	SYSTEM_Initialize();

	TMR0_SetInterruptHandler(tmr0isr);
	TMR1_SetInterruptHandler(delayisr);

	// Enable high priority global interrupts
	INTERRUPT_GlobalInterruptHighEnable();

	// Enable low priority global interrupts.
	INTERRUPT_GlobalInterruptLowEnable();

	TMR0_StartTimer();
	TMR1_StartTimer();

	while (true) {
		// Add your application code
		MLED_Toggle();
		wdtdelay(300000);
		if (UART1_is_tx_ready()) {
			/*
			 * format data to JSON
			 */
			snprintf(buffer, 510, "{\r\n     \"Qname\": \"%s\",\r\n     \"Qsequence\": %ld,\r\n     \"Qbuild_date\": \"%s\",\r\n     \"Qbuild_time\": \"%s\"\r\n}",
				build_version, count++, build_date, build_time);
			/*
			 * STDIO redirected to UART1
			 */
			printf("%s", buffer);
		}
	}
}

/*
 * check timer0 irq 1 second time
 */
static void tmr0isr(void)
{
	RLED_Toggle();
}

/*
 * check timer1 irq 500us time
 * update ticks
 */
static void delayisr(void)
{
	ticks++;
}

uint32_t get_ticks(void)
{
	static uint32_t tmp = 0;

	PIE3bits.TMR1IE = 0;
	tmp = (uint32_t) ticks;
	PIE3bits.TMR1IE = 1;
	return tmp;
}

void clear_ticks(void)
{
	PIE3bits.TMR1IE = 0;
	ticks = 0;
	PIE3bits.TMR1IE = 1;
}

/*
 * busy loop delay with WDT reset
 */
void wdtdelay(const uint32_t delay)
{
	uint32_t dcount;

	for (dcount = 0; dcount <= delay; dcount++) { // delay a bit
		ClrWdt(); // reset the WDT timer
	};
}
/**
 End of File
 */