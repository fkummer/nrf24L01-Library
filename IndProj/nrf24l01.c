
// serial stuff
#include <stdio.h>
#include <stdlib.h>

#include "nrf24l01.h"


// PIN Setup
// SCK -> SCK2 (pin 26)
// SDI -> MISO (RPA4) (pin 12)
// SDO -> MOSI (RPB2) (pin 9)
// IRQ -> extern interrupt 1 (RPB10) (pin 21)
// CSN -> RPB9 (I/O) (pin 18)
// CE -> RPB8 (I/O) (pin 17)
// i/o names 

// frequency we're running at
#define	SYS_FREQ 64000000

char rf_spiwrite(unsigned char c){ // Transfer to SPI
    while (TxBufFullSPI2());
    WriteSPI2(c);
    while( !SPI2STATbits.SPIRBF); // check for complete transmit
    return ReadSPI2();
}

void init_SPI(){
    // Set up SPI2 to be active high, master, 8 bit mode, and ~4 Mhz CLK
    SpiChnOpen(2, SPI_OPEN_MSTEN | SPI_OPEN_MODE8 | SPI_OPEN_ON | SPI_OPEN_CKE_REV, 16);
    // Set SDI2 to pin 12
    PPSInput(3, SDI2, RPA4);
    // Set SDO2 to pin 9
    PPSOutput(3, RPA2, SDO2);
    // Set external interrupt 1 to pin 21
    PPSInput(4, INT1, RPB10);
    
    
    ConfigINT1(EXT_INT_PRI_2 | FALLING_EDGE_INT | EXT_INT_ENABLE);
    EnableINT1;
}

// Read a register from the nrf24l01
// reg is the array to read, len is the length of data expected to be received (1-5 bytes)
// NOTE: only address 0 and 1 registers use 5 bytes all others use 1 byte 
// NOTE: writing or reading payload is done using a specific command
void nrf_read_reg(char reg, char * buff, int len){
    //char reg_read[5]; // register used for reading a single register
    int i = 0;
    _csn = 0; // begin transmission
    status = rf_spiwrite(nrf24l01_R_REGISTER | reg); // send command to read register
    
    for(i=0;i<len;i++){
        buff[i] = rf_spiwrite(nrf24l01_SEND_CLOCK); // send clock pulse to continue receiving data
    }
    _csn = 1; // end transmission
}


void nrf_write_reg(char reg, char * data, char len){
    int i = 0;
    _csn = 0; // begin transmission
    status = rf_spiwrite(nrf24l01_W_REGISTER | reg); // send command to write reg
    for(i=0;i<len;i++){
        rf_spiwrite(data[i]); // write each char/byte to address reg
    }
    _csn = 1; // end transmission
}

// flushes the tx FIFO
void nrf_flush_tx(){
    _csn = 0;
    rf_spiwrite(nrf24l01_FLUSH_TX);
    _csn = 1;
    
}

// flushes the rx FIFO
// NOTE: do not use while sending acknowledge
void nrf_flush_rx(){
    _csn = 0;
    rf_spiwrite(nrf24l01_FLUSH_RX);
    _csn = 1;   
}


// Write a payload to be sent over the radio
// data: array of chars to be sent (1-32 chars/bytes)
// len: amount of chars in array/bytes to be sent
// NOT TESTED YET
void nrf_write_payload(char * data, char len){
    int i = 0;
    _csn = 0; // begin transmission
    status = rf_spiwrite(nrf24l01_W_TX_PAYLOAD); // send the command to write the payload
    for(i=0;i<len;i++){
        rf_spiwrite(data[i]); // write each char/byte to tx payload one at a time
    }
    _csn = 1; // end transmission
    
}

// should read the payload into a buffer NOT TESTED YET
void nrf_read_payload(char * buff){
    _csn = 0; // begin transmission
    status = rf_spiwrite(nrf24l01_R_RX_PAYLOAD); // send command to read payload
    int i;
    for(i=0;i<payload_size;i++){
        buff[i] = rf_spiwrite(nrf24l01_SEND_CLOCK);
    }
    _csn = 1; // end transmission

}

