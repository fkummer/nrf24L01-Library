
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

char retry_num = nrf24l01_SETUP_RETR_ARC_15 | nrf24l01_SETUP_RETR_ARD_1000;

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

static PT_THREAD(protothread_radio(struct pt *pt)) {
    PT_BEGIN(pt);
    static int toggle = 0;
    //char reg[5];
    char payload = 0xaa;
    int test = 2;
    //nrf_set_address_width(5);
    while (1) {
//        test =  nrf_set_rx_addr(1, 0x12129A, 3);
//        nrf_read_reg(nrf24l01_RX_ADDR_P1, &reg, 5);        
//        int i;
//        for(i=0;i<5;i++){
//            tft_setCursor(20, 20+20*i);
//            tft_setTextColor(ILI9340_YELLOW);
//            tft_setTextSize(2);
//            sprintf(buffer, "%02X", reg[i]);
//            tft_writeString(buffer);
//        }
//        nrf_read_reg(nrf24l01_RX_ADDR_P2, &reg, 1);
//        tft_setCursor(20, 140);
//            tft_setTextColor(ILI9340_YELLOW);
//            tft_setTextSize(2);
//            sprintf(buffer, "%d", test);
//            tft_writeString(buffer);
        
            tft_setCursor(20, 140);
            tft_setTextColor(ILI9340_YELLOW);
            tft_setTextSize(2);
            sprintf(buffer, "%d", test);
            tft_writeString(buffer);
        
        if(button_press == 1){
            if(toggle == 0){
                toggle = 1;
                _LEDRED = 1;
                test = nrf_send_payload(&payload,1);
                sent = 0;
                error = 0;
                payload++;
            }else{
                toggle = 0;
                _LEDRED = 0;
            }
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
    //reset();
    //PT_setup();
    buttonSetup();
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;
    _TRIS_LEDRED = 0;
    //PT_INIT(&pt_radio);
    tft_init_hw();
    tft_begin();
    tft_fillScreen(ILI9340_BLACK);
    //240x320 vertical display
    tft_setRotation(0); // Use tft_setRotation(1) for 320x240
   
    while (1) {    
        PT_SCHEDULE(protothread_radio(&pt_radio));
    }
} // main

// === end  ======================================================