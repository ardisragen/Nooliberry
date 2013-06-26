/*
 * Copyright (c) 2011, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \file
 *         Nooliberry platform tests program
 * \author
 *         Olivier Debreu <olivier.debreu@noolitic.biz>
 */
#include "contiki.h"
#include "ds2411.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "dev/slip.h"
#include <string.h>
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "net/rpl/rpl.h"

#define DEBUG DEBUG_FULL
#include "net/uip-debug.h"
#include "cmd.h"

#include "packetutils.h"

#ifdef SLIP_RADIO_CONF_SENSORS
extern const struct slip_radio_sensors SLIP_RADIO_CONF_SENSORS;
#endif

void slip_send_packet(const uint8_t *ptr, int len);

static uint8_t serialNumber[6] = {0};
	
static uint8_t isSerialRead = 0;
	
#define UDP_REMOTE_PORT 2000
#define UDP_LOCAL_PORT 2000

static struct uip_udp_conn *slot_udp;
static uip_ipaddr_t server_ipaddr;

 /* max 16 packets at the same time??? */
uint8_t packet_ids[16];
int packet_pos;

/*---------------------------------------------------------------------------*/

static void sendTestPacket()
{
	char buffer[] = "Nooliberry test";
	uip_udp_packet_send(slot_udp, buffer, strlen(buffer));
	printf("packet sent\n");
}

/*---------------------------------------------------------------------------*/

static void initUDP()
{
	uip_ip6addr(&server_ipaddr, 0xff02, 0, 0, 0, 0, 0, 0, 0x1);
	
	slot_udp = udp_new(&server_ipaddr, UIP_HTONS(UDP_REMOTE_PORT), NULL); 
	
	if(slot_udp == NULL) {
		printf("No UDP connection available, exiting the process!\n");
		return;
	}
	else
		printf("init UDP OK\n");
	
	udp_bind(slot_udp, UIP_HTONS(UDP_LOCAL_PORT)); 
}

/*---------------------------------------------------------------------------*/
static void readSerial()
{
	if(ds2411_read(serialNumber))  /* if serial available*/
	{
		printf("Serial number read OK\n");
		isSerialRead = 1;
	}		
	else
	{
		printf("Error during serial number read\n");
		isSerialRead = 0;
	}		
}

static void printSerial()
{
	int i;
	if(isSerialRead)
	{
		for(i=0; i<5; i++)
			printf("%x-", serialNumber[i]);
			
		printf("%x\n", serialNumber[5]);
	}
	else
		printf("Could not read serial number\n");
}	
  
/*---------------------------------------------------------------------------*/
PROCESS(nooliberry_test_process, "Nooliberry test process");
AUTOSTART_PROCESSES(&nooliberry_test_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(nooliberry_test_process, ev, data)
{
	static struct etimer et;
	PROCESS_BEGIN();

	NETSTACK_RDC.off(1);

	printf("Nooliberry tests program started...\n");

	initUDP();
	
	if(slot_udp == NULL)
		PROCESS_EXIT();
	
	readSerial();
	printf("serial number: ");
	printSerial();
	
	etimer_set(&et, CLOCK_SECOND * 3);

	while(1) {
		PROCESS_YIELD();

		if(etimer_expired(&et)) {
			etimer_reset(&et);
			printf("UART emission test\n");
			
			sendTestPacket();
			
		}
	}
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/

