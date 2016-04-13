

/**
 * @file nrf24l01.h
 * @author Douglas Katz and Frederick Kummer
 * @date February 7 2016
 * @brief Library for use of the nrf24l01+ radio module with a PIC32
 *
 *
 */


 #define _SUPPRESS_PLIB_WARNING 1
 #include <plib.h>
 #include "inttypes.h"
// PIN Setup
// SCK -> SCK1 (pin 26)
// SDI -> MISO (RPA4) (pin 12)
// SDO -> MOSI (RPA2) (pin 9)
// IRQ -> extern interrupt 1 (RPB10) (pin 21)
// CSN -> RPB9 (I/O) (pin 18)
// CE -> RPB8 (I/O) (pin 17)

#define SPI_CHANNEL 2 //SPI channel used
#define SDI RPA4 //Pin used as MISO for SPI
#define SDO RPA2 //Pin used as MOSI for SPI
#define IRQ RPB10 //Pin used for external interrupts from radio

#define _csn         LATBbits.LATB9
#define TRIS_csn     TRISBbits.TRISB9

#define _ce         LATBbits.LATB8
#define TRIS_ce     TRISBbits.TRISB8

#define _LEDGREEN        LATAbits.LATA0
#define _TRIS_LEDGREEN   TRISAbits.TRISA0

#define _LEDYELLOW        LATAbits.LATA3
#define _TRIS_LEDYELLOW   TRISAbits.TRISA3

#define _LEDRED        LATBbits.LATB4
#define _TRIS_LEDRED   TRISBbits.TRISB4

// SPI Commands
#define nrf24l01_R_REGISTER		0x00
#define nrf24l01_W_REGISTER		0x20
#define nrf24l01_R_REGISTER_WID 0x61
#define nrf24l01_R_RX_PL_WID    0x60
#define nrf24l01_R_RX_PAYLOAD	0x61
#define nrf24l01_W_TX_PAYLOAD	0xA0
#define nrf24l01_FLUSH_TX		0xE1
#define nrf24l01_FLUSH_RX		0xE2
#define nrf24l01_REUSE_TX_PL	0xE3
#define nrf24l01_NOP			0xFF


// Register definitions
#define nrf24l01_CONFIG			0x00
#define nrf24l01_EN_AA			0x01
#define nrf24l01_EN_RXADDR		0x02
#define nrf24l01_SETUP_AW		0x03
#define nrf24l01_SETUP_RETR		0x04
#define nrf24l01_RF_CH			0x05
#define nrf24l01_RF_SETUP		0x06
#define nrf24l01_STATUS			0x07
#define nrf24l01_OBSERVE_TX		0x08
#define nrf24l01_RPD            0x09
#define nrf24l01_RX_ADDR_P0		0x0A
#define nrf24l01_RX_ADDR_P1		0x0B
#define nrf24l01_RX_ADDR_P2		0x0C
#define nrf24l01_RX_ADDR_P3		0x0D
#define nrf24l01_RX_ADDR_P4		0x0E
#define nrf24l01_RX_ADDR_P5		0x0F
#define nrf24l01_TX_ADDR		0x10
#define nrf24l01_RX_PW_P0		0x11
#define nrf24l01_RX_PW_P1		0x12
#define nrf24l01_RX_PW_P2		0x13
#define nrf24l01_RX_PW_P3		0x14
#define nrf24l01_RX_PW_P4		0x15
#define nrf24l01_RX_PW_P5		0x16
#define nrf24l01_FIFO_STATUS	0x17
#define nrf24l01_DYNPD          0x1C
#define nrf24l01_FEATURE        0x1D

//CONFIG register bitwise definitions
#define nrf24l01_CONFIG_RESERVED	0x80
#define	nrf24l01_CONFIG_MASK_RX_DR	0x40
#define	nrf24l01_CONFIG_MASK_TX_DS	0x20
#define	nrf24l01_CONFIG_MASK_MAX_RT	0x10
#define	nrf24l01_CONFIG_EN_CRC		0x08
#define	nrf24l01_CONFIG_CRCO		0x04
#define	nrf24l01_CONFIG_PWR_UP		0x02
#define	nrf24l01_CONFIG_PRIM_RX		0x01

