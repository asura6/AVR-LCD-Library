/* Auxillary function-header which provides unique 4-bit mode functions */
void LCD_Initialize(char N, char C, char B, char I_D, char S);
void Init_Command(char command);
void LCD_Send_Command(char command);
void LCD_Flash_Enable_Signal();
void LCD_Wait_Until_Ready();
void LCD_Send_Character(char character); 
void LCD_Register_Update(char RS, char RW); 

/* 4-bit mode initialization 
 * Arguments: 
 * N: line number, 1 = more than one line, 0 = one line display 
 * C: Cursor on or off, 1 = on, 0 = off 
 * B: Cursor position on or off, 1 = on, 1 = off
 * I_D: Increment/Decrement cursor, 1 = right, 0 = left 
 * S: Display shift, 1 = on, 0 = off
 */ 
void LCD_Initialize(char N, char C, char B, char I_D, char S) { 
    LCD_CTRL_DDR |= (1<<LCD_RS) | (1<<LCD_RW) | (1<<LCD_E); //Outputs 
    LCD_DDR |= 0xF0; //Data as output 
    /* 4-bit initialization sequence */
    _delay_ms(20); 
    Init_Command(0x30); 
    _delay_ms(5);
    Init_Command(0x30);
    _delay_ms(1);
    Init_Command(0x30);
    _delay_ms(1);
    Init_Command(0x20);
/* 4-bit interface is initialized, change of command structure */
    LCD_Wait_Until_Ready(); 
    LCD_Send_Command(0x20 | N<<3); //Use 4 bit interface, N lines 
    LCD_Send_Command(0x0C | C<<1 | B); //Display on, C cursor, B cursor pos.
    LCD_Send_Command(0x01); //Clear display
    LCD_Send_Command(0x04 | I_D <<1 | S); //Increment and shift settings
    _delay_ms(5); 
}

/* Command-function to use before 4-bit mode initialized */
void Init_Command(char command) { 
    LCD_Register_Update(0, 0); //Instruction register, Write mode
    LCD_PORT &= ~(0xF0);
    LCD_PORT |= (command & 0xF0);
    LCD_Flash_Enable_Signal();
}

/* Send command in 4-bit interface */ 
void LCD_Send_Command(char command) {
    LCD_Wait_Until_Ready();
    LCD_Register_Update(0, 0); //Instruction register, Write mode 
    LCD_PORT &= 0x0F; 
    LCD_PORT |= (command & 0xF0); 
    LCD_Flash_Enable_Signal(); 
    LCD_PORT &= 0x0F; 
    LCD_PORT |= ((command & 0x0F) << 4);
    LCD_Flash_Enable_Signal(); 
}
/* Send character in 4-bit interface */
void LCD_Send_Character(char character) { 
    LCD_Wait_Until_Ready();
    LCD_Register_Update(1, 0); //Data register, Write mode 
    LCD_PORT &= 0x0F; 
    LCD_PORT |= (character & 0xF0); 
    LCD_Flash_Enable_Signal(); 
    LCD_PORT &= 0x0F; 
    LCD_PORT |= ((character & 0x0F) << 4);
    LCD_Flash_Enable_Signal(); 
} 
