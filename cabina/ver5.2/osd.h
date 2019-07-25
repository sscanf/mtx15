#include <hc05c9_r.h>

#define BIT_OSD_MUTE    1
#define BIT_OSD_DATA    2
#define BIT_OSD_CLOCK	4
#define BIT_OSD_CSU     8
#define OSD_CTRL_PORT	PORTB
#define OSD_SERIAL_PORT	PORTB


#define TEXT_COLOR_BLACK	0x0
#define TEXT_COLOR_BLUE		0x100
#define TEXT_COLOR_GREEN	0x200
#define TEXT_COLOR_CYAN		0x300
#define TEXT_COLOR_RED		0x400
#define TEXT_COLOR_MAGENTA	0x500
#define TEXT_COLOR_YELLOW	0x600
#define TEXT_COLOR_WHITE	0x700	
#define TEXT_BACKGROUND		0x800
#define TEXT_BLINK			0x80

#define MODE_BLINK_OFF		0
#define MODE_BLINK_75		1
#define MODE_BLINK_5		2
#define MODE_BLINK_25		3

void OSD_Reset (void);
void OSD_ScreenPos (char hor, char vert);
void OSD_TextPos (char x, char y);
void OSD_PrintStr (char *str);
void OSD_PrintChar (char ch,char mode);
void OSD_FullMode (char mode);
void OSD_CharMode (char mode);
void OSD_DisplayOn (char display);
void OSD_ColorMode (char mode);
void OSD_BlinkOption (char option);
void OSD_BlinkPeriod (char period);
void OSD_SyncMode (char mode);
void OSD_StandarType (char type);
void OSD_ColorEncoder (char encoder);
void OSD_LuminanceLevels (char levels);
void OSD_VideoFullPageMode (char mode);
void OSD_PrintExChar (char ch, char mode);
