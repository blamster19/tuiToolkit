//	file: tuitoolkit.h
#ifndef tuitoolkit_h
#define tuitoolkit_h

#include <stdlib.h>
#include <ncurses.h>

/*
tuiToolkit - small TUI widget library


---
HOW TO USE
---

tuiToolkit currently relies on ncurses library for displaying the user interface in a terminal.
To compile properly, you have to provide an appropriate compiler flag, for gcc it's -lncursesw
Make sure you have libncurses5-dev installed on your system

To begin working with the library, first initiate screen using function:
```
	tuiInitScreen();
```

To make a window use tuiWindow type and then initiate it with tuiInitScreen function by passing an address:
```
	tuiWindow example;
	tuiInitWindow(&example, x, y, width, height, title, decorations, colorpalette);
```

tuiWindow parameters:
	[type]		   [name]      [description]
	tuiWindow		tWnd  	   - window structure address
	unsigned int	posX       - window x position from upper left corner measured in characters
	unsigned int	posY       - window y position from upper left corner measured in characters
	unsigned int	width      - window width measured in characters
	unsigned int	height     - window height measured in characters
	char*			title      - title shown on the title bar
	char			decoration - window decorations flag:
								 0 - no decorations
								 1 - frame only
								 2 - title only
								 3 - background only (no background - blank terminal color)
								 4 - frame + title
								 5 - frame + background
								 6 - background + title
								 7 - all decorations
			
	char			palette	   - color palette:
								[fill]       [widgets]
								 0 - light		 blue
								 1 - light		 green
								 2 - light		 red
								 3 - dark        blue
								 4 - dark        green
								 5 - dark        red
	char			extendedSet- use extended character set [UNIMPLEMENTED]	
								 0 - off
								 1 - on

tuiWindow return values:
		0 - window generated successfully
		1 - terminal has no color support
		2 - invalid color palette

---

To add a widget to the window create an instance of respective widget and initiate it using a function:
```
	tuiWidgetWIDGET instance;
	tuiInitWIDGET(&example, &instance, ...);
```
where:
	WIDGET - widget type:
			Label 	 - plain single-line text
			Button 	 - selectable and clickable button
			Checkbox - selectable and settable checkbox
			List 	 - scrollable list of selectable positions

arguments of init functions:
tuiInitLabel(Window, Widget, posX, posY, text)
tuiInitButton(Window, Widget, posX, posY, text)
tuiInitCheckbox(Window, Widget, posX, posY, text)
tuiInitList(Window, Widget, posX, posY, list, length, width, height)

parameters:
	[type] 			[name] 		[description]
	tuiWindow 		Window 		address of tuiWindow structure
	/WIDGET/ 		Widget 		address of respective widget
	unsigned int 	posX 		widget x position from upper left corner measured in characters
	unsigned int 	posY 		widget Y  position from upper left corner measured in characters
	*char 			text 		text to be displayed
	*char[] 		list 		array of strings; each string is one selectable option
	unsigned int 	length  	length of said array
	unsigned int 	width 		widget width measured in characters
	unsigned int 	height 		widget height measured in characters

where /WIDGET/ is one of the following types: tuiWidgetLabel, tuiWidgetButton, tuiWidgetCheckbox, tuiWidgetList
---

To print a window on screen use tuiDrawWindow:
```
	tuiDrawWindow(&example);
```
where &example is an address of the window structure

To destroy window use tuiEndWindow:
```
	tuiEndWindow(&example);
```
where &example is an address of the window structure

To end working with the library and safely exit ncurses use tuiEndScreen:
```
	tuiEndScreen();
```
*/

#define TUI_ERR_INVALID_ARGUMENT  1
#define TUI_ERR_BAD_TERMINAL      2
#define TUI_ERR_OUT_OF_BOUNDS	  3
#define TUI_ERR_CANT_ALLOCATE	  4

//list node struct
struct listNode{
	void *data;
	int dataType;
	struct listNode *next;
};

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
	char extendedSet;//for later
	//ncurses window pointer
	WINDOW *wndPtr;
	//widget linked list head
	struct listNode *wdgHead;
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
	int selection;
	int scroll;
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
	int selection;
	char *(*values)[];//'values' won't store the list of values, only pointer
	char highlighted;
	char dropped;
	int maxLength;
	//pointer to parent window
	tuiWindow *tWndPtr;
}tuiWidgetDropdown;

void tuiInitLabel(tuiWindow *tWnd, tuiWidgetLabel *tLabl, unsigned int posX, unsigned int posY, char *label);
void tuiInitButton(tuiWindow *tWnd, tuiWidgetButton *tButton, unsigned int posX, unsigned int posY, char *label);
void tuiInitCheckbox(tuiWindow *tWnd, tuiWidgetCheckbox *tButton, unsigned int posX, unsigned int posY, char *label);
void tuiInitList(tuiWindow *tWnd, tuiWidgetList *tList, unsigned int posX, unsigned int posY, char *(*values)[], unsigned int listLength, unsigned int width, unsigned int height);

//initiate window
void tuiInitWindow(tuiWindow *tWnd, unsigned int posX, unsigned int posY, unsigned int wdt, unsigned int hgt, char *title, char decor, char palt);
//draw window on screen
void tuiDrawWindow(tuiWindow *tWnd);
//end window
void tuiEndWindow(tuiWindow *tWnd);

/* screens */

//initiate screen
int tuiInitScreen();
//end screen
void tuiEndScreen();
#endif

