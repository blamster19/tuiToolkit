//	file: tuitoolkit.h
#ifndef tuitoolkit_h
#define tuitoolkit_h

/*
tuiToolkit - small TUI widget library


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
								[background][foreground]
							0 - light		 blue
							1 - light		 green
							2 - light		 red

int tuiInitWindow:
	return values:
		0 - window generated successfully
		1 - terminal has no color support
		2 - invalid color palette
*/

/* window elements */
//label
typedef struct {
	int posX;
	int posY;
	char *text;
}tuiElementLabel;
//button
typedef struct {
	int posX;
	int posY;
	char *text;
	char highlighted;
}tuiElementButton;


/* windows */
//window struct
typedef struct {
	int posX;
	int posY;
	int width;
	int height;
	char *title;
	char decoration;
	char palette;

	char windowBuffer[width*height+1] = {0};
	char windowColorMode;//stores 1 if terminal is capable of printing colored output
}tuiWindow;

//window init macro
#define initWindow(wnd) tuiWindow wnd = {.posX = 0, .posY = 0, .width = 30, .height = 15, .title = "window", .decoration = 7, .palette = 0}

//initiate window
int tuiInitWindow(struct tuiWindow window);
//draw window on screen
void tuiDrawWindow(struct tuiWindow window);
//end window
void tuiEndWindow(struct tuiWindow window);

/* screens */
//screen struct
typedef struct {
	int screenWidth;
	int screenHeight;
	int focusedWindow;
}tuiScreen;

//init screen macro
#define initScreen(scr, wdth, hght) tuiScreen scr = {.screenWidth = wdth, .screenHeight = hght, .focusedWindow = 0}

//initiate screen
int tui
//end screen
void tuiScreenEnd();


#endif