//Sets the power up bit and waits for the startup time, putting the radio in Standby-I mode
void nrf_pwrup(){
    nrf_read_reg(nrf24l01_CONFIG, &config, 1);
    config |= nrf24l01_CONFIG_PWR_UP;
    nrf_write_reg(nrf24l01_CONFIG, &config, 1);
    delay_ms(2);//Delay for power up time
}

//Clear the pwr_up bit, transitioning to power down mode
void nrf_pwrdown(){
    nrf_read_reg(nrf24l01_CONFIG, &config, 1);
    config &= ~(nrf24l01_CONFIG_PWR_UP);
    nrf_write_reg(nrf24l01_CONFIG, &config, 1);
    _ce = 0;
}

//Transitions to rx mode from standby mode
void nrf_rx_mode(){
//    nrf_read_reg(nrf24l01_STATUS, &status, 1); // read the status register
//    status |= nrf24l01_STATUS_RX_DR; // clear interrupt on radio
//    status |= nrf24l01_STATUS_TX_DS; // clear interrupt on radio
//    status |= nrf24l01_STATUS_MAX_RT; // clear interrupt on radio
//    nrf_write_reg(nrf24l01_STATUS, &status, 1);
    
    nrf_read_reg(nrf24l01_CONFIG, &config, 1);
    config |= nrf24l01_CONFIG_PRIM_RX;
    nrf_write_reg(nrf24l01_CONFIG, &config, 1);
    _ce = 1;
    delay_us(200); //Wait for transition to rx mode time

}

//Transitions to tx mode from standby mode
void nrf_tx_mode(){
//    nrf_read_reg(nrf24l01_STATUS, &status, 1); // read the status register
//    status |= nrf24l01_STATUS_RX_DR; // clear interrupt on radio
//    status |= nrf24l01_STATUS_TX_DS; // clear interrupt on radio
//    status |= nrf24l01_STATUS_MAX_RT; // clear interrupt on radio
//    nrf_write_reg(nrf24l01_STATUS, &status, 1);
    
    nrf_read_reg(nrf24l01_CONFIG, &config, 1);
    config &= ~(nrf24l01_CONFIG_PRIM_RX);
    nrf_write_reg(nrf24l01_CONFIG, &config, 1);
    _ce = 1;
    delay_us(200); //Wait for transition to tx mode time
}

void nrf_standby_mode(){
    _ce = 0;
}

// TESTED
// sets power of transmitter, possible values and definitions for them are
//  0dBm: nrf24l01_RF_SETUP_RF_PWR_0
// -6dBm: nrf24l01_RF_SETUP_RF_PWR_6
// -12dBm: nrf24l01_RF_SETUP_RF_PWR_12
// -18dBm: nrf24l01_RF_SETUP_RF_PWR_18
void nrf_set_transmit_pwr(char power){
    char setup;
    nrf_read_reg(nrf24l01_RF_SETUP, &setup, 1); // check value of setup register
    setup &= ~(nrf24l01_RF_SETUP_RF_PWR); // clear power bits in register
    setup |= power; // set power bits in register
    nrf_write_reg(nrf24l01_RF_SETUP, &setup, 1);  
}

// TESTED
// sets the rf data rate, possible values and definitoins for them are
// 250 kbps: nrf24l01_DR_LOW
// 1 Mbps: nrf24l01_DR_MED
// 2 Mbps: nrf24l01_DR_HIGH
void nrf_set_transmit_rate(char rate){
    char setup; // check value of setup register
    nrf_read_reg(nrf24l01_RF_SETUP, &setup, 1);
    setup &= 0xd7;  // clear data rate bits in register
    setup |= rate; // set data rate bits in register
    nrf_write_reg(nrf24l01_RF_SETUP, &setup, 1); 
}