//EN_AA register bitwise definitions
#define nrf24l01_EN_AA_RESERVED		0xC0
#define nrf24l01_EN_AA_ENAA_ALL		0x3F
#define nrf24l01_EN_AA_ENAA_P5		0x20
#define nrf24l01_EN_AA_ENAA_P4		0x10
#define nrf24l01_EN_AA_ENAA_P3		0x08
#define nrf24l01_EN_AA_ENAA_P2		0x04
#define nrf24l01_EN_AA_ENAA_P1		0x02
#define nrf24l01_EN_AA_ENAA_P0		0x01
#define nrf24l01_EN_AA_ENAA_NONE	0x00

//EN_RXADDR register bitwise definitions
#define nrf24l01_EN_RXADDR_RESERVED	0xC0
#define nrf24l01_EN_RXADDR_ERX_ALL	0x3F
#define nrf24l01_EN_RXADDR_ERX_P5	0x20
#define nrf24l01_EN_RXADDR_ERX_P4	0x10
#define nrf24l01_EN_RXADDR_ERX_P3	0x08
#define nrf24l01_EN_RXADDR_ERX_P2	0x04
#define nrf24l01_EN_RXADDR_ERX_P1	0x02
#define nrf24l01_EN_RXADDR_ERX_P0	0x01
#define nrf24l01_EN_RXADDR_ERX_NONE	0x00

//SETUP_AW register bitwise definitions
#define nrf24l01_SETUP_AW_RESERVED	0xFC
#define nrf24l01_SETUP_AW			0x03
#define nrf24l01_SETUP_AW_5BYTES	0x03
#define nrf24l01_SETUP_AW_4BYTES	0x02
#define nrf24l01_SETUP_AW_3BYTES	0x01
#define nrf24l01_SETUP_AW_ILLEGAL	0x00

//SETUP_RETR register bitwise definitions
#define nrf24l01_SETUP_RETR_ARD			0xF0
#define nrf24l01_SETUP_RETR_ARD_4000	0xF0
#define nrf24l01_SETUP_RETR_ARD_3750	0xE0
#define nrf24l01_SETUP_RETR_ARD_3500	0xD0
#define nrf24l01_SETUP_RETR_ARD_3250	0xC0
#define nrf24l01_SETUP_RETR_ARD_3000	0xB0
#define nrf24l01_SETUP_RETR_ARD_2750	0xA0
#define nrf24l01_SETUP_RETR_ARD_2500	0x90
#define nrf24l01_SETUP_RETR_ARD_2250	0x80
#define nrf24l01_SETUP_RETR_ARD_2000	0x70
#define nrf24l01_SETUP_RETR_ARD_1750	0x60
#define nrf24l01_SETUP_RETR_ARD_1500	0x50
#define nrf24l01_SETUP_RETR_ARD_1250	0x40
#define nrf24l01_SETUP_RETR_ARD_1000	0x30
#define nrf24l01_SETUP_RETR_ARD_750		0x20
#define nrf24l01_SETUP_RETR_ARD_500		0x10
#define nrf24l01_SETUP_RETR_ARD_250		0x00
#define nrf24l01_SETUP_RETR_ARC			0x0F
#define nrf24l01_SETUP_RETR_ARC_15		0x0F
#define nrf24l01_SETUP_RETR_ARC_14		0x0E
#define nrf24l01_SETUP_RETR_ARC_13		0x0D
#define nrf24l01_SETUP_RETR_ARC_12		0x0C
#define nrf24l01_SETUP_RETR_ARC_11		0x0B
#define nrf24l01_SETUP_RETR_ARC_10		0x0A
#define nrf24l01_SETUP_RETR_ARC_9		0x09
#define nrf24l01_SETUP_RETR_ARC_8		0x08
#define nrf24l01_SETUP_RETR_ARC_7		0x07
#define nrf24l01_SETUP_RETR_ARC_6		0x06
#define nrf24l01_SETUP_RETR_ARC_5		0x05
#define nrf24l01_SETUP_RETR_ARC_4		0x04
#define nrf24l01_SETUP_RETR_ARC_3		0x03
#define nrf24l01_SETUP_RETR_ARC_2		0x02
#define nrf24l01_SETUP_RETR_ARC_1		0x01
#define nrf24l01_SETUP_RETR_ARC_0		0x00

//RF_CH register bitwise definitions
#define nrf24l01_RF_CH_RESERVED	0x80

