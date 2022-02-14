//		file:tuitoolkit.c
//#include <ncurses.h>
#include "tuitoolkit.h"
//#include "tuitoolkit_internal.h"

#define initWindow(wnd) tuiWindow wnd = {.posX = 0, .posY = 0, .width = 30, .height = 15, .title = "window", .decoration = 7, .palette = 0}
#define initScreen(scr, wdth, hght) tuiScreen scr = {.screenWidth = wdth, .screenHeight = hght, .focusedWindow = 0}

int _returnLength(char *string){
	unsigned int t = 0;
	while(string[t] != '\0')
		t++;
	return t;
}

char* _truncString(char *string, unsigned int len){
	char s[len+1];
	for(int i = 0; i < len; i++)
		s[i] = string[i];
	s[len] = '\0';
	return s;
}

//return palette number,; mode: 0 - fill, 1 - widget
int _returnPalette(tuiWindow *tWnd, char mode){	
	switch(mode){
		//fill
		case 0:
			switch(tWnd -> palette){
				//light
				case 0: case 1: case 2:
					return  1;
					break;
				//dark
				case 3: case 4: case 5:
					return 2;
					break;
			}
			break;
		//widget
		case 1:
			switch(tWnd -> palette){
				//blue
				case 0: case 3:
					return 3;
					break;
				//green
				case 1: case 4:
					return 4;
					break;
				//red
				case 2: case 5:
					return 5;
					break;
			}
			break;
		//invalid - fallback to default colors
		default:
			return 0;
			break;
	}
}

void tuiInitLabel(tuiWindow *tWnd, tuiWidgetLabel *tLabel, unsigned int posX, unsigned int posY, char *label){
	*tLabel = (tuiWidgetLabel){.posX = posX, .posY = posY, .text = label, .tWndPtr = tWnd};
}
void tuiInitButton(tuiWindow *tWnd, tuiWidgetButton *tButton, unsigned int posX, unsigned int posY, char *label){
	*tButton = (tuiWidgetButton){.posX = posX, .posY = posY, .text = label, .tWndPtr = tWnd, .highlighted = 0};
}
void tuiInitCheckbox(tuiWindow *tWnd, tuiWidgetCheckbox *tCheckbox, unsigned int posX, unsigned int posY, char *label){
	*tCheckbox = (tuiWidgetCheckbox){.posX = posX, .posY = posY, .text = label, .tWndPtr = tWnd, .highlighted = 0, .ticked = 0};
}
void tuiInitDropdown(tuiWindow *tWnd, tuiWidgetDropdown *tDropdown, unsigned int posX, unsigned int posY, char *(*values)[]){	
	*tDropdown = (tuiWidgetDropdown){.posX = posX, .posY = posY, .values = values, .tWndPtr = tWnd, .highlighted = 0, .dropped = 0};
}

void _drawLabel(tuiWidgetLabel *tWdg){		
	wattron((tWdg -> tWndPtr) -> wndPtr, COLOR_PAIR(_returnPalette(tWdg -> tWndPtr, 0)));
	mvwprintw((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX, tWdg -> text);
	wrefresh((tWdg -> tWndPtr) -> wndPtr);
}
void _drawButton(tuiWidgetButton *tWdg){	
	switch(tWdg -> highlighted){
		case 0:
			wattron((tWdg -> tWndPtr) -> wndPtr, COLOR_PAIR(_returnPalette(tWdg -> tWndPtr, 0)));
			break;
		case 1:
			wattron((tWdg -> tWndPtr) -> wndPtr, COLOR_PAIR(_returnPalette(tWdg -> tWndPtr, 1)));
			break;
	}
	char *label = tWdg -> text;
	mvwaddch((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX, '<');
	mvwprintw((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX + 1, label);
	mvwaddch((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX + _returnLength(label) + 1, '>');
	wrefresh((tWdg -> tWndPtr) -> wndPtr);
}
void _drawCheckbox(tuiWidgetCheckbox *tWdg){	
	switch(tWdg -> highlighted){
		case 0:
			wattron((tWdg -> tWndPtr) -> wndPtr, COLOR_PAIR(_returnPalette(tWdg -> tWndPtr, 0)));
			break;
		case 1:
			wattron((tWdg -> tWndPtr) -> wndPtr, COLOR_PAIR(_returnPalette(tWdg -> tWndPtr, 1)));
			break;
	}
	char *label = tWdg -> text;
	//box
	switch(tWdg -> ticked){
		case 0:	
			mvwprintw((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX, "[ ]");
			break;
		case 1:
			mvwprintw((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX, "[#]");
			break;
	}
	//label
	wattron((tWdg -> tWndPtr) -> wndPtr, COLOR_PAIR(_returnPalette(tWdg -> tWndPtr, 0)));
	mvwprintw((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX + 4, label);
	wrefresh((tWdg -> tWndPtr) -> wndPtr);
}
/* window */

void tuiInitWindow(tuiWindow *tWnd, unsigned int posX, unsigned int posY, unsigned int wdt, unsigned int hgt, char *title, char decor, char palt, char set){
	//create ncurses window
	WINDOW *ncrsWnd = newwin(hgt, wdt, posY, posX);
	noecho();
	wclear(ncrsWnd);
	//set keypad support
	keypad(stdscr, TRUE);
	
	*tWnd = (tuiWindow){.posX = posX, .posY = posY, .width = wdt, .height = hgt, .title = title, .decoration = decor, .palette = palt, .wndPtr = ncrsWnd, .extendedSet = set};
}

void tuiDrawWindow(tuiWindow *tWnd){
	/* draw window body */
	//set color palette
	char fillpalette = _returnPalette(tWnd, 0);
	char wdgtpalette = _returnPalette(tWnd, 1);
	//fill background
	wbkgd((*tWnd).wndPtr, COLOR_PAIR(fillpalette));
	//draw frame
	switch(tWnd -> decoration){
		case 1: case 4: case 5: case 7://decoration modes
			box((*tWnd).wndPtr, 0, 0);
			break;
	}
	wattron((*tWnd).wndPtr, COLOR_PAIR(wdgtpalette));
	//draw title
	switch(tWnd -> decoration){
		case 2: case 4: case 6: case 7://decoration modes
			if(_returnLength(tWnd ->title) < tWnd -> width - 4){//short enough
				mvwprintw((*tWnd).wndPtr, 0, 2, tWnd-> title);
			} else{//longer than window width
				unsigned int len = tWnd -> width - 7;
				char trunc[len+1];
				for(int i = 0; i < len; i++)
					trunc[i] = tWnd -> title[i];
				trunc[len] = '\0';
				mvwprintw((*tWnd).wndPtr, 0, 2, trunc);
				mvwprintw((*tWnd).wndPtr, 0, tWnd -> width - 5, "...");
			}
			break;
	}
	wattroff((*tWnd).wndPtr, COLOR_PAIR(wdgtpalette));
	/* draw widgets */
	/*
	############################
	*/

	wrefresh((*tWnd).wndPtr);
}
/*
void tuiMoveWindow(tuiWindow *tWnd, unsigned int posX, unsigned int posY){
	//wclear((*tWnd).wndPtr);
	mvwin((*tWnd).wndPtr, posY, posX);
	wrefresh((*tWnd).wndPtr);
	wclear((*tWnd).wndPtr);
}
*/
void tuiEndWindow(tuiWindow *tWnd){
	endwin();
}

/* screen */

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
