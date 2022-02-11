//		file:tuitoolkit.c
#include "tuitoolkit.h"
#include <ncurses.h>

#define BCKGLIGHT   47
#define BCKGDARK    100
#define ELEMBLUE    46
#define ELEMGREEN   42
#define ELEMRED     41

#define initWindow(wnd) tuiWindow wnd = {.posX = 0, .posY = 0, .width = 30, .height = 15, .title = "window", .decoration = 7, .palette = 0}
#define initScreen(scr, wdth, hght) tuiScreen scr = {.screenWidth = wdth, .screenHeight = hght, .focusedWindow = 0}

int tuiInitWindow(struct tuiWindow *window){
	noecho();
	wclear();
	//keypad support
	keypad(stdscr, TRUE);
	start_color();
	//set color palette
	switch(window.palette){
		case 0 :
			init_pair(0, COLOR_BLACK, COLOR_WHITE);//background
			init_pair(1, COLOR_BLACK, COLOR_CYAN);//foreground
			break;
		case 1 :
			init_pair(0, COLOR_BLACK, COLOR_WHITE);
			init_pair(1, COLOR_BLACK, COLOR_GREEN);
			break;
		case 2 :
			init_pair(0, COLOR_BLACK, COLOR_WHITE);
			init_pair(1, COLOR_BLACK, COLOR_RED);
			break;
		default :
			endwin();
			return 2;//invalid palette
	}
}

void tuiDrawWindow(struct tuiWindow *window){
	refresh();
}

void tuiEndWindow(struct tuiWindow *window){
	endwin();
}

int tuiInitScreen(){
	initscr();
	//probe for terminal color support
	if(has_colors() == 0){
	    endwin();
		    return 1;//no color support
			}

}
