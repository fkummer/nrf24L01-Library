#include "config.h"
#include <stdio.h>
#define	SYS_FREQ 64000000 // change the frequency of the clock
// radio library
#include "nrf24l01.h"
#define spi_divider 10

/* This code sends four arrays of different sizes using the 
 * dynamic payload length (dpl) feature of the radio. This file is the
 * transmitter and will only send data. The other file is the receiver and will
 * receive the data of each array and display it. This code is made to 
 * demonstrate sending multiple bytes at once and sending dynamic length 
 * payloads
 */

// set up the radio
void radioSetup() {
    nrf_setup(); // initializing function

    nrf_set_arc(0x05); // 5 retransmits
    
    nrf_set_ard(0x00); // 250 us between retransmission

    nrf_set_rf_ch(0x01); // freq = 2.401 GHz

    nrf_en_dpl(0);

    // set up addresses for autoack mode and dynamic payload length (dpl) mode
    // The tx address and the pipe 0 rx address must be the same for autoack mode
    // and dpl mode
    nrf_set_address_width(3);
    nrf_set_rx_addr(0, 0xAABBCC, 3);
    nrf_set_tx_addr(0xAABBCC);
}

// Fill an array with with consecutive numbers
// This is so there is something to look at on the receiving end.
void fillArray(char * array, int len){
    int i;
    for(i=0;i<len;i++){
        array[i] = i;
    }
}

void main(void) {
    INTEnableSystemMultiVectoredInt();
    radioSetup(); // setup the radio for this program
    //240x320 vertical display
    tft_setRotation(0); // Use tft_setRotation(1) for 320x240
    char array1;
    char array5[5];
    char array10[10];
    char array32[32];
    // Fill each array with consecutive incrementing numbers
    array1 = 1;
    fillArray(&array5,5);
    fillArray(&array10,10);
    fillArray(&array32,32);
    while (1) { // send each array one after the other
        while(!nrf_send_payload(&array1, 1));
        nrf_delay_ms(1000); // wait a second between sending each array
        while(!nrf_send_payload(&array5, 5));
        nrf_delay_ms(1000);
        while(!nrf_send_payload(&array10, 10));
        nrf_delay_ms(1000);
        while(!nrf_send_payload(&array32, 32));
        nrf_delay_ms(1000);
    }
} // main

// === end  ======================================================