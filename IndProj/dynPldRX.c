#include "config.h"
#include "tft_gfx.h"
#include "tft_master.h"
#include <stdio.h>
#define	SYS_FREQ 64000000 // change the frequency of the clock
#include "nrf24l01.h"
// These two defines are for the tft
#define spi_channel	1
#define spi_divider 10

/* This code sends four arrays of different sizes using the 
 * dynamic payload length (dpl) feature of the radio. This file is the
 * receiver and will receive arrays of varying size and display them. The other
 * file will create and send these arrays.
 */

// set up the radio
void radioSetup() {
    nrf_setup(); // initializing function

    nrf_set_rf_ch(0x01); // freq = 2.401 GHz

    nrf_en_dpl(1); // enable dpl for pipe 1

    // set up addresses for autoack mode and dynamic payload length (dpl) mode
    // The radio will receive on pipe 1
    nrf_set_address_width(3);
    nrf_set_rx_addr(1, 0xAABBCC, 3);
}
void main(void) {
    INTEnableSystemMultiVectoredInt();
    radioSetup(); // setup the radio for this program
    tft_init_hw(); // setup for tft
    tft_begin(); // setup for tft
    tft_fillScreen(ILI9340_BLACK);
    //240x320 vertical display
    tft_setRotation(0); // Use tft_setRotation(1) for 320x240
    char buffer[120]; // a buffer for writing to the tft
    // arrays to read payloads into
    char array[32];
    int width; // the width of the received payload
    while (1) {
        tft_setTextColor(ILI9340_GREEN);
        tft_setTextSize(2);
        tft_setCursor(0, 0);
        sprintf(buffer, "%s", "Waiting for data...");
        tft_writeString(buffer);
        // enter receive mode to search for packets
        nrf_state_rx_mode();
        if(nrf_payload_available()){ // wait for a payload to be received
            width = nrf_get_width(); // get the width of the payload
            nrf_get_payload(&array, width); // read the payload into the array
            tft_fillScreen(ILI9340_BLACK); // clear the display
            
            // display the received data on the tft
            tft_setTextColor(ILI9340_BLUE);
            tft_setTextSize(2);
            tft_setCursor(0, 20);
            sprintf(buffer, "%s", "Received data:");
            tft_writeString(buffer);
            tft_setCursor(40, 60);
            tft_setTextColor(ILI9340_RED);
            sprintf(buffer, "%s", "Width:");
            tft_writeString(buffer);
            tft_setCursor(110, 60);
            sprintf(buffer, "%d", width);
            tft_writeString(buffer);
            tft_setTextColor(ILI9340_BLUE);
            int i;
            for(i=0;i<width;i++){
                tft_setCursor(0, 40+20*i);
                sprintf(buffer, "%d", array[i]);
                tft_writeString(buffer);
            }
        }
    }
} // main

// === end  ======================================================
