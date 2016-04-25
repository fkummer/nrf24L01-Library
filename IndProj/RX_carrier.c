// graphics libraries
#include "config.h"
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
 * This code will send a continuous carrier wave that can be detected by another
 * radio.  It can be helpful for finding the maximum distance two radios can
 * receive from.  The power the radio transmits at can be changed if desired.
 */

// set up the radio
void radioSetup() {
    nrf_setup(); // initializing function
    
    nrf_set_rf_ch(0x01); // freq = 2.401 GHz
    
    // set up addresses
    nrf_set_address_width(5);
    nrf_set_rx_addr(0, 0xAABBCCDDFF, 5);
}

void main(void) {
    INTEnableSystemMultiVectoredInt();
    radioSetup(); // setup the radio for this program
    mINT1ClearIntFlag();
    nrf_state_rx_mode(); // put the radio into receive mode
    while(1){
        
    }
} // main

// === end  ======================================================