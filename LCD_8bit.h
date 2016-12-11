/* Auxillary function-header which provides unique 8-bit mode functions */ 
void LCD_Initialize(); 
void LCD_Send_Command(char command);
void LCD_Flash_Enable_Signal();
void LCD_Wait_Until_Ready();
void LCD_Send_Character(char character); 
void LCD_Register_Update(char RS, char RW);

void LCD_Initialize() {
    LCD_CTRL_DDR |= 1<<LCD_RS | 1<<LCD_RW | 1<<LCD_E; 
    _delay_ms(20);
    LCD_Send_Command(0x38); // 8 bit mode 
    LCD_Send_Command(0x0E); // Clear screen 
    LCD_Send_Command(0x01); // Display on cursor on 
    _delay_ms(2);
}

void LCD_Send_Command(char command) { 
    LCD_Wait_Until_Ready();
    LCD_Register_Update(0, 0); //Instruction register, Write mode
    LCD_DATA_PORT = command;
    LCD_Flash_Enable_Signal(); 
}

void LCD_Send_Character(char character) { 
    LCD_Wait_Until_Ready();
    LCD_Register_Update(0, 0); //Data register, Write mode 
    LCD_DATA_PORT = character;
    LCD_Flash_Enable_Signal(); 
} 
