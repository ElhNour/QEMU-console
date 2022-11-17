#include "main.h"
/**
 * This is the C entry point, upcalled once the hardware has been setup properly
 * in assembly language, see the reset.s file.
 */
#define NUMBER_OF_STRING 20
#define MAX_STRING_SIZE 50

/* Global variables */
int global_1=1;
int global_2=0;

void _start() {// including both interrupts and traps.
  int i = 0;// for lines
  int j = 0;// for characters
  int pointer=0; // cursor's position
  int offset=0; // line length
  int k,cmd,idx; // k: index iterator, cmd: command that should be parsed, idx: index of the displayed line
  char cmd_1[6]={'r','e','s','e','t','\n'};// first cmd to parse
  char cmd_2[5]={'e','c','h','o',' '};// second cmd to parse
  char lines[NUMBER_OF_STRING][MAX_STRING_SIZE];//matrix of 20 lines containing entered characters

  uart_send_string(UART0,"\033[2J\033[0;0H");
  uart_send_string(UART0, "\nQuit with \"C-a c\" and then type in \"quit\".\n");
  uart_send_string(UART0, "\nHello world! This is an emulated ARM shell with QEMU...\n");

  while (1) {
    unsigned char c;
    while (0 == uart_receive(UART0, &c));

    //Perform a left shift if NUMBER_OF_STRING is reached and the user writes a new line 
    //whithout hitting return which could be an empty line that we don't save
    //or backspace that we don't save as well


    if (offset<MAX_STRING_SIZE){
      switch (c)
      {
      case '\r':// return is pressed
        uart_send(UART0, '\n');
        if (lines[i][offset-1]!='\n'){//If "return" isn't hit yet
        lines[i][offset]='\n';

        /*Parsing the entered line*/
        k=0;
        while (k<offset){
          if(cmd_1[k]!=lines[i][k] && cmd_2[k]!=lines[i][k]) break; //different cmds
          else if(cmd_1[k]==lines[i][k]) {
            cmd=1;//the entered line corresponds to cmd_1  so far
            k++;
            if (cmd_1[k]=='\n') break;// end of cmd, stop comparison
          }
          else {
            cmd=2; //the entered line corresponds to cmd_2  so far
            k++;
            if (cmd_2[k]==' ') break; // end of cmd, stop comparison
          }
        }
        //See if it's cmd_1 or cmd_2
        if (cmd==1){//reset
          uart_send_string(UART0,"\033[2J\033[0;0H");//clears the entire screen, leaving the cursor at the top-left corner
        }
        else if (cmd==2){//echo
          k++;
          while (lines[i][k]!='\n')
          {
           uart_send(UART0,lines[i][k]);//display the line
           k++;
          }
          uart_send(UART0,'\n');
          
        }
    
        /*Update the number of lines, index of line, pointer, and the offset*/
        i=(i+1)%20; //new line
        idx=i;
        offset=0;
        pointer=0;
        }
        break;

        case 0x7F://backspace is pressed
        if (pointer==offset)//delete fromt the end
        {
          uart_send_string(UART0,"\b\x20\b");//backspace simulation: cursor returns left, print escape to replace the current char, cursor returns left again
          lines[i][pointer-1]=' '; //remove the last char
          if (pointer>0) pointer--;
          if (offset>0) offset--;
        }
        else if (pointer<offset){//delete fromt the middle
          //Left shift 
          for (j=pointer-1;j<offset-1;j++){
            lines[i][j]=lines[i][j+1];
          }
           lines[i][j+1]=' ';//remove the last char
          uart_send_string(UART0,"\b\x20\b");
          if (pointer>0) pointer--;
          if (offset>0) offset--;
          
          for (j=pointer;j<offset;j++){
              uart_send(UART0,lines[i][j]);//display the rest of the line
            }
          //uart_send_string(UART0,"\x20");
          for (k=0;k<offset-pointer;k++) uart_send_string(UART0,"\033\[\D");//go back to the original position
        }
        
        break;

        case '\033'://Beginning of a sequence that might be an arrow
        while (0 == uart_receive(UART0, &c));//ignored char
        while (0 == uart_receive(UART0, &c));
        switch (c)
        {
        case 'D'://left arrow is pressed 
          if (pointer>0) pointer--; //position pointer to left
          uart_send_string(UART0,"\033\[\D");//move cursor to left
        break;

        case 'C'://right arrow is pressed
          if (pointer<offset) pointer++;  //position pointer to right
          uart_send_string(UART0,"\033\[\C");//move cursor to right
        break;

        case 'A'://up arrow is pressed
          //Display line at idx in the history
          if (idx>0) idx --;//old lines
          else idx=20;
          for (k=0;k<MAX_STRING_SIZE;k++) uart_send(UART0,lines[idx][k]);
        break;

        case 'B'://down arrow is pressed
          //Display line at idx in the history
          if (idx<NUMBER_OF_STRING-1) idx ++;//recent lines
          else idx=0;
          for (k=0;k<MAX_STRING_SIZE;k++) uart_send(UART0,lines[idx][k]);
        break;
        }

        break;

        default://a simple char is pressed
        if (pointer<offset)//in the middle of the line
        {     
          uart_send(UART0, c);
          //Right shift 
          for (j=pointer;j<offset;j++) {
            uart_send(UART0, lines[i][j]);//display the line simultanouesly
            lines[i][offset+pointer-j]=lines[i][offset+pointer-j-1];
            }
          uart_send(UART0, lines[i][offset]);//display the last char
          lines[i][pointer]=c;//store the entered char
          if (pointer<MAX_STRING_SIZE) pointer++;
          if (offset<MAX_STRING_SIZE) offset++;
          for (k=0;k<=offset-pointer;k++) uart_send_string(UART0,"\033\[\D");//go back to the original position
          }
        else{//insert from the end of the line
        uart_send(UART0, c);
        lines[i][offset]=c; 
        if (pointer<MAX_STRING_SIZE) pointer++;
        if (offset<MAX_STRING_SIZE) offset++;
        }
        break;
      }

    }
    else { 
      uart_send_string(UART0, "\nMax String Size is reached!");
      offset=0;
      pointer=0;
      i=(i+1)%20;//Here I chose to store the entered line although it may be incomplete, but the user should display it normally
      idx=i;
    }
  }
}
