//TUI Dictionary [Dict - CSV]
//Coded by v3l0r3k
//Date: July 2025

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rterm.h"
#include "listc.h"
#include "keyb.h"

#define MAX_FIELDS 4
#define MAX_FIELD_LEN 255
#define MAX_LINE_LEN 1024
#define DICTIONARY "data.csv"
#define MAX_STRING_LENGTH 255

typedef struct stringArray {
    char **data;
    size_t capacity;
    size_t size;
} stringArray;


//GLOBALS
int globalCursorX=0, globalCursorY=0;
int termR=0, termC=0;
int ntermR=0, ntermC=0;
int count=0;
int filterCount=0;
int entries = 0;
SCROLLDATA scrollData;
char fields[4][255];
stringArray miArray1;
stringArray miArray2;
stringArray miArray3;
stringArray miArray4;
 
/* DYNAMIC ARRAY ROUTINES */
/**************************/

void initStringArray(stringArray *arr) {
    arr->data = NULL;
    arr->capacity = 0;
    arr->size = 0;
}

void addString(stringArray *arr, const char *str) {
    size_t new_capacity = 0;
    if (arr->capacity == arr->size) {
        new_capacity = (arr->capacity == 0) ? 4 : arr->capacity * 2;
        arr->data = realloc(arr->data, new_capacity * sizeof(char *));
        if (!arr->data) {
            perror("Failed to allocate array of chars");
            exit(EXIT_FAILURE);
        }
        arr->capacity = new_capacity; // Update capacity
    }
    arr->data[arr->size] = malloc(MAX_STRING_LENGTH + 1);
    if (!arr->data[arr->size]) {
        perror("Failed to allocate memory for the string");
        exit(EXIT_FAILURE);
    }
    strncpy(arr->data[arr->size], str, MAX_STRING_LENGTH);
    arr->data[arr->size][MAX_STRING_LENGTH] = '\0';
    arr->size++;
}

void freeStringArray(stringArray *arr) {
    size_t i = 0;
    for (i = 0; i < arr->size; i++) {
        free(arr->data[i]);
    }
    free(arr->data);
    arr->data = NULL;
    arr->capacity = 0;
    arr->size = 0;
}

void printStringArray(stringArray *arr) {
    size_t i = 0;
    for (i = 0; i < arr->size; i++) {
        printf("%s\n", arr->data[i]);
    }
}

/**************************/

int parse_csv_line(FILE *fp, char fields[MAX_FIELDS][MAX_FIELD_LEN]) {
    char line[MAX_LINE_LEN];

    // Read one line from the file
    if (!fgets(line, sizeof(line), fp)) {
        return 0;  // EOF or error
    }

    // Remove newline character at the end (if any)
    line[strcspn(line, "\r\n")] = '\0';

    // Tokenize line with comma as delimiter
    int fieldIndex = 0;
    char *token = strtok(line, ",");

    while (token && fieldIndex < MAX_FIELDS) {
        strncpy(fields[fieldIndex], token, MAX_FIELD_LEN - 1);
        fields[fieldIndex][MAX_FIELD_LEN - 1] = '\0';  // Null-terminate safely
        token = strtok(NULL, ",");
        fieldIndex++;
    }

    // Fill remaining fields with empty strings (if fewer than 4 columns)
    while (fieldIndex < MAX_FIELDS) {
        fields[fieldIndex][0] = '\0';
        fieldIndex++;
    }

    return 1;  // Successfully read one line
}