// TESTED
void nrf_set_ard(char ard){
    char setup; // check value of setup register
    nrf_read_reg(nrf24l01_SETUP_RETR, &setup, 1);
    setup &= 0x0F;  // clear data rate bits in register
    setup |= ard; // set data rate bits in register
    nrf_write_reg(nrf24l01_SETUP_RETR, &setup, 1); 
}

// TESTED
void nrf_set_arc(char arc){
    char setup; // check value of setup register
    nrf_read_reg(nrf24l01_SETUP_RETR, &setup, 1);
    setup &= 0xF0;  // clear data rate bits in register
    setup |= arc; // set data rate bits in register
    nrf_write_reg(nrf24l01_SETUP_RETR, &setup, 1); 
}

// TESTED
void nrf_set_rf_ch(char ch){
    nrf_write_reg(nrf24l01_RF_CH, &ch, 1);
}

// TESTED
void nrf_set_address_width(char width){
    char setting = width - 2;
    nrf_write_reg(nrf24l01_SETUP_AW, &setting, 1);
}



char * parse_addr(uint64_t address){
    static char addr[5];
    int i = 0;
    for(i = 0; i < 5;i++){
        addr[i] = (char)((address >> i*8) & 0xFF);
    }
    return &addr;
}

int nrf_set_rx_addr(int pipe, uint64_t address, int len){
    char width;
    nrf_read_reg(nrf24l01_SETUP_AW, &width, 1);
    width = width + 2;
    char * addr = parse_addr(address);
    
    if(pipe == 0 || pipe == 1){
        if(len != width){//Must equal preset address width
            return 0;
        }
        
    }else{
        if(len != 1){
            return 0;//For pipes 2,3,4,5 can only set last byte
        }
    }
    
    nrf_write_reg(nrf24l01_RX_ADDR_P0+pipe, addr, len);
    return 1;
    
    
}
// Sends out a specified payload (in auto acknowledge mode by default)
// use after powering up radio, and setting address or other settings
void nrf_send_payload(char * data, int len){
    nrf_flush_tx(); // clear the TX FIFO so for a new transmission
    nrf_write_payload(data, len);
    nrf_tx_mode();
    while(!(sent)){ // wait until data sent interrupt triggers
        TRISBbits.TRISB3 = 0;
        LATBbits.LATB3 = 1;
    }
    LATBbits.LATB3 = 0;
    sent = 0;
    //_ce = 0; // transition to standby II mode
}

// TESTED
void nrf_start_cont_wave(char pwr){
    nrf_pwrup();
    nrf_tx_mode();
    char setting;
    nrf_read_reg(nrf24l01_RF_SETUP, &setting, 1);
    setting |= nrf24l01_RF_SETUP_PLL_LOCK | nrf24l01_RD_SETUP_CONT_WAVE;
    nrf_write_reg(nrf24l01_RF_SETUP, &setting, 1);
    nrf_set_transmit_pwr(pwr);
    _ce = 1;   
}

// TESTED
void nrf_stop_cont_wave(){
    _ce = 0;
    char reg;
    nrf_read_reg(nrf24l01_RF_SETUP, &reg, 1);
    reg &= ~(nrf24l01_RF_SETUP_PLL_LOCK | nrf24l01_RD_SETUP_CONT_WAVE);  
    nrf_write_reg(nrf24l01_RF_SETUP, &reg, 1);
    nrf_pwrdown();
}

//TESTED
char nrf_received_pwr(){
    char pwr;
    nrf_read_reg(nrf24l01_RPD, &pwr, 1);
    pwr &= 0x01;
    return pwr;
}

// TESTED
void nrf_en_aa(int pipe){
    char num = 0x01;
    num = num << pipe;
    char reg;
    nrf_read_reg(nrf24l01_EN_AA, &reg, 1);
    reg |= num;
    // enable the pipe
    nrf_en_rxaddr(pipe);
    nrf_write_reg(nrf24l01_EN_AA, &reg, 1);
}

