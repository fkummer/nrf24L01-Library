
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

char reg;

volatile int button_press = 0; // goes high after button was pressed

char retry_num = nrf24l01_SETUP_RETR_ARC_15 | nrf24l01_SETUP_RETR_ARD_1000;

// sets up button on base station
// button uses external interrupt 0 on pin 16

void buttonSetup() {
    TRISBbits.TRISB7 = 1; // set pin 16 as interrupt
    ConfigINT0(EXT_INT_ENABLE | RISING_EDGE_INT | EXT_INT_PRI_2);
    EnableINT0;
}

void radioSetup() {
    nrf_setup();
    TRIS_csn = 0;
    TRIS_ce = 0;
    nrf_set_arc(0x0A);//NEW ADDITION, disables retransmits
    nrf_set_rf_ch(0x01);
    nrf_en_aa(0);
    nrf_en_dpl(0);
    nrf_set_pw(15, 0);
    nrf_set_address_width(5);
    nrf_set_rx_addr(0, 0xAABBCCDDFF, 5); //SOMETHING WRONG WITH ADDRESSES
    nrf_set_tx_addr(0xAABBCCDDFF);
}

// button was pressed

void __ISR(_EXTERNAL_0_VECTOR, ipl2) INT0Interrupt() {
    
    int count; // debounce counter
    while (mPORTBReadBits(BIT_7)) {
        count++;
  
    }
    if (count > 50000) { // debounce button
        button_press = 1;

    }
    count = 0;
    mINT0ClearIntFlag();
}

static PT_THREAD(protothread_radio(struct pt *pt)) {
    PT_BEGIN(pt);
    static int toggle = 0;
    static int flag = 0;
    static char payload[15];
    static char payload2[10];
    int i;
    for(i=0;i<15;i++){
        payload[i] = i;
    }
    for(i=0;i<10;i++){
        payload2[i] = 10-i;
    }
    int succSend = 0;
    while (1) {
        tft_setTextColor(ILI9340_GREEN);
        tft_setTextSize(2);
        tft_setCursor(20, 0);
        sprintf(buffer, "%d", succSend);
        tft_writeString(buffer);
        tft_setTextColor(ILI9340_YELLOW);
        for(i=0;i<15;i++){
            tft_setCursor(20, 20 + 20*i);
            sprintf(buffer, "%02X", payload[i]);
            tft_writeString(buffer);
        }
        for(i=0;i<10;i++){
            tft_setCursor(80, 20 + 20*i);
            sprintf(buffer, "%02X", payload2[i]);
            tft_writeString(buffer);
        }
        if (button_press == 1) {
            _LEDYELLOW = 1;
            if (1) {
                succSend = nrf_send_payload(&payload, 15);
                for (i = 0; i < 15; i++) {
                    payload[i]++;
                }

            } else {
                succSend = nrf_send_payload(&payload2, 10);
                for (i = 0; i < 10; i++) {
                    payload2[i]++;
                }
            }
            flag = !flag;

            tft_fillScreen(ILI9340_BLACK);
            button_press = 0;
        }
        delay_ms(2000);
        tft_fillScreen(ILI9340_BLACK);
    }
    
    
    PT_END(pt);
} // timer thread
//=== Main  ======================================================

void main(void) {
    INTEnableSystemMultiVectoredInt();
    PT_setup();
    buttonSetup();
    radioSetup();
    _TRIS_LEDRED = 0;
    _TRIS_LEDYELLOW = 0;
    _TRIS_LEDGREEN = 0;
    _LEDRED = 0;
    _LEDYELLOW = 0;
    _LEDGREEN = 0;
    PT_INIT(&pt_radio);
    tft_init_hw();
    tft_begin();
    tft_fillScreen(ILI9340_BLACK);
    //240x320 vertical display
    tft_setRotation(0); // Use tft_setRotation(1) for 320x240

    
    mINT1ClearIntFlag();
   
    while (1) {
        PT_SCHEDULE(protothread_radio(&pt_radio));
    }
} // main

// === end  ======================================================