//RF_SETUP register bitwise definitions
#define nrf24l01_RD_SETUP_CONT_WAVE 0x80
#define nrf24l01_RF_SETUP_RESERVED	0xE0
#define nrf24l01_RF_SETUP_PLL_LOCK	0x10
#define nrf24l01_RF_SETUP_RF_DR		0x08
#define nrf24l01_RF_SETUP_RF_PWR	0x06
#define nrf24l01_RF_SETUP_RF_PWR_0	0x06
#define nrf24l01_RF_SETUP_RF_PWR_6 	0x04
#define nrf24l01_RF_SETUP_RF_PWR_12	0x02
#define nrf24l01_RF_SETUP_RF_PWR_18	0x00
#define nrf24l01_RF_SETUP_LNA_HCURR	0x01

//STATUS register bitwise definitions
#define nrf24l01_STATUS_RESERVED					0x80
#define nrf24l01_STATUS_RX_DR						0x40
#define nrf24l01_STATUS_TX_DS						0x20
#define nrf24l01_STATUS_MAX_RT						0x10
#define nrf24l01_STATUS_RX_P_NO						0x0E
#define nrf24l01_STATUS_RX_P_NO_RX_FIFO_NOT_EMPTY	0x0E
#define nrf24l01_STATUS_RX_P_NO_UNUSED				0x0C
#define nrf24l01_STATUS_RX_P_NO_5					0x0A
#define nrf24l01_STATUS_RX_P_NO_4					0x08
#define nrf24l01_STATUS_RX_P_NO_3					0x06
#define nrf24l01_STATUS_RX_P_NO_2					0x04
#define nrf24l01_STATUS_RX_P_NO_1					0x02
#define nrf24l01_STATUS_RX_P_NO_0					0x00
#define nrf24l01_STATUS_TX_FULL						0x01

//OBSERVE_TX register bitwise definitions
#define nrf24l01_OBSERVE_TX_PLOS_CNT	0xF0
#define nrf24l01_OBSERVE_TX_ARC_CNT		0x0F

//CD register bitwise definitions
#define nrf24l01_CD_RESERVED	0xFE
#define nrf24l01_CD_CD			0x01

//RX_PW_P0 register bitwise definitions
#define nrf24l01_RX_PW_P0_RESERVED	0xC0

//RX_PW_P0 register bitwise definitions
#define nrf24l01_RX_PW_P0_RESERVED	0xC0

//RX_PW_P1 register bitwise definitions
#define nrf24l01_RX_PW_P1_RESERVED	0xC0

//RX_PW_P2 register bitwise definitions
#define nrf24l01_RX_PW_P2_RESERVED	0xC0

//RX_PW_P3 register bitwise definitions
#define nrf24l01_RX_PW_P3_RESERVED	0xC0

//RX_PW_P4 register bitwise definitions
#define nrf24l01_RX_PW_P4_RESERVED	0xC0

//RX_PW_P5 register bitwise definitions
#define nrf24l01_RX_PW_P5_RESERVED	0xC0

//FIFO_STATUS register bitwise definitions
#define nrf24l01_FIFO_STATUS_RESERVED	0x8C
#define nrf24l01_FIFO_STATUS_TX_REUSE	0x40
#define nrf24l01_FIFO_STATUS_TX_FULL	0x20
#define nrf24l01_FIFO_STATUS_TX_EMPTY	0x10
#define nrf24l01_FIFO_STATUS_RX_FULL	0x02
#define nrf24l01_FIFO_STATUS_RX_EMPTY	0x01

// RF power definitions
#define nrf24l01_DR_LOW 0x20
#define nrf24l01_DR_MED 0x00
#define nrf24l01_DR_HIGH 0x08

// Send no data for when you read data from the radio
#define nrf24l01_SEND_CLOCK             0x00

// State Definitions
#define PWR_DOWN 0
#define STANDBY_1 1
#define RX_MODE 2
#define TX_MODE 3


char status;
char config;
char buffer[120];
volatile char RX_payload[32];
char payload_size;
int pipe_no; // pipe most recent payload was received from
int width; // width of most recent payload in bytes

int state;

volatile int received; // goes high when message is received
volatile int sent; // goes high after radio finishes sending payload correctly
volatile int error; // goes high when no acknowledge is received

int TX; // is it transmitter or receiver (0 is rx 1 is tx)

