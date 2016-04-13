// graphics libraries
#include "config.h"
#include "tft_gfx.h"
#include "tft_master.h"
// need for rand function
#include <stdlib.h>
// serial stuff
#include <stdio.h>
#include <math.h>
// threading library

#define	SYS_FREQ 64000000 //40000000
#include "pt_cornell_TFT.h"

#include "nrf24l01.h"

#define spi_channel	1
// Use channel 2 since channel 1 is used by TFT display

#define spi_divider 10

// === thread structures ============================================
// thread control structs
// note that UART input and output are threads
static struct pt pt_radio;

char send;
char receive;

volatile int button_press = 0; // goes high after button was pressed

// sets up button on base station
// button uses external interrupt 0 on pin 16

void buttonSetup() {
    TRISBbits.TRISB7 = 1; // set pin 16 as interrupt
    ConfigINT0(EXT_INT_ENABLE | RISING_EDGE_INT | EXT_INT_PRI_2);
    EnableINT0;
}

// button was pressed
void __ISR(_EXTERNAL_0_VECTOR, ipl2) INT0Interrupt() {
    int count; // debounce counter
    while (mPORTBReadBits(BIT_7)) {
        count++;
//        if (count > 750) {
//        }
    }
    if (count > 700) { // debounce button
        button_press = 1;
    }
    count = 0;
    mINT0ClearIntFlag();
}

void radioSetup() {
    nrf_setup();
    TRIS_csn = 0;
    TRIS_ce = 0;
    nrf_set_arc(0x0A);//NEW ADDITION
    nrf_set_rf_ch(0x01);
    nrf_en_aa(1);
    nrf_en_dpl(1);
    nrf_en_aa(4);
    nrf_en_dpl(4);
    nrf_set_pw(15, 1);
    nrf_set_address_width(5);
    nrf_set_rx_addr(1, 0xAABBCCDDFF, 5);
    nrf_set_rx_addr(4, 0xEE, 1);
    nrf_set_tx_addr(0xAABBCCDDEE);
}

static PT_THREAD(protothread_radio(struct pt *pt)) {
    PT_BEGIN(pt);
    int width;
    char payload[15];
    static int flag = 0;
    nrf_state_rx_mode();
    nrf_flush_rx();
    while (1) {
        while(!nrf_payload_available()){
            tft_setCursor(20, 0);
            tft_setTextColor(ILI9340_YELLOW);
            tft_setTextSize(2);
            sprintf(buffer, "%s", "Waiting for payload...");
            tft_writeString(buffer);
        }
        _LEDRED ^= 1;
        width = nrf_get_width();
        nrf_get_payload(&payload, nrf_get_width());
        tft_fillScreen(ILI9340_BLACK);
        tft_writeString(buffer);
        tft_setTextSize(2);
        //payload = RX_payload[0];
        //tft_setCursor(20, 40);
        tft_setTextColor(ILI9340_GREEN);
        tft_setTextSize(2);
        if(1){
            int i;
            for(i=0;i<15;i++){
                tft_setCursor(20, 20+20*i);
                sprintf(buffer, "%02X", payload[i]);
                tft_writeString(buffer);
                
                tft_setTextColor(ILI9340_GREEN);
                tft_setCursor(100, 20);
                sprintf(buffer, "%d", nrf_get_pipe());
                tft_writeString(buffer);
                
                tft_setTextColor(ILI9340_RED);
                tft_setCursor(100, 40);
                sprintf(buffer, "%d", nrf_get_width());
                tft_writeString(buffer);
            }
        }else{
            int i;
            for(i=0;i<15;i++){
                tft_setCursor(20, 20+20*i);
                sprintf(buffer, "%02X", payload[i]);
                tft_writeString(buffer);
            }
        }
        flag = !flag;
    }
    PT_END(pt);
} // timer thread

// === Main  ======================================================

void main(void) {
    INTEnableSystemMultiVectoredInt();
    //reset();
    PT_setup();
    buttonSetup();
    _TRIS_LEDRED = 0;
    _TRIS_LEDYELLOW = 0;
    _TRIS_LEDGREEN = 0;
    _LEDRED = 0;
    _LEDYELLOW = 0;
    _LEDGREEN = 0;
    PT_INIT(&pt_radio);
    radioSetup();
    
    tft_init_hw();
    tft_begin();
    tft_fillScreen(ILI9340_BLACK);
    //240x320 vertical display
    tft_setRotation(0); // Use tft_setRotation(1) for 320x240
    
    while (1) {
        PT_SCHEDULE(protothread_radio(&pt_radio));
    }
} // main

// === end  ===