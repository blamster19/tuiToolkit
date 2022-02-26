//		file:tuitoolkit.c
#include "tuitoolkit.h"

#define TYPE_HEAD		0
#define TYPE_LABEL		1
#define TYPE_BUTTON 	2
#define TYPE_CHECKBOX   3
#define TYPE_LIST		4

int _returnLength(char *string){
	unsigned int t = 0;
	while(string[t] != '\0')
		t++;
	return t;
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
//debug function
void _drawERROR(tuiWindow *wnd, char *name, int errcode, unsigned int posX, unsigned int posY){		
	switch(errcode){
		case 1:
			mvwprintw(wnd -> wndPtr, posY, posX, "[X]ERROR in %s; INVALID ARGUMENT", name);
			break;
		case 3:
			mvwprintw(wnd -> wndPtr, posY, posX, "[X]ERROR in %s; OUT OF BOUNDS", name);
			break;
		case 4:
			mvwprintw(wnd -> wndPtr, posY, posX, "[X]ERROR in %s; CANNOT ALLOCATE MEMORY", name);
			break;
		default:
			mvwprintw(wnd -> wndPtr, posY, posX, "[?]META ERROR in %s; WRONG ERROR CODE", name);
			break;
	}
	wrefresh(wnd -> wndPtr);
}
//create new node of linked list
void _appendToEnd(struct listNode** headerPtr, void *newData, int newDataType){
	//allocate new node
	struct listNode* newNode = malloc(sizeof(struct listNode));
	newNode -> data = newData;
	newNode -> dataType = newDataType;
	newNode -> next = NULL;
	//pointer to current last element
	struct listNode *lastPtr = *headerPtr;
	//if header is NULL, set it to point to  new node
	if(*headerPtr == NULL){
		*headerPtr = newNode;
		return;
	}
	//else find the last node that points to NULL and set it to point to new node instead
	while(lastPtr -> next != NULL)
		lastPtr = lastPtr -> next;
	lastPtr -> next = newNode;
}

/*    widgets    */

void tuiInitLabel(tuiWindow *tWnd, tuiWidgetLabel *tLabel, unsigned int posX, unsigned int posY, char *label){
	*tLabel = (tuiWidgetLabel){.posX = posX, .posY = posY, .text = label, .tWndPtr = tWnd};
	_appendToEnd(&tWnd -> wdgHead, tLabel, TYPE_LABEL);
}
void tuiInitButton(tuiWindow *tWnd, tuiWidgetButton *tButton, unsigned int posX, unsigned int posY, char *label){
	*tButton = (tuiWidgetButton){.posX = posX, .posY = posY, .text = label, .tWndPtr = tWnd, .highlighted = 0};
	_appendToEnd(&tWnd -> wdgHead, tButton, TYPE_BUTTON);
}
void tuiInitCheckbox(tuiWindow *tWnd, tuiWidgetCheckbox *tCheckbox, unsigned int posX, unsigned int posY, char *label){
	*tCheckbox = (tuiWidgetCheckbox){.posX = posX, .posY = posY, .text = label, .tWndPtr = tWnd, .highlighted = 0, .ticked = 0};
	_appendToEnd(&tWnd -> wdgHead, tCheckbox, TYPE_CHECKBOX);
}
void tuiInitList(tuiWindow *tWnd, tuiWidgetList *tList,  unsigned int posX, unsigned int posY, char *(*values)[], unsigned int listLength, unsigned int width, unsigned int height){	
	*tList = (tuiWidgetList){.posX = posX, .posY = posY, .values = values,.listLength = listLength, .tWndPtr = tWnd, .selection = 0, .scroll = 0, .highlighted = 0, .width = width, .height = height, .scroll = 0};
	_appendToEnd(&tWnd -> wdgHead, tList, TYPE_LIST);
}

int _drawLabel(tuiWidgetLabel *tWdg){		
	//check bounds
	if(( (tWdg -> posX) < 0 ) || ( (tWdg -> posY) < 0)){
		_drawERROR((tWdg -> tWndPtr), "label", TUI_ERR_OUT_OF_BOUNDS, tWdg -> posX, tWdg -> posY);
		return 3;
	}
	//print
	wattron((tWdg -> tWndPtr) -> wndPtr, COLOR_PAIR(_returnPalette(tWdg -> tWndPtr, 0)));
	mvwprintw((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX, tWdg -> text);
	return 0;
}
int _drawButton(tuiWidgetButton *tWdg){	
	//check bounds
	if(( (tWdg -> posX) < 0 ) || ( (tWdg -> posY) < 0)){
		_drawERROR((tWdg -> tWndPtr), "button", TUI_ERR_OUT_OF_BOUNDS, tWdg -> posX, tWdg -> posY);
		return 3;
	}
	if( !( (tWdg -> highlighted == 0) || (tWdg -> highlighted == 1) ) ){
		_drawERROR((tWdg -> tWndPtr), "button", TUI_ERR_OUT_OF_BOUNDS, tWdg -> posX, tWdg -> posY);
		return 3;
	}
	//set highlight
	switch(tWdg -> highlighted){
		case 0:
			wattron((tWdg -> tWndPtr) -> wndPtr, COLOR_PAIR(_returnPalette(tWdg -> tWndPtr, 0)));
			break;
		case 1:
			wattron((tWdg -> tWndPtr) -> wndPtr, COLOR_PAIR(_returnPalette(tWdg -> tWndPtr, 1)));
			break;
	}
	//print
	char *label = tWdg -> text;
	mvwaddch((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX, '<');
	mvwprintw((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX + 1, label);
	mvwaddch((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX + _returnLength(label) + 1, '>');
	return 0;
}
int _drawCheckbox(tuiWidgetCheckbox *tWdg){	
	//check bounds
	if(( (tWdg -> posX) < 0 ) || ( (tWdg -> posY) < 0)){
		_drawERROR((tWdg -> tWndPtr), "checkbox", TUI_ERR_OUT_OF_BOUNDS, tWdg -> posX, tWdg -> posY);
		return 3;
	}
	//set highlight
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
	return 0;
}
int _drawList(tuiWidgetList *tWdg){	
	//check bounds
	if(( (tWdg -> posX) < 0 ) || ( (tWdg -> posY) < 0)){
		_drawERROR((tWdg -> tWndPtr), "list", TUI_ERR_OUT_OF_BOUNDS, tWdg -> posX, tWdg -> posY);
		return 3;
	}	
	if(tWdg -> selection <= 0){
		_drawERROR((tWdg -> tWndPtr), "list", TUI_ERR_OUT_OF_BOUNDS, tWdg -> posX, tWdg -> posY);
		return 3;
	}
	if(tWdg -> selection > tWdg -> listLength){
		_drawERROR((tWdg -> tWndPtr), "list", TUI_ERR_OUT_OF_BOUNDS, tWdg -> posX, tWdg -> posY);
		return 3;
	}
	//upper line
	mvwaddch((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX, ACS_ULCORNER);
	mvwhline((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX + 1, ACS_HLINE, tWdg -> width - 2);
	mvwaddch((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY, tWdg -> posX + tWdg -> width - 1, ACS_URCORNER);
	//entries
	char text[tWdg -> width];
	int offset = (tWdg -> selection) - ((tWdg -> height)/2);
	//draw values
	for(int i = 0; i < (tWdg -> height - 2); i++){
		//frame
		mvwaddch((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY + i + 1, tWdg -> posX, ACS_VLINE);
		//clear field
		mvwhline((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY + i + 1, tWdg -> posX + 1, ' ', tWdg -> width - 2);
		//options / empty field
		if( !( ((offset + i) < 0) || ( (offset + i) > (tWdg -> listLength - 1 ) )) ){//empty field	
			//set color if index in the middle
			if(i == ((tWdg -> height)/2 - 1))
				wattron( (tWdg -> tWndPtr) -> wndPtr, COLOR_PAIR(_returnPalette( tWdg -> tWndPtr, 1 ) ) );		
			//set text and truncate if necessary
			if(_returnLength( (*tWdg -> values)[i + offset] ) < ( (tWdg -> width) - 2 ) ){//if the value is shorter than width
				int j = 0;
				while((*tWdg ->values)[i + offset][j] != '\0'){
					text[j] = (*tWdg -> values)[i + offset][j];
					j++;
				}
				text[j] = '\0';
			} else{//else truncate
				unsigned int ll = (tWdg -> width) - 2;
				char trunc[ll+1];
				int j = 0;
				for(; j < ll-3; j++)
					text[j] = (*tWdg -> values)[i + offset][j];
				for(; j<ll+3; j++)
					text[j] = '.';
				text[ll] = '\0';
			}
		//print the value
		mvwprintw((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY + i + 1, tWdg -> posX + 1, text);
		}
		wattroff((tWdg -> tWndPtr) -> wndPtr, COLOR_PAIR(_returnPalette(tWdg -> tWndPtr, 1)));
		//frame
		mvwaddch((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY + i + 1, tWdg -> posX + tWdg -> width - 1, ACS_VLINE);
	}
	//lower line
	mvwaddch((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY + tWdg -> height - 1, tWdg -> posX, ACS_LLCORNER);
	mvwhline((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY + tWdg -> height - 1, tWdg -> posX + 1, ACS_HLINE, tWdg -> width - 2);
	mvwaddch((tWdg -> tWndPtr) -> wndPtr, tWdg -> posY + tWdg -> height - 1, tWdg -> posX + tWdg -> width - 1, ACS_LRCORNER);
	return 0;
}

/* window */

void tuiInitWindow(tuiWindow *tWnd, unsigned int posX, unsigned int posY, unsigned int wdt, unsigned int hgt, char *title, char decor, char palt){
	//create ncurses window
	WINDOW *ncrsWnd = newwin(hgt, wdt, posY, posX);
	noecho();
	wclear(ncrsWnd);
	//set keypad support
	keypad(stdscr, TRUE);
	*tWnd = (tuiWindow){.posX = posX, .posY = posY, .width = wdt, .height = hgt, .title = title, .decoration = decor, .palette = palt, .wndPtr = ncrsWnd};
	tWnd -> wdgHead = NULL;
}
#define tuiDrawWidget(wdgt) _Generic (wdgt, tuiWidgetLabel*: _drawLabel, tuiWidgetButton*: _drawButton,tuiWidgetList*: _drawList, tuiWidgetCheckbox*: _drawCheckbox, tuiWidgetList: _drawList)(wdgt)
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
				//truncate title
				unsigned int len = tWnd -> width - 7;
				char trunc[len+1];
				for(int i = 0; i < len; i++)
					trunc[i] = tWnd -> title[i];
				trunc[len] = '\0';
				mvwprintw((*tWnd).wndPtr, 0, 2, trunc);	
				//ellipsis
				mvwprintw((*tWnd).wndPtr, 0, tWnd -> width - 5, "...");
			}
			break;
	}
	wattroff((*tWnd).wndPtr, COLOR_PAIR(wdgtpalette));
	/* draw widgets */
	struct listNode* wdgPtr;
	wdgPtr = tWnd -> wdgHead;
	while(wdgPtr != NULL){
		switch(wdgPtr -> dataType){
			case TYPE_LABEL:
				tuiDrawWidget( (tuiWidgetLabel*)wdgPtr -> data );
				break;
			case TYPE_BUTTON:	
				tuiDrawWidget( (tuiWidgetButton*)wdgPtr -> data );
				break;
			case TYPE_CHECKBOX:
				tuiDrawWidget( (tuiWidgetCheckbox*)wdgPtr -> data );
				break;
			case TYPE_LIST:
				tuiDrawWidget( (tuiWidgetList*)wdgPtr -> data );
				break;
			default:
				break;
		}
		wdgPtr = wdgPtr -> next;
	}
	wrefresh((*tWnd).wndPtr);
}
void tuiEndWindow(tuiWindow *tWnd){	
	if(tWnd -> wdgHead != NULL){
		struct listNode* Ptr1;
		struct listNode* Ptr2;
		struct listNode* Ptr3;
		Ptr1 = tWnd -> wdgHead;
		Ptr2 = Ptr1 -> next;
		while(Ptr2 != NULL){
			Ptr3 = Ptr2;
			//deallocate node
			free(Ptr1);
			Ptr1 = Ptr2;
			Ptr2 = Ptr3 -> next;
		}
	}
	delwin(tWnd -> wndPtr);
	refresh();
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