/**
 * @brief Transfer and receive a byte over SPI
 * 
 * @param c What to transfer.
 * @return What was received.
 */
char rf_spiwrite(unsigned char c);

/**
 * @brief Set up SPI and interrupt for radio.
 */
void init_SPI();

/**
 * @brief Sets up the radio, spi, and interrupts.  Also resets all radio 
 * registers to their default values.
 */
void nrf_setup();

/**
 *@brief Read a register and store the data in an array. Can be multiple bytes 
 * of data.
 *
 * @param reg What register to read from.  Use constants in nrf24l01.h
 * @param buff Pointer to array to store data. LSB first.
 * @param len How many bytes of data need to be read. 1 to 5 bytes.
 */
void nrf_read_reg(char reg, char * buff, int len);

/**
 *@brief Write to a register from an array. Can be multiple bytes of data.
 *
 * @param reg What register to read from.  Use constants in nrf24l01.h
 * @param buff Pointer to array to write data from. LSB first.
 * @param len How many bytes of data to be written. (1-5bytes)
 */
void nrf_write_reg(char reg, char * data, char len);

/**
 * @brief Flush the TX FIFO.
 */
void nrf_flush_tx();

/**
 * @brief Flush the RX FIFO.
 */
void nrf_flush_rx();

/**
 * @brief Get the width of the top payload in the RX FIFO.
 * 
 * This function is only meant for use with dynamic payload lengths.  It must be
 * called before reading the payload because the payload is removed from the 
 * FIFO when reading.
 * 
 * @return The width of the payload in bytes.
 */
int nrf_get_payload_width();

/**
 * @brief Write a payload to the TX FIFO.
 *
 * @param data Pointer to data to be written.
 * @param len How many bytes of data to be written. (1-32bytes)
 */
void nrf_write_payload(char * data, char len);

/**
 * @brief Read a payload from the RX FIFO.
 *
 * @param buff Pointer to array where data will be written.
 */
void nrf_read_payload(char * buff);

/**
 * @brief Checks if a payload was received and clears the flag signaling so.
 * 
 * @return 1 if a payload was received and is available to be read, 0 if not.
 */
int nrf_payload_available();

/**
 * @brief Get the number of the pipe the most recent payload was received on.
 * 
 * @return The number of the pipe the most recent payload was received on.
 */
int nrf_get_pipe();

/**
 * @brief Get the width of the most recently received payload.
 * 
 * @return Width of the payload in bytes.
 */
int nrf_get_payload_width();

/**
 * @brief Sets the power up bit in the status register to leave the power down 
 * state.
 */
void nrf_pwrup();

/**
 * @brief Clears the power up bit in the status register to enter the power 
 * down state.
 */
void nrf_pwrdown();

/**
 * @brief Put the radio in the power down state.
 */
void nrf_state_pwr_down();

/**
 * @brief Put the radio in the standby 1 state.
 */
void nrf_state_standby_1();

/**
 * @brief Put the radio in the rx mode state.
 */
void nrf_state_rx_mode();

/**
 * @brief Set the PRIM_RX bit in the CONFIG register.
 */
void nrf_set_prim_rx();

/**
 * @brief Clear the PRIM_RX bit in the CONFIG register.
 */
void nrf_clear_prim_rx();

/**
 * @brief Set power of transmitter.
 *
 * @param power Power level to set transmitter to.  Possible values are:
 * 0dBm: nrf24l01_RF_SETUP_RF_PWR_0
 *-6dBm: nrf24l01_RF_SETUP_RF_PWR_6
 *-12dBm: nrf24l01_RF_SETUP_RF_PWR_12
 *-18dBm: nrf24l01_RF_SETUP_RF_PWR_18
 */
void nrf_set_transmit_pwr(char power);


/**
 * @brief Set data rate.
 *
 * @param rate Rate to set transmitter to.  Possible values are:
 * 250 kbps: nrf24l01_DR_LOW
 * 1 Mbps: nrf24l01_DR_MED
 * 2 Mbps: nrf24l01_DR_HIGH
 */
void nrf_set_transmit_rate(char rate);

/**
 * @brief Set the auto retransmit delay.
 * 
 * Set how long the nrf24l01 should wait between retransmitting packets after
 * not receiving an acknowledgement packet. Delay is defined as the end of one 
 * transmission to the start of the next. The delay is set according to the 
 * equation, delay = 250 + ard * 250 (us).
 * 
 * @param ard The auto retransmit delay
 */
