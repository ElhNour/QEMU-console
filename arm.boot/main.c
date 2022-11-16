#include "main.h"
/**
 * This is the C entry point, upcalled once the hardware has been setup properly
 * in assembly language, see the reset.s file.
 */
#define NUMBER_OF_STRING 20
#define MAX_STRING_SIZE 50

int global_1=1;
int global_2=0;

void _start() {// including both interrupts and traps.
  int i = 0;
  int j = 0;
  int pointer=0;
  int offset=0;
  int k,l,cmd;
  char string[2];
  char cmd_1[6]={'r','e','s','e','t','\n'};
  char cmd_2[5]={'e','c','h','o',' '};
  char lines[NUMBER_OF_STRING][MAX_STRING_SIZE];
  //char string[MAX_STRING_SIZE];
  uart_send_string(UART0,"\033[2J\033[0;0H");
  uart_send_string(UART0, "\nQuit with \"C-a c\" and then type in \"quit\".\n");
  uart_send_string(UART0, "\nHello world! This is an emulated ARM shell with QEMU...\n");

  while (1) {
    unsigned char c;
    while (0 == uart_receive(UART0, &c)) {
      
      // friendly reminder that you are polling and therefore spinning...
      // not good for the planet! But until we introduce interrupts,
      // there is nothing you can do about it... except comment out
      // this annoying code ;-)
      continue;
    }

    if (c=="0x4B" && lines[i][j-1]== "0x0E") uart_send_string(UART0, 'right arrow');
    //Perform a left shift if NUMBER_OF_STRING is reached and the user writes a new line 
    //whithout hitting return which could be an empty line that we don't save
    //or backspace that we don't save as well

    if (i==NUMBER_OF_STRING && c!='\r' && c!=0x7F){ 
      for (k=0;k<i-1;k++){
        for (l=0;l<offset;l++){
          lines[k][l]=lines[k+1][l];
        }
      } 
      i--;
    }

    if (offset<MAX_STRING_SIZE){
      switch (c)
      {
      case '\r':
        uart_send(UART0, '\n');
        pointer++;
        if (lines[i][offset-1]!='\n'){//If "return" isn't hit yet
        lines[i][offset]='\n';
        
        //include string.h doesn't work
        /*memcpy(string, lines, strlen(lines[i]));
        string[strlen(lines[i])] = '\0'; 
        uart_send_string(UART0, string);*/

        //Display the line character by character 
        /*int n=0;
        while (lines[i][n]!='\n')
        {
          uart_send(UART0, lines[i][n]);
          n++;
        }
        uart_send(UART0, '\n');*/

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
          uart_send_string(UART0,"\033[2J\033[0;0H");
        }
        else if (cmd==2){//echo
          k++;
          while (lines[i][k]!='\n')
          {
           uart_send(UART0,lines[i][k]);
           k++;
          }
          uart_send(UART0,'\n');
          
        }
    
        /*Update the number of lines and the offset*/
        i++; //new line
        offset=0;
        }
        break;

        case 0x7F:
        if (pointer==offset)
        {
          offset--;
          pointer--;
        }
        else if (pointer<offset){
          //Left shift 
          for ( j=offset-1;j>=pointer;j--){
            lines[i][j+1]=lines[i][j];
          }
        }
        uart_send_string(UART0,"\b\x20\b");
        
        break;

        case '\033':
        while (0 == uart_receive(UART0, &c));
        while (0 == uart_receive(UART0, &c));
        if (c=='D'){//left arrow is pressed
          if (pointer>0) pointer--;
          uart_send_string(UART0,"\033\[\D");
        } else if (c=='C'){//right arrow is pressed
          if (pointer<offset) pointer++;
          uart_send_string(UART0,"\033\[\C");
        }
        break;

        default:
        if (pointer<offset)
        {        
          uart_send(UART0, c);
          //Right shift 
          for (j=pointer;j<offset;j++) {
            lines[i][offset+pointer-j+1]=lines[i][offset+pointer-j];
            uart_send(UART0, lines[i][j]);
            }
          lines[i][pointer]=c;
          pointer++;
          offset++;
          uart_send_string(UART0,"\033\[\D");
          }
        else{
        uart_send(UART0, c);
        lines[i][offset]=c; 
        offset++;
        pointer++;
        }
        break;
      }
      //compare offset to pointer and  see if we move..



      // if (c=='\r'){
        
      // }
      // else if (c!=0x7F) {
      //   }
      // else j--; //just to not forget it
    }
    else { 
      uart_send_string(UART0, "\nMax String Size is reached!");
      offset=0;
      i++;
    }
  }
}
