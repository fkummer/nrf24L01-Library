#include "config.h"
#include <stdio.h>
#define	SYS_FREQ 64000000 // change the frequency of the clock
#include "nrf24l01.h"

// Set up LEDs for displaying power
// Green LED on pin 2
#define _LEDGREEN        LATAbits.LATA0
#define _TRIS_LEDGREEN   TRISAbits.TRISA0

// Yellow LED on pin 10
#define _LEDYELLOW        LATAbits.LATA3
#define _TRIS_LEDYELLOW   TRISAbits.TRISA3

// Red LED on pin 11
#define _LEDRED        LATBbits.LATB4
#define _TRIS_LEDRED   TRISBbits.TRISB4

/*
 * This code will send a continuous carrier wave that can be detected by another
 * radio.  It can be helpful for finding the maximum distance two radios can
 * receive from.  The power the radio transmits at can be changed if desired.
 */

// set up the radio
void radioSetup() {
    nrf_setup(); // initializing function
    
    nrf_set_rf_ch(0x01); // freq = 2.401 GHz
}

void main(void) {
    INTEnableSystemMultiVectoredInt();
    radioSetup(); // setup the radio for this program
    mINT1ClearIntFlag();
    nrf_state_rx_mode(); // put the radio into receive mode
    nrf_delay_us(40); // need to wait 40us after entering receive mode before checking power
    // set up LEDs
    _TRIS_LEDGREEN = 0;
    _TRIS_LEDRED = 0;
    // continuously check power
    while(1){
        // if power is received
        if(nrf_received_pwr()){
            _LEDGREEN = 1; // turn on green LED when power is received
            _LEDRED = 0; // turn off red LED when power is received
        }else{
            _LEDGREEN = 0; // turn on green LED when power is not received
            _LEDRED = 1; // turn on red LED when power is not received
        }
    }
} // main

// === end  ======================================================