int options(int total){
  char och=0;
  int keypressed = 0;
  int i = 0;
  int whereX = 0, whereY = 0;
  int xDIR = 0, yDIR = 0;
  int nrandom = (rand() % (total-1)+1); // Generates 0 to upper_bound - 1

  if (entries==1) return -1;
  fprintf(stderr,"%d",entries);
  resetAnsi(0);
  draw_window(5, (termR/2) - 6, termC-5, (termR/2) +6, B_BLACK,F_WHITE, B_BLACK,1,0,0);
  gotoxy((termC/2)-10,(termR/2)-5);
  outputcolor(F_BLACK,B_WHITE);
  printf("Dictionary Use\n");
  gotoxy(7,(termR/2)-4);
  outputcolor(FH_GREEN,B_BLACK);
  printf("[+] KEYS:\n");
  gotoxy(7,(termR/2)-3);
  outputcolor(F_GREEN,B_BLACK);
  printf("- [ENTER]: Run command.\n");
  gotoxy(7,(termR/2)-2);
  outputcolor(F_GREEN,B_BLACK);
  printf("- [TAB]: Type command.\n");
  gotoxy(7,(termR/2)-1);
  outputcolor(F_GREEN,B_BLACK);
  printf("- [ESC]: Exit.\n");
  gotoxy(7,(termR/2));
  outputcolor(F_GREEN,B_BLACK);
  printf("- [x]: Invert list.\n");
  gotoxy(7,(termR/2)+1);
  outputcolor(F_GREEN,B_BLACK);
  printf("- [f/r]: Add/remove filter.\n");
   gotoxy(7,(termR/2)+2);
  outputcolor(FH_BLACK,B_BLACK);
  printf(":: dict v0.1 - 2025 ::\n");
  for(i=7; i<=termC-5;i++){
    gotoxy(i,(termR/2)+3);
    outputcolor(F_WHITE,B_BLACK);
    printf("%lc", HOR_LINE);
  }
  gotoxy(8,(termR/2)+7);
  outputcolor(F_WHITE,B_BLACK);
  printf("PRESS ANY KEY...\n");
  i=1;
     whereX = 7;
     xDIR = 1;
     yDIR = 1;
     whereY = (termR/2)+4;
  do{
	 keypressed= kbhit(200);
	 //wait for keypress
	 if (keypressed)
		 och = readch();
	//Animation
        gotoxy(whereX,whereY);
        outputcolor(F_BLACK,B_BLACK);
	printf("%s:%s\n",miArray2.data[nrandom],miArray3.data[nrandom]);
 	if (whereX == termC-7-16) xDIR = -1;
        if (whereX == 7) xDIR = 1;
        if (whereY == (termR/2)+4)  {yDIR = 1;}
        if (whereY == (termR/2)+6) {yDIR = -1;}
        whereX = whereX + xDIR;
        whereY = whereY + yDIR;

        get_terminal_dimensions (&termR,&termC);
        if ((termR != ntermR) || (termC != ntermC))
	{
		//screen was resized
		return -1;

	}
        gotoxy(whereX,whereY);
        outputcolor(FH_CYAN,B_BLACK);
        
	printf("%s:%s\n",miArray2.data[nrandom],miArray3.data[nrandom]);
  } while(keypressed != 1);
  och++;
  return 0;
}

int showWord(int index){
  char och=0;
  int keypressed = 0;
  if (entries==1) return -1;
  resetAnsi(0);
  draw_window(5, (termR/2) - 6, termC-5, (termR/2) +6, B_WHITE,F_BLACK, B_WHITE,1,0,0);
  gotoxy((termC/2)-10,(termR/2)-5);
  outputcolor(F_WHITE,B_BLACK);
  printf("Word Info\n");
  gotoxy(7,(termR/2)-4);
  outputcolor(FH_BLACK,B_YELLOW);
  printf("[+] %s:\n",miArray2.data[index+1]);
  gotoxy(7,(termR/2)-3);
  outputcolor(F_BLACK,B_WHITE);
  printf("- [%s]\n", miArray3.data[index+1]);
  gotoxy(7,(termR/2)-2);
  printf("- [%s]\n", miArray4.data[index+1]);
  outputcolor(F_BLACK,B_WHITE);
  gotoxy(8,(termR/2)+7);
  printf("PRESS ANY KEY...\n");
  do{
	 keypressed= kbhit(100);
	 //wait for keypress
	 if (keypressed)
		 och = readch();
        get_terminal_dimensions (&termR,&termC);
        if ((termR != ntermR) || (termC != ntermC))
	{
		//screen was resized
		return -1;

	}
        
  } while(keypressed != 1);
  och++;
  return 0;
}


void mainwindow(){

  draw_window(5, (termR/2) - 6, termC-5, (termR/2) +6, B_MAGENTA,F_WHITE, B_WHITE,1,0,0);
  gotoxy((termC/2)-10,(termR/2)-5);
  outputcolor(F_BLACK,B_WHITE);
  printf("Dictionary v0.1");
  gotoxy(8,(termR/2)+7);
  outputcolor(F_BLACK,B_MAGENTA);
  printf("^v: SCROLL | SPACE : INFO");

}

