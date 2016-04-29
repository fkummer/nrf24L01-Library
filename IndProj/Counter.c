/// graphics libraries
#include "config.h"
#include "tft_gfx.h"
#include "tft_master.h"
// serial stuff
#include <stdio.h>
// threading library
#define	SYS_FREQ 64000000 // change the frequency of the clock
// radio library
#include "nrf24l01.h"

#define spi_channel	1
// Use channel 2 since channel 1 is used by TFT display

#define spi_divider 10

/*
 * This code implements a synchronous counter using two PIC32s both connected to 
 * tft displays and nrf24l01+ radios.  The ID field for the two PICs must be set
 * to 0 on the first PIC and 1 on the second.  Radio 0 will begin the program by
 * sending the initial value of counter which will be zero.  Once it gets an
 * acknowledgement from radio 1 it will go into receive mode and the counting
 * will start.  Radio 0 will increment the counter and send it to radio 1 which
 * will display the count and send it back to radio 0.  Radio 0 will receive the
 * count back and will then display it, increment it, and send the updated count
 * to radio 1.  Radio 1 will repeat what it did before and the loop will then 
 * continue indefinitely.
 * 
 * The counter will increment as fast as the radios can send and the screen can
 * update.
 */

// set up the radio
void radioSetup() {
    nrf_setup(); // initializing function

    nrf_set_arc(0x0A); // 10 retransmits
    
    nrf_set_ard(0x00); // 250 us between retransmission

    nrf_set_rf_ch(0x01); // freq = 2.401 GHz

    nrf_en_aa(0); // enable autoack on pipe 0

    nrf_set_pw(1, 0); //set the payload width to be 1 byte

    // set up addresses for autoack mode
    // The tx address and the pipe 0 rx address must be the same for autoack mode
    nrf_set_address_width(5);
    nrf_set_rx_addr(0, 0xAABBCCDDFF, 5);
    nrf_set_tx_addr(0xAABBCCDDFF);
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
    while (1) {
        //NOTE: Set one radio's ID to 0 and the other radio's ID to 1
        int ID = 0;
        volatile char counter = 0; // synchronous counter
        if (!ID) { // the first radio will keep transmitting until a packet is received        
            // while the packet isn't acknowledged/received continously resend
            while (!nrf_send_payload(&counter, 1)) {
                // blink a circle on screen while waiting
                tft_fillCircle(20, 20, 10, ILI9340_BLACK);
                nrf_delay_ms(300);
                tft_fillCircle(20, 20, 10, ILI9340_RED);
                nrf_delay_ms(300);
            }
        }
        // set radio to receive mode to start
        nrf_state_rx_mode();
        // clear the screen
        tft_fillScreen(ILI9340_BLACK);
        while (1) {
            // display "Current Count:" at top of screen
            tft_setTextColor(ILI9340_GREEN);
            tft_setTextSize(2);
            tft_setCursor(20, 0);
            sprintf(buffer, "%s", "Current Count:");
            tft_writeString(buffer);
            // wait until a payload is received before doing anything
            if (nrf_payload_available()) { // when a payload has been received
                tft_fillScreen(ILI9340_BLACK);
                // get new counter value from transmitter
                nrf_get_payload(&counter, 1);
                // display counter value
                tft_setCursor(20, 20);
                sprintf(buffer, "%d", counter);
                tft_writeString(buffer);
                if (!ID) { // increment counter on one radio
                    counter++;
                }
                // send incremented payload
                nrf_send_payload(&counter, 1); // send counter back to other radio
                nrf_state_rx_mode(); // put the radio in rx mode after the transmission
            }
        }
    }
} // main

// === end  ======================================================