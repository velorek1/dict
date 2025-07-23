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

//PROTOTYPES

void initStringArray(stringArray *arr);
void updateString(stringArray *arr, int index, const char *new_str);
void addString(stringArray *arr, const char *str);
void freeStringArray(stringArray *arr);
void printStringArray(stringArray *arr);
int parse_csv_line(FILE *fp, char fields[MAX_FIELDS][MAX_FIELD_LEN]);
int load_history();
void addEntry(char *word, char *translation, char *comment);
int write_data();
void input(char csearch[MAX_TEXT]);
int options(int total);
int showWord(int index);
void mainwindow();
/* DYNAMIC ARRAY ROUTINES */
/**************************/

void initStringArray(stringArray *arr) {
    arr->data = NULL;
    arr->capacity = 0;
    arr->size = 0;
}

void updateString(stringArray *arr, int index, const char *new_str) {
   /* if (index < 0 || index >= arr->size) {
        fprintf(stderr, "updateString: index %d out of bounds\n", index);
        return;
    }*/
    // Safely copy into fixed-size buffer
    strncpy(arr->data[index], new_str, MAX_STRING_LENGTH);
    arr->data[index][MAX_STRING_LENGTH] = '\0';  // Ensure null-termination
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
int load_history() {
   int i=0;
   int linecount=0;
   FILE *fp = fopen(DICTIONARY, "r");
     if  (miArray1.size>0) freeStringArray(&miArray1);
     if (miArray2.size>0) freeStringArray(&miArray2);
     if (miArray3.size>0) freeStringArray(&miArray3);
     if (miArray4.size>0) freeStringArray(&miArray4);
    initStringArray(&miArray1);
    initStringArray(&miArray2);
    initStringArray(&miArray3);
    initStringArray(&miArray4);
  
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

void addEntry(char *word, char *translation, char *comment){
  FILE *fp = fopen(DICTIONARY, "a");
  if (fp == NULL) {
        perror("Failed to open file");
        return;
    }
   fprintf(fp, "%d,%s,%s,%s\n", entries, word,translation,comment);
   fclose(fp);
}


int write_data(){
   int i=0;
   FILE *fp = fopen(DICTIONARY, "w");
   rewind(fp);
  if (fp == NULL) {
        perror("Failed to open file");
        return 1;
    }
   fprintf(fp, "ID,WORD,TRANSLITION,COMMENT\n");
   for (i=1; i<entries; i++){
      fprintf(fp, "%d,%s,%s,%s\n", i, miArray2.data[i],miArray3.data[i],miArray4.data[i]);
   }
   fclose(fp);
   return 0;
}



void input(char csearch[MAX_TEXT]){
       textbox(7, (termR/2)+7-1, 20,"Edit:", csearch, B_MAGENTA, F_WHITE, F_WHITE, 1);
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
  gotoxy(8,(termR/2)-5);
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
	resetAnsi(0);
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
  char ch=0;
  char iWord[MAX_TEXT];
  char iTran[MAX_TEXT];
  char iCom[MAX_TEXT];
  if (entries==1) return -1;
  resetAnsi(0);
  draw_window(5, (termR/2) - 6, termC-5, (termR/2) +6, B_WHITE,F_BLACK, B_WHITE,1,0,0);
  gotoxy(8,(termR/2)-5);
  outputcolor(FH_WHITE,B_BLACK);
  printf("Word Info\n");
  gotoxy(7,(termR/2)-4);
  resetAnsi(0);
 outputcolor(F_BLACK,B_WHITE);
  gotoxy(8,(termR/2)+7);
  printf("ESC: RETURN | SPACE: EDIT\n"); 
  do{ 
    if (listBox1!= NULL) removeList(&listBox1);  
  listBox1 = addatend(listBox1, newitem(miArray2.data[index+1], -1, -1, -1, -1));
  listBox1 = addatend(listBox1, newitem(miArray3.data[index+1], -1, -1, -1, -1));
  listBox1 = addatend(listBox1, newitem(miArray4.data[index+1], -1, -1, -1, -1));
 
        get_terminal_dimensions (&termR,&termC);
        if ((termR != ntermR) || (termC != ntermC))
	{
		//screen was resized
		return -1;

	}
 
    ch = listBox(listBox1, 7, (termR/2)-4, &scrollData, B_WHITE, F_BLACK, B_MAGENTA,
	       F_WHITE, 11, VERTICAL, 1,1);
   if (ch == K_ENTER) break;
   if (ch == K_SPACE) {
	switch (scrollData.itemIndex) {
	     case 0:              input(iWord); if (strlen(iWord)>1) {updateString(&miArray2, index+1, iWord); write_data();} break;
	     case 1:              input(iTran); if (strlen(iTran)>1) {updateString(&miArray3, index+1, iTran); write_data();} break;
	     case 2:              input(iCom); if (strlen(iCom)>1) {updateString(&miArray4, index+1, iCom); write_data();} break;
	}
	scrollData.lastch = 0;
	ch = 0;
	resetAnsi(0);
        draw_window(5, (termR/2) - 6, termC-5, (termR/2) +6, B_WHITE,F_BLACK, B_WHITE,1,0,0);
        gotoxy(8,(termR/2)-5);
        outputcolor(FH_WHITE,B_BLACK);
        printf("Word Info\n");
        resetAnsi(0);
        outputcolor(F_BLACK,B_WHITE);
	gotoxy(8,(termR/2)+7);
        printf("ESC: RETURN | SPACE: EDIT\n"); 
   }
   if (ch == K_TAB) break;
   if (ch == 'r') {write_data();}
   if (ch == 'x') {addEntry("test1","test2","test3");}
 //  if (ch == 'x') {if (invert == 1) invert = 0; else invert = 1;}
 //  if (ch == 'f') {search(csearch); if (strlen(csearch)==0 || filterCount==0) csearch[0]='\0';}
 //  if (ch == 'r') {csearch[0]='\0';}
   if (listBox1!= NULL) removeList(&listBox1);  
} while (scrollData.lastch != K_ESCAPE);
   if (listBox1!= NULL) removeList(&listBox1);  

  scrollData.lastch = 0;
  return 0;
}


void mainwindow(){

  draw_window(5, (termR/2) - 6, termC-5, (termR/2) +6, B_MAGENTA,F_WHITE, B_WHITE,1,0,0);
 // gotoxy(8,(termR/2)-5);
 // outputcolor(F_BLACK,B_WHITE);
 // printf("Dictionary v0.1");
  gotoxy(8,(termR/2)+7);
  outputcolor(F_BLACK,B_MAGENTA);
  printf("^v: SCROLL | SPACE : INFO");

}

int main() {
    char ch=0;
//    int invert = 0;
    //char csearch[MAX_TEXT];
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
 do{ 
    entries=load_history();
    mainwindow();
    gotoxy(8,(termR/2)-5);
    outputcolor(FH_WHITE,B_BLACK);
    printf("Dictionary v0.1 [%d]", entries-1);
 
 
    ch = listBox(listBox1, 7, (termR/2)-4, &scrollData, B_MAGENTA, F_WHITE, B_WHITE,
	       F_BLACK, 11, VERTICAL, 1,1);
   if (ch == K_ENTER) {showWord(scrollData.itemIndex); ch=0;}
   if (ch == K_TAB) break;
   if (ch == 'r') {write_data();}
   if (ch == 'x') {addEntry("test1","test2","test3");}
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
