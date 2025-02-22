#define DISPLAY_ADDRESS 0x78

// Kind of a prelude
#define CHARGE_PUMP_SETTING 0x8D
#define ENABLE_CHARGE_PUMP 0x14
#define DISPLAY_ON_NORMAL_MODE 0xAF
// Prelude End

#define DISPLAY_FULL_ON 0xA5 

// Control bytes
#define CO_CTRL 0x00
#define CO_DATA 0x40
#define SINGLE_CTRL 0x80
#define SINGLE_DATA 0xC0

// Write modes
#define RIGHT_HORIZONTAL_SCROLL 0x26    
#define VERTICAL_RIGHT_HORIZONTAL_SCROLL 0x29

void clear_screen();
void clear_row();
void newline();
void printc(char c);
