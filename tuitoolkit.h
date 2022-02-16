//	file: tuitoolkit.h
#ifndef tuitoolkit_h
#define tuitoolkit_h

//#include "tuitoolkit_internal.h"
#include <ncurses.h>

/*
tuiToolkit - small TUI widget library



HOW TO USE

To make a window use tuiWindow type and then initiate it with tuiInitScreen function by passing a pointer:
```
	tuiWindow example;
	tuiInitWindow(&example, x, y, width, height, title, decorations, colorpalette);
```

tuiWindow data structure:
	variables:
	[default]   [name]      [description]
	0			posX      - window x position from upper left corner measured in characters
	0			posY      - window y position from upper left corner measured in characters
	30			width     - window width measured in characters
	15			height    - window height measured in characters
	"window"	title     - title shown on the title bar
	3			decoration - window decorations flag:
							0 - no decorations
							1 - frame only
							2 - title only
							3 - background only (no background - blank terminal color)
							4 - frame + title
							5 - frame + background
							6 - background + title
							7 - all decorations
			
	0			palette   - color palette:
								[fill]       [widgets]
							0 - light		 blue
							1 - light		 green
							2 - light		 red
							3 - dark         blue
							4 - dark         green
							5 - dark         red

int tuiInitWindow:
	return values:
		0 - window generated successfully
		1 - terminal has no color support
		2 - invalid color palette

*/

#define TUI_ERR_INVALID_ARGUMENT  1
#define TUI_ERR_BAD_TERMINAL      2

//screen struct
typedef struct {
	int screenWidth;
	int screenHeight;
	int focusedWindow;
}tuiScreen;

//window struct
typedef struct {
	int posX;
	int posY;
	int width;
	int height;
	char *title;
	char decoration;
	char palette;
	char extendedSet;
	//ncurses window pointer
	WINDOW *wndPtr;
}tuiWindow;

/* widgets */
//label
typedef struct {
	int posX;
	int posY;
	char *text;
	//pointer to parent window
	tuiWindow *tWndPtr;
}tuiWidgetLabel;
//button
typedef struct {
	int posX;
	int posY;
	char *text;
	char highlighted;
	//pointer to parent window
	tuiWindow *tWndPtr;
}tuiWidgetButton;
//checkbox
typedef struct {
	int posX;
	int posY;
	char *text;
	char highlighted;
	char ticked;
	//pointer to parent window
	tuiWindow *tWndPtr;
}tuiWidgetCheckbox;
//scrollable list
typedef struct {
	int posX;
	int posY;
	unsigned int selection;
	unsigned int scroll;
	char *(*values)[];// pointer to the array of values
	unsigned int listLength;
	char highlighted;
	unsigned int width;
	unsigned int height;
	//pointer to parent window
	tuiWindow *tWndPtr;
}tuiWidgetList;
//dropdown
typedef struct {
	int posX;
	int posY;
	unsigned int selection;
	char *(*values)[];//'values' won't store the list of values, only pointer
	char highlighted;
	char dropped;
	unsigned int maxLength;
	//pointer to parent window
	tuiWindow *tWndPtr;
}tuiWidgetDropdown;
//window init macro
//#define initWindow(wnd) tuiWindow wnd = {.posX = 0, .posY = 0, .width = 30, .height = 15, .title = "window", .decoration = 7, .palette = 0}

void tuiInitLabel(tuiWindow *tWnd, tuiWidgetLabel *tLabl, unsigned int posX, unsigned int posY, char *label);
void tuiInitButton(tuiWindow *tWnd, tuiWidgetButton *tButton, unsigned int posX, unsigned int posY, char *label);
void tuiInitCheckbox(tuiWindow *tWnd, tuiWidgetCheckbox *tButton, unsigned int posX, unsigned int posY, char *label);
void tuiInitList(tuiWindow *tWnd, tuiWidgetList *tList, unsigned int listLength, unsigned int posX, unsigned int posY, char *(*values)[], unsigned int width, unsigned int height);

/*
void tuiInitDropdown(tuiWindow *tWnd, tuiWidgetDropdown *tDropdown, unsigned int posX, unsigned int posY, char *(*values)[], unsigned int mLen);
*/


/* draw widget */

void _drawLabel(tuiWidgetLabel *tWdg);
void _drawButton(tuiWidgetButton *tWdg);
void _drawCheckbox(tuiWidgetCheckbox *tWdg);
void _drawList(tuiWidgetList *tWdg);
#define tuiDrawWidget(wdgt) _Generic (wdgt, tuiWidgetLabel*: _drawLabel, tuiWidgetButton*: _drawButton,tuiWidgetList*: _drawList, tuiWidgetCheckbox*: _drawCheckbox, tuiWidgetList: _drawList)(wdgt)
//initiate window
void tuiInitWindow(tuiWindow *tWnd, unsigned int posX, unsigned int posY, unsigned int wdt, unsigned int hgt, char *title, char decor, char palt, char set);
//draw window on screen
void tuiDrawWindow(tuiWindow *tWnd);
/*
//move window
void tuiMoveWindow(tuiWindow *tWnd, unsigned int posX, unsigned int posY);
*/
//end window
void tuiEndWindow(tuiWindow *tWnd);

/* screens */

//init screen macro
//#define initScreen(scr, wdth, hght) tuiScreen scr = {.screenWidth = wdth, .screenHeight = hght, .focusedWindow = 0}

//initiate screen
int tuiInitScreen();
//end screen
void tuiEndScreen();


#endif

