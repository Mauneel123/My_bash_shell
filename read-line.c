
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<termios.h>
#define MAX_BUFFER_LINE 2048

extern void tty_raw_mode(void);

// Buffer where line is stored
int line_length;
char line_buffer[MAX_BUFFER_LINE];
int ind=0;
// Simple history array
// This history does not change. 
// Yours have to be updated.
int history_index = 0;
char * history [] = {
  "ls -al | grep x", 
  "ps -e",
  "cat read-line-example.c",
  "vi hello.c",
  "make",
  "ls -al | grep xxx | grep yyy"
};
int history_length = sizeof(history)/sizeof(char *);

void read_line_print_usage()
{
  char * usage = "\n"
    " ctrl-?       Print usage\n"
    " Backspace    Deletes last character\n"
    " up arrow     See last command in the history\n";

  write(1, usage, strlen(usage));
}

/* 
 * Input a line with some basic editing.
 */
char * read_line() 
{
  
  // Set terminal in raw mode
  tty_raw_mode();

  line_length = 0;
  ind=0;

  // Read one line until enter is typed
  while (1) 
  {

    // Read one character in raw mode.
    char ch;
    read(0, &ch, 1);

    if (ch>=32)
    {

      // It is a printable character. 

      // Do echo
      write(1,&ch,1);

      // If max number of character reached return.
      if (line_length==MAX_BUFFER_LINE-2) break; 

      // add char to buffer.
      line_buffer[ind]=ch;
      line_length++;
      ind++;
    }
    else if (ch==10) {
      // <Enter> was typed. Return line
      
      // Print newline
      write(1,&ch,1);

      break;
    }
    else if (ch == 31) {
      // ctrl-?
      read_line_print_usage();
      line_buffer[0]=0;
      break;
    }
    else if (ch == 8) {
      // <backspace> was typed. Remove previous character read.

      // Go back one character
      ch = 8;
      write(1,&ch,1);

      // Write a space to erase the last character read
      ch = ' ';
      write(1,&ch,1);

      // Go back one character
      ch = 8;
      write(1,&ch,1);

      // Remove one character from buffer
      line_length--;
    }
        //control A
    else if (ch==1)
            {
               int i;
               for(i=ind;i>0;i--)
                {
                  ch=8;
                  write(1,&ch,1);
                  ind--;
                }
            }
      //control E
    else if(ch==5)
            {
              int i;
               for(i=ind;i<line_length;i++)
                {
                  char ch= line_buffer[i];
                  write(1,&ch,1);
                  ind++;
                }
            }
      //control D
    else if(ch==4)
          {
            if(ind <= 0 || ind >= line_length)
              continue;
            int i=ind;
            while(i<line_length-1)
                {
                  line_buffer[i]= line_buffer[i+1];
                  write(1,line_buffer[i],1);
                  i++;
                }

                line_buffer[line_length - 1]=0;
                line_length--;
                if(ind == line_length){
                  ind--;
                }
               for(i=ind;i<line_length;i++)
                  {
                    ch=line_buffer[i];
                    write(1,&ch,1);
                  }
                  ch=8;
                
                ch = ' ';
                write(1, &ch, 1);

                for(i=ind;i<=line_length;i++)
                  {
                    ch=8;
                    write(1,&ch,1);
                  }
          }
          
    else if (ch==27) {
      // Escape sequence. Read two chars more
      //
      // HINT: Use the program "keyboard-example" to
      // see the ascii code for the different chars typed.
      //
      char ch1; 
      char ch2;
      read(0, &ch1, 1);
      read(0, &ch2, 1);
      if (ch1==91 && ch2==65) 
      {
            // Up arrow. Print next line in history.

            // Erase old line
            // Print backspaces
            int i = 0;
            for (i =0; i < line_length; i++) {
              ch = 8;
              write(1,&ch,1);
            }

            // Print spaces on top
            for (i =0; i < line_length; i++) {
              ch = ' ';
              write(1,&ch,1);
            }

            // Print backspaces
            for (i =0; i < line_length; i++) {
              ch = 8;
              write(1,&ch,1);
            }	

            // Copy line from history
            strcpy(line_buffer, history[history_index]);
            line_length = strlen(line_buffer);
            history_index=(history_index+1)%history_length;
            // echo line
            write(1, line_buffer, line_length);
        }
                //left
            if (ch1==91 && ch2==68)
            {
                if(ind>0)
                {
                  ch=8;
                  write(1,&ch,1);
                  ind--;
                }
            }
          //right
            if (ch1==91 && ch2==67)
            {
                if(ind<line_length)
                {
                  char ch=line_buffer[ind];
                  write(1,&ch,1);
                  ind++;
                }
            }

           
          
                
    }

  }

  // Add eol and null char at the end of string
  line_buffer[line_length]=10;
  line_length++;
  line_buffer[line_length]=0;

  return line_buffer;
}

