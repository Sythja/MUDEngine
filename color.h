/**************************************************************************/
// color.h - Dawn color system
/***************************************************************************
 * The Dawn of Time v1.69s.beta6 (c)1997-2010 Kalahn                       *
 * >> A number of people have contributed to the Dawn codebase, with the   *
 *    majority of code written by Kalahn - www.dawnoftime.org              *
 * >> To use this source code, you must fully comply with the dawn license *
 *    in licenses.txt... In particular, you may not remove this copyright  *
 *    notice.                                                              *
 **************************************************************************/
#ifndef COLOR_H
#define COLOR_H

#ifndef COLORCODE
#define COLORCODE '`'
#endif

// color code in string form
#ifndef CCSTR
#define CCSTR "`"
#endif
 #ifndef CC
#define CC "`"
#endif

enum COLOR_TYPE {CT_NOCOLOR, CT_ANSI, CT_IRC, CT_IRCWHITE, CT_HTML, CT_MXP, CT_BRIGHT, CT_AUTODETECT}; 

#define MAX_SAVED_COLOR_ARRAY	(15)	// number of colors that the color saving can store

struct color_codes{
	unsigned char code;
	char *name;	
	bool special;
	bool dont_repeat;
	// different codings
	char nocolor[5];
	char ansi[8];
	char irc[7]; 
	char ircWhite[7];  // white background
	char html[61];
	char mxp[61];
	char bright[61];

	char original_ansi_character;
	
};

struct COLOR_TEMPLATE_TYPE;
struct COLOR_MEMORY_TYPE // one per descriptor
{
	unsigned char current;
	unsigned char saved[MAX_SAVED_COLOR_ARRAY];
	unsigned char saved_index;
	unsigned char saved_before_helplink; // the color which after a helplink, the system reverts to
	char *custom_color;
	COLOR_TEMPLATE_TYPE *color_template;
	bool in_help_link;
	bool flashing_disabled;
};

//prototypes
void initColor();
void color_generate_html_table();
int convertColor(const char *src, char *dest, COLOR_TYPE cType, bool partial_html);
class connection_data;
//char *process_color(const char *raw_text, color_TYPE cType, color_MEMORY_TYPE *cm, connection_data *d);
char *process_color(const char *raw_text, connection_data *d);
char *strip_color(const char *colored_text);

struct color_table_type{char code[10];};
extern const struct color_table_type color_table[];

#endif