int load_history(int ascending, const char *instrstr) {
   int i=0;
   int linecount=0;
   FILE *fp = fopen(DICTIONARY, "r");
    if (!fp) {
        perror("Failed to open file");
        listBox1 = addatend(listBox1, newitem("No data found!", -1, -1, -1, -1));
        return 1;
    }

    char fields[4][255];

    while (parse_csv_line(fp, fields)) {
        addString(&miArray1, fields[0]);
        addString(&miArray2, fields[1]);
        addString(&miArray3, fields[2]);
        addString(&miArray4, fields[3]);
	linecount++;
    }
    if (linecount >1){
	    for (i=1;i<linecount;i++){
               listBox1 = addatend(listBox1, newitem(miArray2.data[i], -1, -1, -1, -1));
	    }

    } else
        listBox1 = addatend(listBox1, newitem("No data found!", -1, -1, -1, -1));
    fclose(fp);
    return linecount;
}

void search(char csearch[MAX_TEXT]){
       textbox(21, (termR/2)+7-1, 20,"Filter:", csearch, B_BLACK, F_WHITE, F_WHITE, 1);
}

int main() {
    size_t k = 0;
    char ch=0;
    char command[500];
    int invert = 0;
   char csearch[MAX_TEXT];
     initStringArray(&miArray1);
    initStringArray(&miArray2);
    initStringArray(&miArray3);
    initStringArray(&miArray4);
    srand(time(NULL));  // Seed the random number generator (only once)

  
   get_pos(&globalCursorY, &globalCursorX);
   get_terminal_dimensions (&termR,&termC);
   //check screen size
   if ((termR < 15) || (termC<30)){
         fprintf(stderr, "Screen is too small to display.\n");
        return EXIT_FAILURE;
   }
   ntermR = termR;
   ntermC = termC;
   init_term();
   hidecursor();   

    resetScrollData(&scrollData);
    setselectorLimit(termC-11);
   csearch[0] = '\0';
 do{ 
    entries=load_history(invert,csearch);
    mainwindow();
    gotoxy((termC/2)-10,(termR/2)-5);
    outputcolor(F_BLACK,B_WHITE);
    printf("Dictionary v0.1 [%d]", entries);
 
 
    ch = listBox(listBox1, 7, (termR/2)-4, &scrollData, B_MAGENTA, F_WHITE, B_WHITE,
	       F_BLACK, 11, VERTICAL, 1,1);
   if (ch == K_ENTER) showWord(scrollData.itemIndex);
   if (ch == K_TAB) break;
 //  if (ch == 'x') {if (invert == 1) invert = 0; else invert = 1;}
 //  if (ch == 'f') {search(csearch); if (strlen(csearch)==0 || filterCount==0) csearch[0]='\0';}
 //  if (ch == 'r') {csearch[0]='\0';}

   if (ch == K_SPACE) {
	   ch = 0; 
	  if (options(entries) == -1 ) {scrollData.screenChanged = 1; scrollData.itemIndex = -1; break;}
  
   }
   if (listBox1!= NULL) removeList(&listBox1);  
 
} while (scrollData.lastch != K_ESCAPE);   
   showcursor();   
   ch++;
   draw_transparent(5, (termR/2) - 6, termC-5, (termR/2) +6);
   gotoxy(globalCursorX, globalCursorY-2);
    close_term();
    
   if (scrollData.screenChanged == 1) system("clear");
    printf("\n");
    //printf("First run: %d:%d:%c\n",globalCursorX,globalCursorY,ch);
   //write(STDOUT_FILENO, scrollData.item, strlen(scrollData.item)); 
   if (scrollData.itemIndex != -1)  {
     strcpy(command, "\0"); 
     strcat(command, scrollData.item);
     if (scrollData.lastch!=K_TAB) strcat(command, "\n");
     //system(command);
//    for (k = 0; k < strlen(command); k++) {
//     ioctl(STDIN_FILENO, TIOCSTI, &command[k]);
     printf("\r"); //avoids echo
    //}
   }
    freeStringArray(&miArray1);
    freeStringArray(&miArray2);
    freeStringArray(&miArray3);
    freeStringArray(&miArray4);
    return EXIT_SUCCESS;
}
