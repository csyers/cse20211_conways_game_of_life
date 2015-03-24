// life.c		cse 20211	Lab 9
// Christopher Syers	November 19, 2014

#include <stdio.h>

static int MAX = 40;					// size of the board - can be adjusted easily
static int pause = 50000;				// length of delay when 'p' is selected
void init_board(char[MAX][MAX]);			// fills a board with blank spaces
void display_board(char[MAX][MAX]);			// prints out board
void update_board(char[MAX][MAX],char[MAX][MAX]);	// applies the rules of life to the current board and updates it
int num_surround(int,int,char[MAX][MAX]);		// checks all surrounding cells and returns the number of surrounding 'x' cells

int main(int argc,char *argv[])
{
 FILE *p;
 int row, col;
 char com = 'a';					// initialzed to 'a' to allow it to enter loop for the first time
 char board[MAX][MAX];					
 char temp_board[MAX][MAX];				// temp board for storing the board during period when rules are applied
 init_board(board);					// makes board all spaces to start
 init_board(temp_board);				// makes temp_board all spaces to start
 if(argc==2){						
   p = fopen(argv[1],"r");				// p - FILE * to other argument (data file) passed in command line
   if(!p){						// returns an error if the file does not exist
     printf("Error: input file does not exist\n");
     return 1;
   }
 }
 else if(argc==1){					// p - FILE * to stdin if only executable is passed into command line
   p = stdin;
 }
 else{
   printf("Please enter command in the form of '[*.x]' or '[*.x] [commands.txt]'\n");	// returns an error if a different fromat is passed into command line
   return 1;
 }
 while(com!='q'){					// stops when the user enters 'q'
   // only prints the next line if the entering interactive mode - no need to print if the commands are being read from a data file
   if (p == stdin) printf("Enter 'a' to add alive cell, 'r' to remove alive cell, 'n' to advance simulation 1 iteration, 'p' to play simulation, and 'q' to quit: ");
   fscanf(p," %c",&com);			// gets a character from the FILE *p. Space before %c prevents the program from reading the newline as the char.
   if(isalpha(com)){				// needs to be an alphabetic char to add it
     switch(com){
       case 'a':				// option to add an alive cell
	 fscanf(p," %d %d",&row,&col);		// gets 2 numbers from the FILE *
	 board[row][col]='x';			// changes the board at that position to an alive cell
	 if (p == stdin){			// displays the board if the program is running in interactive mode
           system("clear");
	   display_board(board);
	 }
	 break;
       case 'r':				// option to remove an alive cell
	 fscanf(p," %d %d",&row,&col);		// gets a row and col from FILE *p
	 board[row][col]=' ';			// changes the board at that position it a blank space
	 if (p == stdin){			// displays the board if the program is running in interactive mode
	   system("clear");
	   display_board(board);
         }
	 break;
       case 'n':
	 update_board(board,temp_board);	// applies the rules of life to the board
         system("clear");			
	 display_board(board);			// displays the updated board
         break;
       case 'p':
         while(1){				// infinte while that clears screen, applies the rules once, displays the update, and then pauses. 
           system("clear");			// End when the user presses control c
           update_board(board,temp_board);
           display_board(board);
           usleep(pause);
         }
     }
   }
 }
}

// function to make all spaces in a passed in 2d array to ' '
void init_board(char board[][MAX])
{
 int i,j;
 for(i=0;i<MAX;i++)
  for(j=0;j<MAX;j++)
   board[i][j]=' ';				// nested loops run through all position and change them to blank spaces.
}
// function that displays the board with a border of dashes and * corners
void display_board(char board[][MAX])
{
  int i,j,k;
  printf("*");				// top left corner is a star
  for(k=0;k<MAX;k++){
    printf("-");			// prints as many '-' as there are elements in the board
  }
  printf("*");				// top right corner is a star
  printf("\n");
  for(i=0;i<MAX;i++){			// nested loops go through every position of the board to be displayed
    for(j=0;j<MAX;j++){
      if(j==0){				// printf a '|' before every row
        printf("|");
      }
      printf("%c",board[i][j]);		
    }
    printf("|\n");			// prints a '|' and a newline after every row 
  }
  printf("*");				// star for bottom left
  for(k=0;k<MAX;k++)
    printf("-");			// '-' for the bottom border
  printf("*\n");			// * for bottom right
}

// function to apply the rules of life
void update_board(char board[][MAX],char updated_board[][MAX])
{
 int i,j,k,l,surround=0;
 for(i=0;i<40;i++){										// nested loops look at every cell in the board
   for(j=0;j<40;j++){
     surround=num_surround(i,j,board);								// calls function that returns the number of surrounding alive cells
     if(surround<2) updated_board[i][j] = ' ';							// rule: the cell dies if it has fewer that 3 neighbors
     if((surround==2 || surround==3) && board[i][j]=='x') updated_board[i][j] = 'x';		// rule: an alive cell stays alive when it has 2 or 3 neighbors
     if(surround==3 && board[i][j]==' ') updated_board[i][j] = 'x';				// rule: a dead cell is regenerated if it has exactly 3 neighbors 
     if(surround>3) updated_board[i][j] = ' ';							// rules: a cell is dead if it has more that 3 neighbors
   }
 }
  for(k=0;k<40;k++){
    for(l=0;l<40;l++){
      board[k][l]=updated_board[k][l];							// after all cells are looked at, changes board to look like updated board
    }
  }
  init_board(updated_board);								// changes the updated_board back to all blank spaces
}

// returns the number cell alive that neighbor the board position at (row,col)
int num_surround(int row, int col, char board[][MAX])
{
 int surround = 0;
 if(col-1>=0 && row-1>=0)					// checks above left spot for alive cell
   if(board[row-1][col-1]=='x') surround++;
 if(col-1>=0 && row+1<MAX)					// checks below left spot for alive cell
   if(board[row+1][col-1]=='x') surround++;
 if(col-1>=0)
   if(board[row][col-1]=='x') surround++;			// checks left spot for alive cell
 if(col+1<MAX && row-1>=0)
   if(board[row-1][col+1]=='x') surround++;			// checks above right cell for alive cell
 if(col+1<MAX && row+1<MAX)
   if(board[row+1][col+1]=='x') surround++;			// checks below right cell for alive cell
 if(col+1<MAX)
   if(board[row][col+1]=='x') surround++;			// checks right spot for alive cell
 if(row+1<MAX)
   if(board[row+1][col]=='x') surround++;			// checks below spot for alive cell
 if(row-1>=0)
   if(board[row-1][col]=='x') surround++;			// checks above spot for alive cell
 return surround;
}
