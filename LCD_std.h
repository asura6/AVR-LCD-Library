/* Author: Robin Isaksson, 2016. robin.isaksson|rainfoo.com
 * NOTE: 8-bit mode is broken as of now.
 */
#ifndef LCD_header
#define LCD_header 
/* ============================
 * DEVICE SPECIFIC SETUP BELOW 
 * ============================
 * Specify: 
 * 4-bit or 8-bit mode
 * MCU data-bus port
 * MCU DDR and port and and pins for Register Select, Read-Write and Enable pins
 * Pin-number for LCD Register-select, Read-write and Enable
 *
 * In 4-bit mode it is possible to use only one port, ex: RS, RW, E on D0, D1,
 * D2 and data-pins as D4, D5, D6, D7. In 8-bit mode all 8 outputs on a port are
 * reserved as data pins and RS, RW, and Enable must occupy a different port.
 */
#define LCD_BIT_INTERFACE 4 //4-bit interface or 8-bit interface 
#define LCD_DDR DDRD    //MCU Data Direction Port
#define LCD_PORT PORTD  //MCU Data Port, must be same as LCD_DDR
#define LCD_CTRL_DDR DDRD //MCU DDR Port for RS, RW and Enable pins
#define LCD_CTRL PORTD  //MCU Port for RS, RW and Enable pins
#define LCD_RS PIND0    //RS (Register Select) pin (H: data, L: Instruction)
#define LCD_RW PIND1    //RW (Read-Write) pin (H: read, L: write)
#define LCD_E PIND2     //Enable pin 
/* =====================
 * END OF DEVICE SPECIFIC SETUP
 * ===================== */

#if LCD_BIT_INTERFACE == 4
#include "./LCD_4bit.h"
#else
#include "./LCD_8bit.h"
#endif
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h> 

void Init_Command(char command);
void LCD_Wait_Until_Ready();
void LCD_Flash_Enable_Signal();
void LCD_Send_Command(char command);
void LCD_Send_Character(char character);
void LCD_Send_String(char *string, unsigned char col, unsigned char row);
void LCD_Move_Cursor(unsigned char col, unsigned char row);
void LCD_Register_Update(char RS, char RW);

/* Wait until LCD is ready if it is busy */ 
void LCD_Wait_Until_Ready() { 
    LCD_PORT &= 0x0F; //Data pins as inputs 
    LCD_Register_Update(0, 1); 

    while (LCD_PORT >= 0x80 ) { //True until pin7 is 0
        LCD_Flash_Enable_Signal();
    } 
    LCD_Register_Update(0, 0);
    LCD_DDR |= 0xF0; //Data pins write mode 
}

/* Flash enable signal/flip latch */ 
void LCD_Flash_Enable_Signal() {
    LCD_CTRL |= 1<<LCD_E; //Enable on
    asm volatile ("nop"); //Assembly, wait clockcycle
    asm volatile ("nop"); 
    LCD_CTRL &= ~(1<<LCD_E); //Enable off
}

/* Send a string of characters to the LCD, specifying the column and row where
 * the string should start.
 * Example: LCD_Send_String("Hello world.", 0, 0) appears at the top left corner
 */
void LCD_Send_String(char *string, unsigned char col, unsigned char row) {
    LCD_Move_Cursor(col, row);
    while (*string != 0x00) {
        LCD_Send_Character(*string++);
    }
}

/* Move the cursor by specifying column and row. This function is most useful
 * for internal function-calls. 
 * */
void LCD_Move_Cursor(unsigned char col, unsigned char row) {
    LCD_Send_Command(0x80 + col + row*64) ;
}

/* Send an integer to the LCD, specifying the column and row where the integer
 * should start. 
 */
void LCD_Send_Integer(int integer, unsigned char col, unsigned char row) {
    //Maximum 6 digits with prepended sign
    //ex -100000
    char integer_string[8] = {0, 0, 0, 0, 0, 0, 0, 0}; 
    itoa(integer, integer_string, 10);
    LCD_Send_String(integer_string, col, row); 
}

/* Send a double (decimal number) to the LCD, specifying the column and row
 * where the double should start. 
 */ 
void LCD_Send_Double(double decimal, unsigned char col, unsigned char row) {
    //Maximum 5 digits with prepended sign and decimal mark
    //ex -132.55
    char decimal_string[8] = {0, 0, 0, 0, 0, 0, 0}; 
    dtostrf(decimal, 7, 3, decimal_string);
    LCD_Send_String(decimal_string, col, row); 
} 

/* Change Register-Select and Read-Write registers */
void LCD_Register_Update(char RS, char RW) { 
    LCD_CTRL ^= (-RS ^ LCD_CTRL) & (1 << LCD_RS);
    LCD_CTRL ^= (-RW ^ LCD_CTRL) & (1 << LCD_RW); 
}

#endif