void nrf_set_ard(char ard);

/**
 * @brief Set the auto retransmit count.
 * 
 * Set how many times the nrf24l01 should attempt to retransmit the packet after
 * not receiving an acknowledgment packet.
 * 
 * @param arc How many times to try retransmitting.
 */
void nrf_set_arc(char arc);

/**
 * @brief Set the RF frequency the radio will operate at.
 * 
 * The nrf24l01 can operate at frequencies ranging from 2.400GHz to 2.525 GHz.
 * At 250 kbps or 1 Mbps the radio occupies less than a 1MHz bandwidth. At 
 * 2 Mbps the radio occupies less than a 2 MHz bandwidth. The frequency is set 
 * according to the equation, frequency = 2400 + ch (MHz). The transmitter and 
 * receiver must be set operate at the same frequency to communicate.
 * 
 * @param ch The center of the channel used by the nrf24l01
 */
void nrf_set_rf_ch(char ch);

/**
 * @brief Returns the pipe data is available in.
 * 
 * Before reading the payload out of the FIFO this function checks which pipe
 * the data was received in.  If 0b111 is returned the FIFO is empty and no data
 * was received.
 * 
 * @return Pipe data is available in
 */
char nrf_received_pipe_num();

/**
 * @brief Send a payload over the radio.
 *
 * Sends out a specified payload (in auto acknowledge mode by default)
 * use after powering up radio, and setting address or other settings
 * 
 * @param data Pointer to data to be sent.
 * @param len How many bytes of data to be sent. (1-32bytes)
 */
//void nrf_send_payload(char * data, int len);

/**
 * @brief Set the address width of RX and TX pipes
 * 
 * Sets the address width of all pipes.  LSB of the address set in the
 * RX_ADDR_PX or TX_ADDR registers are used as the address for the associated
 * pipe.
 * 
 * @param width The address width in bytes(3-5 bytes)
 */
void nrf_set_address_width(char width);

/**
 * @brief Send a constant carrier wave out at specified power.
 * 
 * Send a continuous carrier wave for testing purposes. Wave can be stopped by
 * calling nrf_stop_cont_wave.  
 * 
 * @param pwr Power level to set transmitter to.  Possible values are:
 * 0dBm: nrf24l01_RF_SETUP_RF_PWR_0
 *-6dBm: nrf24l01_RF_SETUP_RF_PWR_6
 *-12dBm: nrf24l01_RF_SETUP_RF_PWR_12
 *-18dBm: nrf24l01_RF_SETUP_RF_PWR_18
 */
//void nrf_start_cont_wave(char pwr);

/**
 * @brief Stop sending the carrier wave.
 */
void nrf_stop_cont_wave();

/**
 * @brief Check the power of the signal the nrf42l01 is receiving
 * 
 * RX mode must be enables for at leat 170 us before measurements will be 
 * accurate. 0 will be returned if power level is below -64dB and 1 will be 
 * returned if power level is above -64dB.
 * 
 * @return 1 If power level is above -64dB.
 */
char nrf_received_pwr();

/**
 * @brief Enable auto-acknowledge for a pipe.
 * 
 * @param pipe Which pipe to enable autoack on. Pipes range from 0 to 5. 
 */
void nrf_en_aa(int pipe);

/**
 * @brief Disable auto-acknowledge for a pipe.
 * 
 * @param pipe Which pipe to disable autoack on. Pipes range from 0 to 5.
 */
void nrf_dis_aa(int pipe);

/**
 * @brief Enable a pipe to receive packets.
 * 
 * The nrf24l01+ contains six parallel pipes that can receive packets from six 
 * different transmitters.  Each pipe must have a unique address.
 * 
 * @param pipe Which pipe to enable. Pipes range from 0 to 5.
 */
void nrf_en_rxaddr(int pipe);

/**
 * @brief Disable a pipe from receiving packets.
 * 
 * @param pipe Which pipe to disable. Pipes range from 0 to 5.
 */
void nrf_dis_rxaddr(int pipe);

