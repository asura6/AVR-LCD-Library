#ifndef LCD_demo
#define LCD_demo
void Init_Command(char command);
void LCD_Send_Command(char command);
void LCD_Flash_Enable_Signal();
void LCD_Wait_Until_Ready();
void LCD_Send_Character(char character); 
void LCD_Register_Update(char RS, char RW); 
void LCD_Send_String(char *string, unsigned char col, unsigned char row);
void LCD_Move_Cursor(unsigned char col, unsigned char row);
void LCD_Send_Double(double decimal, unsigned char col, unsigned char row); 
void LCD_Send_Integer(int integer, unsigned char col, unsigned char row); 

/* Tries out the LCD-functions and displays every character in memory */
void LCD_Demo() {
    unsigned int character = 0x00;
    unsigned int j = 0;
    unsigned int k = 0;
    char demo_string1[] = "Function demo";
    char demo_string2[] = "Incredible!";
    while (1) {
        if (character == 0x00) {
            k = 0;
            LCD_Move_Cursor(0,0);
            while (demo_string1[j] != 0x00) {
                LCD_Send_Character(demo_string1[j]);
                j++;
                _delay_ms(350);
            }
            j = 0;
            _delay_ms(1000);
            LCD_Send_String(demo_string2, 0, 1);
            _delay_ms(1500); 
            LCD_Send_String("                ", 0, 0);
            LCD_Send_String("Fantastic!", 0, 0); 
            _delay_ms(3000);
            LCD_Send_String("                ", 0, 1);
            LCD_Send_String("                ", 0, 0);
            LCD_Move_Cursor(0,0);
        } 
        LCD_Send_Character(character);
        character++;
        k++;
        _delay_ms(125); 
        if (k == 16) {
            LCD_Move_Cursor(0, 1);
        } else if (k == 32) {
            LCD_Move_Cursor(0, 0);
            k = 0;
        }

        if (character == 0xFF) { 
            LCD_Send_String("                ", 0, 1);
            LCD_Send_String("                ", 0, 0); 
            LCD_Send_String("Integer numbers!", 0, 0);
            _delay_ms(1000);
            LCD_Send_Integer(12345, 0, 1);
            _delay_ms(3000);
            LCD_Send_String("                ", 0, 1);
            LCD_Send_String("                ", 0, 0); 
            LCD_Send_String("Decimal numbers!", 0, 1);
            _delay_ms(1000);
            LCD_Send_Double(123.456, 0, 0);
            _delay_ms(3000); 
            character = 0x00;
            LCD_Send_String("                ", 0, 1);
            LCD_Send_String("                ", 0, 0); 
        }
    }
}
#endif
