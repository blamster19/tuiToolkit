//		file:tuitoolkit.c
//#include <ncurses.h>
#include "tuitoolkit.h"

#define initWindow(wnd) tuiWindow wnd = {.posX = 0, .posY = 0, .width = 30, .height = 15, .title = "window", .decoration = 7, .palette = 0}
#define initScreen(scr, wdth, hght) tuiScreen scr = {.screenWidth = wdth, .screenHeight = hght, .focusedWindow = 0}

int tuiInitWindow(tuiWindow *tWnd, unsigned int posX, unsigned int posY, unsigned int wdt, unsigned int hgt, char *title, char decor, char palt){
	//create ncurses window
	WINDOW *ncrsWnd = newwin(hgt, wdt, posY, posX);
	noecho();
	wclear(ncrsWnd);
	//set keypad support
	keypad(stdscr, TRUE);
	
	*tWnd = (tuiWindow){.posX = posX, .posY = posY, .width = wdt, .height = hgt, .title = title, .decoration = decor, .palette = palt, .wndPtr = ncrsWnd};
	return 0;	
}

void tuiDrawWindow(tuiWindow *tWnd){
	char fillpalette;
	char elempalette;
	switch(tWnd -> palette){
		case 0: case 1: case 2:
			fillpalette = 1;
			break;
		case 3: case 4: case 5:
			fillpalette = 2;
			break;
	}
	switch(tWnd -> palette){
		case 0: case 3:
			elempalette = 3;
			break;
		case 1: case 4:
			elempalette = 4;
			break;
		case 2: case 5:
			elempalette = 5;
			break;
	}
	//wattron((*tWnd).wndPtr, COLOR_PAIR(fillpalette));
	/*
	//draw background
	switch(tWnd -> decoration){
		case 3: case 5: case 6: case 7:
			for(int i = 0; i < tWnd -> height; i++)
				mvwhline((*tWnd).wndPtr, i, 0, ' ', tWnd -> width);
	}*/

	wbkgd((*tWnd).wndPtr, COLOR_PAIR(fillpalette));

	//draw frame
	switch(tWnd -> decoration){
		case 1: case 4: case 5: case 7:
			box((*tWnd).wndPtr, 0, 0);
			break;
	}
	wattron((*tWnd).wndPtr, COLOR_PAIR(elempalette));
	//draw title
	switch(tWnd -> decoration){
		case 2: case 4: case 6: case 7:
				mvwprintw((*tWnd).wndPtr, 0, 2, tWnd-> title);
				break;
	}
	//wattroff((*tWnd).wndPtr, COLOR_PAIR(fillpalette));
	wattroff((*tWnd).wndPtr, COLOR_PAIR(elempalette));
	wrefresh((*tWnd).wndPtr);
}

void tuiEndWindow(tuiWindow *tWnd){
	endwin();
}

int tuiInitScreen(){
	initscr();
	//probe for terminal color support
	if(has_colors() == 0){
	    endwin();
		return 1;//no color support
	}
	start_color();
	//set color palette
	init_pair(1, COLOR_BLACK, COLOR_WHITE);//light variant
	init_pair(2, COLOR_WHITE, COLOR_BLACK);//dark variant
	init_pair(3, COLOR_BLACK, COLOR_CYAN);//elements blue
	init_pair(4, COLOR_BLACK, COLOR_GREEN);//elements green
	init_pair(5, COLOR_BLACK, COLOR_RED);//elements red
	refresh();
	return 0;
}

void tuiEndScreen(){
	endwin();
}