/**
 * @brief Set the width received static payloads should be.
 * 
 * When using static payload widths the width of packets must be explicitly set.
 * The receiver's width must be set to the size of the payloads being 
 * transmitted. Use this function to set the width on the receiver.
 * 
 * @param width Width to set pipe to. 1-32 bytes.
 * 
 * @param pipe Which pipe to set the payload width of.
 */
void nrf_set_pw(char width, int pipe);

/**
 * @brief Enable dynamic payload length for a pipe.
 * 
 * If dynamic payload length is enabled the amount of bytes in a packet does not
 * need to be specified.  For this feature to work the transmitter must 
 * have dynamic payload length enabled for pipe 0 and the receiver must have 
 * dynamic payload length enabled for the pipe it will receive from this 
 * transmitter on.  In addition auto acknowledge must be enabled for the two 
 * pipes and EN_DPL must be set in the FEATURE register both of which are done 
 * automatically.
 * 
 * @param pipe Which pipe to enable dynamic payload length on.
 */
void nrf_en_dpl(int pipe);

/**
 * @brief Disable dynamic payload length for a pipe.
 * 
 * If dynamic payload length is disabled the length of a packet must be set 
 * using the nrf_set_pw function.
 * 
 * @param pipe Which pipe to disable dynamic payload length on.
 */
void nrf_dis_dpl(int pipe);

/**
 * @brief Enable dynamic auto-acknowledgements.
 * 
 * Enables sending payloads without using auto-acknowlegment without disabling 
 * the auto-acknowledge setting on the transmitter or receiver.  The SENDNOACK
 * function can then be used to do this.
 */
void nrf_en_dyn_ack();

/**
 * @brief Disable dynamic auto-acknowledgements.
 * 
 * Disables sending payloads without using auto-acknowlegment without disabling 
 * the auto-acknowledge setting on the transmitter or receiver.
 */
void nrf_dis_dyn_ack();



/**
 * @brief Set the address of a pipe.
 * 
 * The nrf24l01 has six pipes to receive on.  Pipes have addresses
 * that are 3-5 bytes long.  Pipes 0 and 1 can have all 5 bytes of their 
 * addresses changed.  The other four pipes can only have the LSB of their 
 * address changed.  The four MSBs of the other four pipes are the MSBs of the 
 * address of pipe 1.  The address width must be set using nrf_set_address_width
 * before calling this function.  If the address width set does not match the
 * length specified in this function 0 will be returned and the address will not
 * be written.  0 will also be returned if length is not set to 1 when writing
 * to pipes 2-5.  
 * 
 * @param pipe The pipe whose address will be set. Pipes range from 0-5.
 * @param address Address pipe will be set to.
 * @param len How many bytes long the address is. Length ranges from 0-5 bytes.
 * @return Returns 1 if address was written correctly.  Returns 0 if address was
 * written incorrectly.  If 0 is returned address was not written.
 */
int nrf_set_rx_addr(int pipe, uint64_t address, int len);

/**
 * @brief Set the address for transmitting.
 * 
 * The address of the transmitter should match the address of the receiver or
 * packets will not be received.  If auto acknowledge is enabled rx pipe 0 on 
 * the transmitter must be set to the same address as the tx address set in this 
 * function.
 * 
 * @param address Address to set. 5 bytes.
 */
void nrf_set_tx_addr(uint64_t address);

//Helper function for parsing addresses into a buffer
char * parse_addr(uint64_t address);

/**
 * @brief Enables acknowledge packets to carry payloads.
 */
void nrf_en_ack_pay();

/**
 * @brief Disables acknowledge packets to carry payloads.
 */
void nrf_dis_ack_pay();

/**
 * 
 * @param data
 * @param len
 */
int nrf_send_payload(char * data, char len);

/**
 * @brief Resets all registers to their default values as listed on the 
 * datasheet.
 */
void nrf_reset();

/**
 * @brief Send a payload without using auto-acknowledge.
 * 
 * @param data The payload to be sent
 * @param len The length of the payload in bytes.
 * @return 1 if payload was successfully sent 0 if not.
 */
int nrf_send_payload_nonblock(char * data, char len);

/**
 * @brief Send a payload with auto-acknowledge enabled.
 * 
 * @param data The payload to be sent.
 * @param len The length of the payload in bytes
 * @return 1 if payload was successfully sent and acknowledged 0 if payload was
 * not acknowledged.
 */
int nrf_send_payload(char * data, char len);