// TESTED
void nrf_dis_aa(int pipe){
    char num = 0x01;
    num = num << pipe;
    char reg;
    nrf_read_reg(nrf24l01_EN_AA, &reg, 1);
    num = ~num;
    reg &= num;
    nrf_write_reg(nrf24l01_EN_AA, &reg, 1);
}

// TESTED
void nrf_en_rxaddr(int pipe){
    char num = 0x01;
    num = num << pipe;
    char reg;
    nrf_read_reg(nrf24l01_EN_RXADDR, &reg, 1);
    reg |= num;
    nrf_write_reg(nrf24l01_EN_RXADDR, &reg, 1);
}

// TESTED
void nrf_dis_rxaddr(int pipe){
    char num = 0x01;
    num = num << pipe;
    char reg;
    nrf_read_reg(nrf24l01_EN_RXADDR, &reg, 1);
    num = ~num;
    reg &= num;
    nrf_write_reg(nrf24l01_EN_RXADDR, &reg, 1);
}

// TESTED
void nrf_en_dpl(int pipe){
    char num = 0x01;
    num = num << pipe;
    char reg;
    nrf_read_reg(nrf24l01_DYNPD, &reg, 1);
    // set bit corresponding to pipe
    reg |= num;
    nrf_write_reg(nrf24l01_DYNPD, &reg, 1);
    // set EN_DPL in FEATURE register
    nrf_read_reg(nrf24l01_FEATURE, &reg, 1);
    reg |= 0x04;
    nrf_write_reg(nrf24l01_FEATURE, &reg, 1);
    // enable autoack for the pipe
    nrf_en_aa(pipe);
}

// TESTED
void nrf_dis_dpl(int pipe){
    char num = 0x01;
    num = num << pipe;
    char reg;
    nrf_read_reg(nrf24l01_DYNPD, &reg, 1);
    // clear bit corresponding to pipe
    num = ~num;
    reg &= num;
    nrf_write_reg(nrf24l01_DYNPD, &reg, 1);
    if(reg == 0x00){ // check if dpl has been disabled in all pipes
        // clear EN_DPL in FEATURE register
        nrf_read_reg(nrf24l01_FEATURE, &reg, 1);
        reg &= ~0x04;
        nrf_write_reg(nrf24l01_FEATURE, &reg, 1);
    }
}

// TESTED
void nrf_en_dyn_ack(){
    char reg;
    nrf_read_reg(nrf24l01_FEATURE, &reg, 1);
    reg |= 0x01;
    nrf_write_reg(nrf24l01_FEATURE, &reg, 1);
}

// TESTED
void nrf_dis_dyn_ack(){
    char reg;
    nrf_read_reg(nrf24l01_FEATURE, &reg, 1);
    reg &= ~0x01;
    nrf_write_reg(nrf24l01_FEATURE, &reg, 1);
}

void __ISR(_EXTERNAL_1_VECTOR, ipl2) INT1Handler(void){
    //_LEDRED = 1;
    nrf_read_reg(nrf24l01_STATUS, &status, 1); // read the status register
    // check which type of interrupt occurred

    if (status & nrf24l01_STATUS_RX_DR) { // if data received
        nrf_read_payload(&RX_payload);
        received = 1; // signal main code that payload was received
        status |= nrf24l01_STATUS_RX_DR; // clear interrupt on radio
        nrf_flush_rx();
    }
        // if data sent or if acknowledge received when auto ack enabled
    else if (status & nrf24l01_STATUS_TX_DS) {
        //_LEDYELLOW = 1;
        sent = 1; // signal main code that payload was sent
        status |= nrf24l01_STATUS_TX_DS; // clear interrupt on radio
    } else { // maximum number of retransmit attempts occurred
        //_LEDRED = 1;
        error = 1; // signal main code that the payload was not received
        status |= nrf24l01_STATUS_MAX_RT; // clear interrupt on radio
    }
    
    nrf_write_reg(nrf24l01_STATUS, &status, 1);
    mINT1ClearIntFlag();
}

