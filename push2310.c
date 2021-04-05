#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/*
Struct that represents players
*/
struct Player{
	// stores the players score
	int score;
	// determines when a player can make a move
	bool turn;
	// stores the players board
	char** board;
	// initiates the players symbol O or X
	char symbol;
};

/**function used for arg error checks
*/
int arg_error_check(int argc, char** argv) {
	if(argc != 4) {//checks if number of arguments is equal to 4
		fprintf(stderr, "Usage: push2310 typeO typeX fname\n");//prints error message to stderr
		exit(1);
	}
	if((strcmp(argv[1], "0") != 0) && (strcmp(argv[1], "1") != 0) && (strcmp(argv[1], "H")!= 0)) {//checks if player types entered are valid
		fprintf(stderr, "Invalid player type\n");
		exit(2);
	}
	if((strcmp(argv[2], "0") != 0) && (strcmp(argv[2], "1") != 0) && (strcmp(argv[2], "H") != 0)) {
		fprintf(stderr,"Invalid player type\n");
		exit(2);
	}
	if(fopen(argv[3],"r") == NULL){// checks if file is present in directory
		fprintf(stderr, "No file to load from\n");
		exit(3);
	}
		
	return 0;
}

char* string_to_int(int number) {
	char* string = malloc(sizeof(char)*3);
	sprintf(string,"%d",number);
	return string;
}
/**
*takes in a file string, which name of the board file
*prints the board file;
*/
void print_board(char* filestring){
	FILE* file;
	file = fopen(filestring,"r");//intialises file pointer to be the boar file
	char* linetoread = malloc(sizeof(char) * 200);
	//allocates memory for string, to store board contents
	int counter = 0;// counter to skip the first two lines
	while(fgets(linetoread,sizeof(linetoread),file)){//reads file till EOF
		counter++;
		if(counter >= 3){
			linetoread = (char*)realloc(linetoread,sizeof(linetoread));//realocates memory that is not used 
			fprintf(stdout,"%s",linetoread);// prints board to stdout
		}
	}
	
}


/** stores board dimensions and returns them, useful for other fucntions
*outputs error if dimensions less than 2
*/
int* dimension_function(char* filestring){
	int* atoidimensions = malloc(sizeof(int)*200);
	FILE* file;
	file = fopen(filestring,"r");
	char* dimensions = malloc(sizeof(char)*200);
	fgets(dimensions,200,file);
	atoidimensions[0] = atoi(&dimensions[0]);//y dimension
	if(strlen(string_to_int(atoidimensions[0])) == 1) {
		atoidimensions[1] = atoi(&dimensions[1]);//x dimension
	}
	else if(strlen(string_to_int(atoidimensions[0])) == 2) {
		atoidimensions[1] = atoi(&dimensions[2]);
	}
	else {
		atoidimensions[1] = atoi(&dimensions[3]);
	}
	if(atoidimensions[0] <= 2 || atoidimensions[1] <= 2){
		fprintf(stderr,"Invalid file contents\n");
		exit(4);
	}
	dimensions = realloc(dimensions,sizeof(dimensions));
	atoidimensions = realloc(atoidimensions,sizeof(atoidimensions));
	fclose(file);
	return atoidimensions;
}

/*
function checks if player type is valid in program
outputs invalid file contents error if not
*/
void check_playertype(char* filestring){
	char* playerString = malloc(sizeof(char)*5);
	int counter;
	char playertype;
	FILE* file;
	file = fopen(filestring, "r");
	while(fgets(playerString,sizeof(playerString),file)){
		counter++;
		//counts up to 2, to line 2 of the file
		if(counter == 2){
			playertype = playerString[0];
		}
	}
		
	if((playertype != 'O') && (playertype != 'X')){
		fprintf(stderr,"Invalid file contents\n");
		exit(4);
	}
	fclose(file);
}

int check_spaces(char* string) {
	int counter = 0;
	for (int i =0; i < strlen(string); i++) {
		if(string[i] == ' ') {
			counter++;
		}
	}
	return counter;
}

/**
returns player type retrived from file
throws invalid file content if not apprporiate file found
*/
char get_playertype(char* filestring){
	char* playerString = malloc(sizeof(char)*5);
	int counter;
	char playertype;
	FILE* file;
	file = fopen(filestring, "r");
	while(fgets(playerString,sizeof(playerString),file)){
		counter++;
		if(counter == 2){
			playertype = playerString[0];
		}
	}
		
	if((playertype != 'O') && (playertype != 'X')){
		fprintf(stderr,"Invalid file contents\n");
		exit(4);
	}
	return playertype;
	fclose(file);
}


/*function that prints any stored board
*based on the num of rows and columns
*/
void print_stored_board(char* filestring,char** board){
	// stores the num of rows and columns retrived from file
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	// for loop that iterates through the board and prints each char
	for(int i =0; i < ydimension; i++){
		for(int x = 0; x < xdimension*2;x++) {
		fprintf(stdout,"%c",board[i][x]);
		}
		fprintf(stdout,"\n");// new line to seprate rows
	}
}

/*function that prints the winner based on the players score being higher than the other
*/
void print_winner(struct Player playerO, struct Player playerX) {
	if(playerO.score > playerX.score) {
		fprintf(stdout,"Winners: %c\n", playerO.symbol);
	}
	else if(playerX.score > playerO.score) {
		fprintf(stdout,"Winners: %c\n", playerX.symbol);
	}
	else {
		fprintf(stdout,"Winners: %c %c\n", playerO.symbol,playerX.symbol);
	}
}	
	
	

/**
*takes in a string which is a file name and allocates the board based on the contents of file
*returns the board retrived from file in form of char**
*/
char** file_board_array(char* filestring){
	int* dimensions = malloc(sizeof(int)*50);
	dimensions = dimension_function(filestring);
	int i = 0;
	int xdimension = dimensions[1];//number of columns in board
	int ydimension = dimensions[0];//number of rows in board
	FILE* file;// File pointer to open  "file" with
	file = fopen(filestring,"r");
	int counter = 0;// counter for while loop to ensure it skips the line with player type
	char** board = malloc(sizeof(char*)*ydimension);//allocates memory for rows on board
	for(int y = 0; y < ydimension; y++){
		board[y] = (char*)malloc(sizeof(char)*xdimension*2);// allocates memory for the columns on board, based on rows
	}
		while(fgets(board[i],(xdimension*2+(ydimension)),file)){//reads the file line by line until EOF
			counter++;
			if(counter >= 3){
				i++;
			}
		}
	fclose(file);//closes file
	return board;// returns the board
}

/** 
*checks if space in board is empty, for human player
*/

bool check_empty(int row, int column,char** board) {
	if(board[row][(column*2)+1] != '.') {
		return false;
	}
	return true;
}
/**
* checks for valid entry from human player
*/
bool check_for_valid_entry(char*filestring,int row, int column,char** board) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	// checks if row or column is negative, this is important as isdigit segfaults with negative values
	if(row < 0 || column < 0) {
		return false;
	}
	// checks if row is out of bounds of board, same as column
	// ensures entry is a digit
	if((row >= ydimension) || (column >= xdimension) 
		|| (isdigit(row)!=0) || (isdigit(column)!=0) 
	|| !check_empty(row,column,board)){
		return false;
	}
	
	return true;
}
			
	
	
	

/**
checks for a full board given in save file throws full board error and exits
if full board found
*/
void check_for_full(char* filestring){
	//stores the x and y dimesions
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	char** board = file_board_array(filestring);//initialises board
	int counter = 0;//counter for for loop
	for(int m = 1; m < ydimension - 1; m++){// goes through all rows in interior
		for(int k = 3; k < (xdimension*2)-1;k = k+2){//goes through all columns in interior
			if(board[m][k] == '.'){
				counter++;//adds one to counter everytime . is found
			}
		}
			
	}
	if(counter == 0) { // if no . is found counter will be 0
		fprintf(stderr,"Full board in load\n");// therefore full board
		exit(6);
	}
				
}

/*
*function that returns a boolean true if interior of board is full
*false otherwise
*/
bool full_board(char* filestring,char** board) {
	//stores the x and y dimesions
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	int counter = 0;//counter for for loop
	for(int m = 1; m < ydimension - 1; m++){// goes through all rows in interior
		for(int k = 3; k < (xdimension*2)-1;k = k+2){//goes through all columns in interior
			if(board[m][k] == '.'){
				counter++;//adds one to counter everytime . is found
			}
		}
			
	}
	// if counter finds no . it will return true as board is full
	if(counter == 0) {
		return true;
	}
	return false;
}



/*
function that handles save game 
*takes in a string input for file name
*players for the boards and types of player
*/
void save_game(char* filestring,char* input,struct Player playerO,struct Player playerX) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	FILE* file;
	file = fopen(input,"w");
	//prints the x and y dimensions of board
	fprintf(file,"%d %d\n",ydimension,xdimension);
	// checks if saved on player O turn then saves second line as O
	if(playerO.turn) {
		fprintf(file,"%c\n",playerO.symbol);
		//saves the board to file
		for(int a = 0; a < ydimension; a++) {
			fprintf(file,"%s",playerO.board[a]);

		}
	}
	else {
		fprintf(file,"%c\n",playerX.symbol);
		for(int a = 0; a <= ydimension; a++) {
			fprintf(file,"%s",playerX.board[a]);
		}
	}
	fclose(file);	
	
}
						
/**
functionality for two type 0 players
*/
void player00_func(int argc, char** argv, char* filestring) {
	//stores both x and y dimensions
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	//two player variables for the game
	struct Player playerO;
	struct Player playerX;
	//sets the symbol for the players
	playerO.symbol = 'O';
	playerX.symbol = 'X';
	//stores the board for players after starting moves.
	playerO.board = file_board_array(filestring);
	playerX.board = playerO.board;
	//prints the players starting moves
	//player_start(argc,argv,filestring);
	//calculates score after starting moves
	playerO.score = 0;
	playerX.score = 0;
	if(get_playertype(filestring) == 'O') {
		playerO.turn = true;//sets playerO turn to be true
	}
	else {
		playerX.turn = true;
		playerO.turn = false;
	}
	//for loop that terminates once board is full
	for(int i = 0; i < (xdimension*ydimension) ; i++){
		if(playerO.turn) {
			//iterates through the rows top to bottom
			for( int m = 1; m < ydimension - 1; m++) {
				//iterates through columns left to right
				for( int k = 3; k < (xdimension*2) - 2; k++) {
					//checks if there is an empty space
					if(playerO.board[m][k] == '.'){
						if(playerO.turn){
							//sets the position as players symbol
							playerO.board[m][k] = playerO.symbol;
							//sets the board as the same for other player
							playerX.board = playerO.board;
							//adds the score of the placing
							playerO.score += playerO.board[m][k - 1] - '0'; 
							//makes the players turn false
							playerO.turn = false;
							//makes other players turn true
							playerX.turn = true;
							//prints where player placed and prints the board
							fprintf(stdout,"Player %c placed at %d %d\n",playerO.symbol,m,k/2);
							print_stored_board(filestring,playerO.board);
					}
				}
			}
		}
	}
		if(playerX.turn){
			//itterates from bottom to top
			for(int a = ydimension - 2; a > 0 ; a--) {
				//itterates from right to left
				for( int b = (xdimension*2)-3; b > 2 ; b--){
					if(playerX.board[a][b] == '.'){
						if(playerX.turn){
							playerX.board[a][b] = playerX.symbol;
							playerO.board = playerX.board;
							playerX.score += playerO.board[a][b - 1] - '0';
							playerX.turn = false;
							playerO.turn = true;
							fprintf(stdout,"Player %c placed at %d %d\n",playerX.symbol,a,b/2);
							print_stored_board(filestring,playerX.board);
						}
					}
				}
			}
		}
	}
	//checks whose score is higher
	// and then prints winning player
	print_winner(playerO,playerX);
		
																
}
/*checks if there is an empty place in row
for edge play
*/
bool empty_place_in_row (char*filestring, char** board,int row) {
	int xdimension = dimension_function(filestring)[1];
	int counter = 0;
	for(int i =1; i < (xdimension*2); i++) {
		if(board[row][i] == '.') {
			counter++;
		}
	}
	// if counter > 1 player can place on edge
	if(counter > 1) {
		return true;
	}
	else{
		return false;
	}
}

/*checks if there is an empty place in row
for edge play
*/
bool empty_place_in_column(char*filestring, char** board,int column) {
	int ydimension = dimension_function(filestring)[0];
	int counter = 0;
	for(int i = 0; i < ydimension; i++) {
		if(board[i][column] == '.') {
			counter++;
		}
	}
	if(counter > 1) {
		return true;
	}
	else{
		return false;
	}
}

/*
*sums the scores of players based on the board and players symbol
*O or X
*/
int sumof_score(char* filestring,char symbol,char** board) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	int score = 0;
	// for loops to iterate through rows and columns 
	for(int i = 0; i < ydimension; i++) {
		for(int j = 0; j < xdimension*2;j++) {
			if(board[i][j] == symbol) {
				score+= board[i][j-1] - '0';// -'0' as the number  is 0 to 9, which returns correct value
			}
		}
	}
	return score;
}
/*
*function that checks if an edge is valid for a human player to play
ie player next to it alread placed and there is an empty spot to move
*/
bool check_for_valid_edge(char* filestring,int row, int column, char** board) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	int lrow = ydimension - 2;
	int lx = (xdimension*2)-3;
	if(row < 0 || column < 0) {
		return false;
	}
	// checks for any top edge play, column*2+1 to account for the . 
	if(row == 0) {
		if((board[1][(column*2)+1] == 'O' ||board[1][(column*2)+1] == 'X')&&
		(empty_place_in_column(filestring,board,(column*2)+1))
		&& board[0][(column*2)+1] == '.') {
			return true;
		}
	}
	//checks for any last row play
	if(row == ydimension-1) {
		if((board[lrow][(column*2)+1] == 'O' || board[lrow][(column*2)+1] == 'X') &&
			empty_place_in_column(filestring,board,(column*2)+1) && board[ydimension -1][(column*2)+1] == '.') {
				return true;
			}
	}
	// checks for last column play
	if(column == xdimension-1) {
		if((board[row][lx] == 'O' ||board[row][lx] == 'X') &&
			empty_place_in_row(filestring,board,row) && board[row][(xdimension*2)-1] == '.') {
				return true;
			}
	}
	// checks for last row play
	if(column == 0) {
		if((board[row][3] == 'O' ||board[row][3] == 'X') &&
			empty_place_in_row(filestring,board,row) && board[row][1] == '.')
			return true;
		}
	return false;
}

/*
function that handles the shift down of a row if human places at top of board
*/
char** shift_down(char* filestring,struct Player player,int row,int column) {
	int ydimension = dimension_function(filestring)[0];
	int yplace = 0;// variable used to find where to shift to
	int col = (column*2)+1;// to account for .
	player.board[row][col] = player.symbol; //places player at the designated area
	//for loop that iterates through rows
	for(int y = 0; y < ydimension; y++) {
		if(player.board[y][col] == '.') {//checks for closest empty space
			yplace = y;// stores the empty space
			break;
		}
	}
	for(int j = yplace -1 ; j>=0; j--) {//starts at the empty space
		player.board[j+1][col] = player.board[j][col];//goes backwards and makes char ahead equal to current char
		player.board[j][col] = '.';//makes current char empty
	}
	player.board[row][col] = '.';//makes the top where placed empty
	return player.board;
}

/*
function that handles the shift up of a row if human places at bottom of board
*/
char** shift_up(char* filestring,struct Player player,int row,int column) {
	int ydimension = dimension_function(filestring)[0];
	int yplace = 0;
	int col = (column*2)+1;
	player.board[row][col] = player.symbol; 
	for(int y = ydimension-1; y > 0; y--) {
		if(player.board[y][col] == '.') {
			yplace = y;// stores closest empty to the bottom of row
			break;
		}
	}
	for(int z = yplace+1; z <= ydimension-1; z++) {
		player.board[z-1][col] = player.board[z][col];//makes char below equal to current char
		player.board[z][col] = '.';//makes curren char empty
	}
	player.board[row][col] = '.';// makes bottom char placed att empty
	return player.board;						
				
}

/*
shifts board right if player places at last column
*/
char** shift_right( char* filestring,struct Player player,int row,int column) {
	int xdimension = dimension_function(filestring)[1];
	int xplace = 0;
	int col = (column*2)+1;
	player.board[row][col] = player.symbol;
	for(int a = (xdimension*2)-1; a >= 0; a--) {
		if(player.board[row][a] == '.') {
			xplace = a;//stores closest empty space
			break;
		}
	}
	for(int v = xplace+2; v <= (xdimension*2) -1; v+=2) {
		player.board[row][v - 2] = player.board[row][v];//shifts right from empty by making char
		//2 spaces left of it the same as current char
		player.board[row][v] = '.';//makes the current space empty
	}
	player.board[row][col] = '.';// makes player placed at empty
	return player.board;
	
}

/*
shifts board lefts if player places at first column
*/
char** shift_left( char* filestring,struct Player player,int row,int column) {
	int xdimension = dimension_function(filestring)[1];
	int xplace = 0;
	int col = (column*2)+1;
	player.board[row][col] = player.symbol;
	for(int a = 0; a < (xdimension*2); a++) {
		if(player.board[row][a] == '.') {
			xplace = a;//stores closest empty space
			break;
		}
	}
	for(int z = xplace-2;  z >= 1 ; z-=2) {
		player.board[row][z+2] = player.board[row][z];//shifts left from empty by making char
		//2 spaces right of it the same as current char
		player.board[row][z] = '.';
	}
	player.board[row][col] = '.';// makes player placed at empty
	return player.board;
		
}	
/*
function that deep coppies a board(ie stores a different memory address)
*allows changes to be made to the coppy or original without altering the other
*/
char** deep_copy(char* filestring, char** board) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	char** boardcopy = malloc(sizeof(char*)*ydimension);// board that will be copy of board
	for(int y = 0; y <ydimension; y++) {
		boardcopy[y] = malloc(sizeof(char)*xdimension*2);//mallocs memory for board
	}
	for(int i = 0; i < ydimension; i++) {
		for(int j = 0; j < xdimension*2; j++) {
			boardcopy[i][j] = board[i][j];// coppies each char of board storing it to a different address 
		}
	}
	return boardcopy;
}

/*
*stores all the columns of the top row moves in an array
*/
int* validtop_moves(char* filestring,struct Player player) {
	int xdimension = dimension_function(filestring)[1];
	int* columns = malloc(sizeof(int) *20);//mallocs space for columns
	int i = 0;//index of columns array
	//for loop that iterates through columns,note that it goes right to left maintaining clockwise nature
	for(int column = 0; column < xdimension*2;column++) { 
		// if statments checks through and makes sure if move would be valid in the column
		if((player.board[1][column] == 'O' ||player.board[1][column] == 'X')&&
		empty_place_in_column(filestring,player.board,column) 
		&& player.board[0][column] == '.') {
			columns[i] = column;// stores the column based on index
			i++;
		}
	}
	columns = realloc(columns,sizeof(columns));// reallocates based on actual size
	return columns;
}

/**
*checks if the top edge is valid
*returns bool, similar to last function
*/
bool check_for_topedge(char* filestring,struct Player player) {
	int xdimension = dimension_function(filestring)[1];
	for(int column = 0; column < xdimension*2;column++) { 
		if((player.board[1][column] == 'O' ||player.board[1][column] == 'X')&&
		empty_place_in_column(filestring,player.board,column) 
		&& player.board[0][column] == '.') {
			return true;
		}
	}
	return false;
}

/**
this shift down function same as the human shift down, accpet column is not altered
*as it already in correct index,check(shiftdown) for more info if needed
*/
char** shift_downp1(char* filestring,struct Player player,int row,int column) {
	int ydimension = dimension_function(filestring)[0];
	int yplace = 0;
	player.board[row][column] = player.symbol; 
	for(int y = 0; y < ydimension; y++) {
		if(player.board[y][column] == '.') {
			yplace = y;
			break;
		}
	}
	for(int j = yplace -1 ; j>=0; j--) {
		player.board[j+1][column] = player.board[j][column];
		player.board[j][column] = '.';
	}
	player.board[row][column] = '.';
	return player.board;
}

/**
function that handles top row play of a player1 type
returns the board after completion
*/
char** player1_toprow(char* filestring,struct Player player,struct Player player2) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	struct Player aftershift;// player used to compare boards with
	aftershift.symbol = player.symbol;
	aftershift.board = malloc(sizeof(char*)*ydimension);
	for(int y = 0; y <ydimension; y++) {
		aftershift.board[y] = malloc(sizeof(char)*xdimension*2);// mallocs space for board
	}
	int* columns = malloc(sizeof(int)*20);// array to store returned array of valid move indexes
	columns = validtop_moves(filestring,player);
	if(check_for_topedge(filestring,player)) {//checks if top edge is valid to make a move on
		for(int i = 0; i < xdimension; i++) {// iterates through board
			if(columns[i] > (xdimension*2) || columns[i] < 0){// if value excedes xdimension*2 to account for . or is less than 0 
			// then break out of loop
				break;
			}
			aftershift.board = deep_copy(filestring,player.board);// deep copies player board to aftershift to allow 1 column change 
			aftershift.board = shift_downp1(filestring,aftershift,0,columns[i]);//shifts the coppied board columns[i] down
			// checks if score of opposing player (player 2) is lower than in the original board
			if(sumof_score(filestring,player2.symbol,player.board) > sumof_score(filestring,player2.symbol,aftershift.board)) {
				// if yes make original board equal shifted board
				player.board = aftershift.board;
				// print the where player places and the board, then break
				fprintf(stdout,"Player %c placed at %d %d\n",player.symbol,0,columns[i]/2);
				print_stored_board(filestring,player.board);
				break;
			
			}
			
			
		}
	}
	return player.board;
}

/**
*Checks for all the valid columns for any bottom row moves
*/
int* validbottom_moves(char* filestring,struct Player player) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	int lrow = ydimension - 2;//the last interior row of the board, to make if more readable
	int* columns = malloc(sizeof(int) *20);
	int i = 0;
	//for loop itterates from last column (xdimension*2) -3 to column 0 to maintain clockwise direction
	for(int x = (xdimension*2)-3; x >= 0;x--) {
		if((player.board[lrow][x] == 'O' || player.board[lrow][x] == 'X') &&//checks if the spot above is taken,if there is empty space
		empty_place_in_column(filestring,player.board,x) && player.board[ydimension -1][x] == '.') {
			columns[i] = x;//stores the index
			i++;//increments index of array
		}
	}
	columns = realloc(columns,sizeof(columns));
	return columns;
}

/**
same as valid_bottom move
* accept returns boolean
*/
bool check_for_bottomedge(char* filestring, struct Player player) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	int lrow = ydimension - 2;
	for(int x = (xdimension*2)-3; x >= 0;x--) {
		if((player.board[lrow][x] == 'O' || player.board[lrow][x] == 'X') &&
		empty_place_in_column(filestring,player.board,x) && player.board[ydimension -1][x] == '.') {
			return true;
		}
	}
	return false;
}

/**
same shift up as human player just the column is not altered
check shiftup for more info
*/
char** shift_upp1(char* filestring,struct Player player, int row, int column) {
	int ydimension = dimension_function(filestring)[0];
	int yplace = 0;
	player.board[row][column] = player.symbol; 
	for(int y = ydimension-1; y > 0; y--) {
		if(player.board[y][column] == '.') {
			yplace = y;
			break;
		}
	}
	for(int z = yplace+1; z <= ydimension-1; z++) {
		player.board[z-1][column] = player.board[z][column];
		player.board[z][column] = '.';
	}
	player.board[row][column] = '.';
	return player.board;						
				
}

/**
controls play of the bottom row of players
*returns the board altered or not altered
*same as player1_toprow but the shift is done using
*shift_downp1
*/
char** player1_bottomrow(char* filestring, struct Player player,struct Player player2) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	struct Player aftershift;
	aftershift.symbol = player.symbol;
	aftershift.board = malloc(sizeof(char*)*ydimension);
	for(int y = 0; y <ydimension; y++) {
		aftershift.board[y] = malloc(sizeof(char)*xdimension*2);
	}
	int* columns = malloc(sizeof(int)*20);
	columns = validbottom_moves(filestring,player);
	if(check_for_bottomedge(filestring,player)) {
		for(int i =0; i < xdimension; i++) {
			if(columns[i] > (xdimension*2) || columns[i] < 0){
				break;
			}
			aftershift.board = deep_copy(filestring,player.board);
			aftershift.board = shift_upp1(filestring,aftershift,ydimension-1,columns[i]);//shifts the deep coppied board down
			if(sumof_score(filestring,player2.symbol,player.board) > sumof_score(filestring,player2.symbol,aftershift.board)) {
					player.board = aftershift.board;
					fprintf(stdout,"Player %c placed at %d %d\n",player.symbol,ydimension-1,columns[i]/2);
					print_stored_board(filestring,player.board);
					break;
			}
		}
	}
	return player.board;
}

/**
same as valid_bottom and valid_top but for last column
*/
int* valid_lastcol_moves(char* filestring, struct Player player) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	int lx = (xdimension*2)-3;
	int* rows = malloc(sizeof(int) *20);
	int i = 0;
	for(int j = ydimension -2 ; j >=1; j--) {
		if((player.board[j][lx] == 'O' ||player.board[j][lx] == 'X') &&
		empty_place_in_row(filestring,player.board,j)&& player.board[j][(xdimension*2)-1] == '.') {
				rows[i] = j;
				i++;
		}
	}
	rows = realloc(rows,sizeof(rows));
	return rows;
}

bool check_for_lastedge(char* filestring,struct Player player) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	int lx = (xdimension*2)-3;
	for(int j = 0; j < ydimension; j++) {
		if((player.board[j][lx] == 'O' ||player.board[j][lx] == 'X') &&
		empty_place_in_row(filestring,player.board,j) && player.board[j][(xdimension*2)-1] == '.') {
			return true;
		}
	}
	return false;
}

char** shift_rightp1(char* filestring,struct Player player, int row, int column) {
	int xdimension = dimension_function(filestring)[1];
	int xplace = 0;
	player.board[row][column] = player.symbol;
	for(int a = (xdimension*2)-1; a >= 0; a--) {
		if(player.board[row][a] == '.') {
			xplace = a;
			break;
		}
	}
	for(int v = xplace+2; v <= (xdimension*2) -1; v+=2) {
		player.board[row][v - 2] = player.board[row][v];
		player.board[row][v] = '.';
	}
	player.board[row][column] = '.';
	return player.board;
}

char** player1_lastcolumn(char* filestring, struct Player player,struct Player player2) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	struct Player aftershift;
	aftershift.symbol = player.symbol;
	aftershift.board = malloc(sizeof(char*)*ydimension);
	for(int y = 0; y <ydimension; y++) {
		aftershift.board[y] = malloc(sizeof(char)*xdimension*2);
	}
	int* rows = malloc(sizeof(int)*20);
	rows = valid_lastcol_moves(filestring,player);
	if(check_for_lastedge(filestring,player)) {
		for(int i =0; i < ydimension; i++) {
			if(rows[i] > ydimension || rows[i] < 0) {
				break;
			}
			aftershift.board = deep_copy(filestring,player.board);
			aftershift.board = shift_rightp1(filestring,aftershift,rows[i],(xdimension*2)-1);
			if(sumof_score(filestring,player2.symbol,player.board) > sumof_score(filestring,player2.symbol,aftershift.board)) {
					player.board = aftershift.board;
					fprintf(stdout,"Player %c placed at %d %d\n",player.symbol,rows[i],(xdimension*2-1)/2);
					print_stored_board(filestring,player.board);
					break;
			}
		}
	}
	return player.board;
}

int* valid_firstcol_moves(char* filestring,struct Player player) {
	int ydimension = dimension_function(filestring)[0];
	int* rows = malloc(sizeof(int) *20);
	int j = 0;
	for(int i = ydimension - 1; i>=0; i--) {
		if((player.board[i][3] == 'O' ||player.board[i][3] == 'X') &&
		empty_place_in_row(filestring,player.board,i) && player.board[i][1] == '.') {
			rows[j] = i;
			j++;
		}
	}
	rows = realloc(rows,sizeof(rows));
	return rows;
}

bool check_for_firstedge(char* filestring, struct Player player) {
	int ydimension = dimension_function(filestring)[0];
	for(int i = ydimension - 1; i>=0; i--) {
		if((player.board[i][3] == 'O' ||player.board[i][3] == 'X') &&
		empty_place_in_row(filestring,player.board,i) && player.board[i][1] == '.') {
			return true;
		}
	}
	return false;
}

char** shift_leftp1( char* filestring,struct Player player,int row,int column) {
	int xdimension = dimension_function(filestring)[1];
	int xplace = 0;
	player.board[row][column] = player.symbol;
	for(int a = 0; a < (xdimension*2); a++) {
		if(player.board[row][a] == '.') {
			xplace = a;
			break;
		}
	}
	for(int z = xplace-2;  z >= 1 ; z-=2) {
		player.board[row][z+2] = player.board[row][z];
		player.board[row][z] = '.';
	}
	player.board[row][column] = '.';
	return player.board;

		
}	


char** player1_firstcolumn(char* filestring, struct Player player,struct Player player2) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	struct Player aftershift;
	aftershift.symbol = player.symbol;
	aftershift.board = malloc(sizeof(char*)*ydimension);
	for(int y = 0; y <ydimension; y++) {
		aftershift.board[y] = malloc(sizeof(char)*xdimension*2);
	}
	int* rows = malloc(sizeof(int)*20);
	rows = valid_firstcol_moves(filestring,player);
	if(check_for_firstedge(filestring,player)) {
		for(int i =0; i < ydimension; i++) {
			if(rows[i] > ydimension || rows[i] < 0){
				break;
			}
			aftershift.board = deep_copy(filestring,player.board);
			aftershift.board = shift_leftp1(filestring,aftershift,rows[i],1);
			if(sumof_score(filestring,player2.symbol,player.board) > sumof_score(filestring,player2.symbol,aftershift.board)) {
					player.board = aftershift.board;
					fprintf(stdout,"Player %c placed at %d %d\n",player.symbol,rows[i],0);
					print_stored_board(filestring,player.board);
					break;
			}
		}
	}
	return player.board;
}
/**
*function that finds the interior place with the highest score
*place has to be empty, in case of a tie, it is which ever comes first
*going left to right top to bottom
*/
int find_maxinterior(char* filestring,char** board) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	int max = 0; // variable to compare the score of position with
	//itterates through board
	for(int i = 1; i < ydimension - 1; i++) {
		for( int x = 2; x < (xdimension*2) - 3; x+=2) {
			if(board[i][x+1] == '.') {// if spot next to score is empty
				if(max < board[i][x] - '0') {// if max is less than that score
					max = board[i][x] - '0';//make max = that score
				}
			}
		}
	}
	return max; // returns the max value found
}

/**
*controls play of the interior
* returns board after player places
*/
char** player1_interior(char* filestring,struct Player player) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	int maxium = find_maxinterior(filestring,player.board);// variable to store the maxiumum empty space 
	if(player.turn) {
		for(int i = 1; i < ydimension - 1; i++) {
			for( int x = 2; x < (xdimension*2) - 3; x+=2) {
				if(player.board[i][x] - '0' == maxium // if space = max and is empty
				&& player.board[i][x+1] == '.') {
					if(player.turn) {// makes sure only placed once
						player.board[i][x+1] = player.symbol;//make spot = player symbol O or X
						player.turn = false;//makes it false to avoid loop iterating through  again
						fprintf(stdout,"Player %c placed at %d %d\n",player.symbol,i,x/2);
					}
				}
			}
		}
	}
	print_stored_board(filestring,player.board);
	return player.board;
}

	
/**
*function that controls player 1 functionality
*calls player1_toprow,player1_bottomrow,player1_lastcolumn,player1_firstcolumn
* and player1_interior
*/	
void player1_func(int argc,char** argv,char*filestring) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	struct Player playerO;
	struct Player playerX;
	char** turnBoard = malloc(sizeof(char*)*ydimension);//to compare the changed board with
	for(int y = 0; y < ydimension;y++ ) {
		turnBoard[y] = malloc(sizeof(char)*xdimension*2);//malloc space for the board
	}
	playerO.symbol = 'O';
	playerX.symbol = 'X';
	playerO.board = file_board_array(filestring);// makes player O board = the board from file
	playerX.board = playerO.board;//trasnfers the board over to the playerX's board
	if(get_playertype(filestring) == 'O') {//gets the player type from file to determine whose turn it is
		playerO.turn = true;
	}
	else {
		playerX.turn = true;
		playerO.turn = false;
	}
	//goes to xdimension*2*ydimension to ensure game never ends prematureley
	for(int q = 0; q < xdimension*2*ydimension; q++) {// for loop to keep the game going for as long as needed
		if(playerO.turn) {
			if(full_board(filestring,playerO.board)) {//if the interior of board is full break out of loop
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerO.board[k][g];//deep copies playerO's board before player O shifts
				}
			}
			playerO.board = player1_toprow(filestring,playerO,playerX);//places on top row and plays according to top row function
			if(sumof_score(filestring,playerX.symbol,playerO.board) != 
			sumof_score(filestring,playerX.symbol,turnBoard)) {// if sum of shift board is != to turnBoard
				playerO.turn = false;// change has occured make player O turn false
				playerX.turn = true;//playerX true
				playerX.board = playerO.board;//trasnfer the board
			}
			else {
				playerO.turn = true;//else keep going through the other options
				playerX.turn = false;
			}
			
		}
		
		if(playerO.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerO.board[k][g];
				}
			}
			playerO.board = player1_lastcolumn(filestring,playerO,playerX);// shifts left if possible
			if(sumof_score(filestring,playerX.symbol,playerO.board) != 
			sumof_score(filestring,playerX.symbol,turnBoard)) {
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			else {
				playerO.turn = true;
				playerX.turn = false;
			}
			
		
			
		}
		if(playerO.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerO.board[k][g];
				}
			}
			playerO.board = player1_bottomrow(filestring,playerO,playerX);//shifts up if pssoible
			if(sumof_score(filestring,playerX.symbol,playerO.board) != 
			sumof_score(filestring,playerX.symbol,turnBoard)) {
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			else {
				playerO.turn = true;
				playerX.turn = false;
			}
			
			

		}
		if(playerO.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerO.board[k][g];
				}
			}
			playerO.board = player1_firstcolumn(filestring,playerO,playerX);//shifts right if possible
			if(sumof_score(filestring,playerX.symbol,playerO.board) != 
			sumof_score(filestring,playerX.symbol,turnBoard)) {
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			else {
				playerO.turn = true;
				playerX.turn = false;
			}
		
			
		}
		if(playerO.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			playerO.board = player1_interior(filestring,playerO);// if all above options are invalid place in highest interior
			playerO.turn = false;
			playerX.turn = true;
			playerX.board = playerO.board;
		}
		//following does the same as player O accept for playerX
		if(playerX.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
			for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerX.board[k][g];
				}
			}
			playerX.board = player1_toprow(filestring,playerX,playerO);
			if(sumof_score(filestring,playerO.symbol,playerX.board) != 
			sumof_score(filestring,playerO.symbol,turnBoard)) {
				playerX.turn = false;
				playerO.turn = true;
				playerO.board = playerX.board;
			}
			else {
				playerX.turn = true;
				playerO.turn = false;
			}
		
		}
		
		
		if(playerX.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerX.board[k][g];
				}
			}
			playerX.board = player1_lastcolumn(filestring,playerX,playerO);
			if(sumof_score(filestring,playerO.symbol,playerX.board) != 
			sumof_score(filestring,playerO.symbol,turnBoard)) {
				playerX.turn = false;
				playerO.turn = true;
				playerO.board = playerX.board;
			}
			else {
				playerX.turn = true;
				playerO.turn = false;
			}
		}
		
		if(playerX.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerX.board[k][g];
				}
			}
			playerX.board = player1_bottomrow(filestring,playerX,playerO);
			if(sumof_score(filestring,playerO.symbol,playerX.board) != 
			sumof_score(filestring,playerO.symbol,turnBoard)) {
				playerX.turn = false;
				playerO.turn = true;
				playerO.board = playerX.board;
			}
			else {
				playerX.turn = true;
				playerO.turn = false;
			}
			
			
		}
		if(playerX.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerX.board[k][g];
				}
			}
			playerX.board = player1_firstcolumn(filestring,playerX,playerO);
			if(sumof_score(filestring,playerO.symbol,playerX.board) != 
			sumof_score(filestring,playerO.symbol,turnBoard)) {
				playerX.turn = false;
				playerO.turn = true;
				playerO.board = playerX.board;
			}
			else {
				playerX.turn = true;
				playerO.turn = false;
			}
			
			
		}
		
		if(playerX.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			playerX.board = player1_interior(filestring,playerX);
			playerO.board = playerX.board;
			playerX.turn = false;
			playerO.turn = true;
		}
		
	}
	//calculates the score after interior of board is full and prints the winner of the game
	playerO.score = sumof_score(filestring,playerO.symbol,playerO.board);
	playerX.score = sumof_score(filestring,playerX.symbol,playerX.board);
	print_winner(playerO,playerX);
	
}

/*
controls for 01 game if argv[1] i 0 and argv[2] is 1
*playerO is type 0(same as playerO from player00_func)
*player X is type 1 same as from player1_func
*/
void player01_func(int argc, char** argv,char* filestring) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	//two player variables for the game
	struct Player playerO;
	struct Player playerX;
	char** turnBoard = malloc(sizeof(char*)*ydimension);
	for(int y = 0; y < ydimension;y++ ) {
		turnBoard[y] = malloc(sizeof(char)*xdimension*2);
	}
	//sets the symbol for the players
	playerO.symbol = 'O';
	playerX.symbol = 'X';
	//stores the board for players after starting moves.
	playerO.board = file_board_array(filestring);
	playerX.board = playerO.board;
	//prints the players starting moves
	//player_start(argc,argv,filestring);
	//calculates score after starting moves
	playerO.score = 0;
	playerX.score = 0;
	if(get_playertype(filestring) == 'O') {
		playerO.turn = true;//sets playerO turn to be true
	}
	else {
		playerX.turn = true;
		playerO.turn = false;
		
	}
	//for loop that terminates once board is full
	for(int i = 0; i < (xdimension*ydimension) ; i++){
		if(playerO.turn) {
		//iterates through the rows top to bottom
			for( int m = 1; m < ydimension - 1; m++) {
				//iterates through columns left to right
				for( int k = 3; k < (xdimension*2) - 2; k++) {
					//checks if there is an empty space
					if(playerO.board[m][k] == '.'){
						if(playerO.turn){
							//sets the position as players symbol
							playerO.board[m][k] = playerO.symbol;
							//sets the board as the same for other player
							playerX.board = playerO.board;
							//makes the players turn false
							playerO.turn = false;
							//makes other players turn true
							playerX.turn = true;
							//prints where player placed and prints the board
							fprintf(stdout,"Player %c placed at %d %d\n",playerO.symbol,m,k/2);
							print_stored_board(filestring,playerO.board);
					}
				}
			}
		}
	}
	if(playerX.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
			for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerX.board[k][g];
				}
			}
			playerX.board = player1_toprow(filestring,playerX,playerO);
			if(sumof_score(filestring,playerO.symbol,playerX.board) != 
			sumof_score(filestring,playerO.symbol,turnBoard)) {
				playerX.turn = false;
				playerO.turn = true;
				playerO.board = playerX.board;
			}
			else {
				playerX.turn = true;
				playerO.turn = false;
			}
	}
	if(playerX.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerX.board[k][g];
				}
			}
			playerX.board = player1_lastcolumn(filestring,playerX,playerO);
			if(sumof_score(filestring,playerO.symbol,playerX.board) != 
			sumof_score(filestring,playerO.symbol,turnBoard)) {
				playerX.turn = false;
				playerO.turn = true;
				playerO.board = playerX.board;
			}
			else {
				playerX.turn = true;
				playerO.turn = false;
			}
		}
		
		if(playerX.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerX.board[k][g];
				}
			}
			playerX.board = player1_bottomrow(filestring,playerX,playerO);
			if(sumof_score(filestring,playerO.symbol,playerX.board) != 
			sumof_score(filestring,playerO.symbol,turnBoard)) {
				playerX.turn = false;
				playerO.turn = true;
				playerO.board = playerX.board;
			}
			else {
				playerX.turn = true;
				playerO.turn = false;
			}
			
			
		}
		if(playerX.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerX.board[k][g];
				}
			}
			playerX.board = player1_firstcolumn(filestring,playerX,playerO);
			if(sumof_score(filestring,playerO.symbol,playerX.board) != 
			sumof_score(filestring,playerO.symbol,turnBoard)) {
				playerX.turn = false;
				playerO.turn = true;
				playerO.board = playerX.board;
			}
			else {
				playerX.turn = true;
				playerO.turn = false;
			}
			
			
		}
		
		if(playerX.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			playerX.board = player1_interior(filestring,playerX);
			playerO.board = playerX.board;
			playerX.turn = false;
			playerO.turn = true;
		}
	}
	playerO.score = sumof_score(filestring,playerO.symbol,playerO.board);
	playerX.score = sumof_score(filestring,playerX.symbol,playerX.board);
	print_winner(playerO,playerX);
}

/*
player O is now playe type 1
player X is now player type 0(same as playerX from player00_func)
*/
void player10_func(int argc, char** argv,char* filestring) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	struct Player playerO;
	struct Player playerX;
	char** turnBoard = malloc(sizeof(char*)*ydimension);
	for(int y = 0; y < ydimension;y++ ) {
		turnBoard[y] = malloc(sizeof(char)*xdimension*2);
	}
	playerO.symbol = 'O';
	playerX.symbol = 'X';
	playerO.board = file_board_array(filestring);
	playerX.board = playerO.board;
	if(get_playertype(filestring) == 'O') {
		playerO.turn = true;
	}
	else {
		playerX.turn = true;
		playerO.turn = false;
	}
	for(int q = 0; q < 30; q++) {
		if(playerO.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerO.board[k][g];
				}
			}
			playerO.board = player1_toprow(filestring,playerO,playerX);
			if(sumof_score(filestring,playerX.symbol,playerO.board) != 
			sumof_score(filestring,playerX.symbol,turnBoard)) {
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			else {
				playerO.turn = true;
				playerX.turn = false;
			}
			
		}
		
		if(playerO.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerO.board[k][g];
				}
			}
			playerO.board = player1_lastcolumn(filestring,playerO,playerX);
			if(sumof_score(filestring,playerX.symbol,playerO.board) != 
			sumof_score(filestring,playerX.symbol,turnBoard)) {
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			else {
				playerO.turn = true;
				playerX.turn = false;
			}
			
		
			
		}
		if(playerO.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerO.board[k][g];
				}
			}
			playerO.board = player1_bottomrow(filestring,playerO,playerX);
			if(sumof_score(filestring,playerX.symbol,playerO.board) != 
			sumof_score(filestring,playerX.symbol,turnBoard)) {
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			else {
				playerO.turn = true;
				playerX.turn = false;
			}
			
			

		}
		if(playerO.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerO.board[k][g];
				}
			}
			playerO.board = player1_firstcolumn(filestring,playerO,playerX);
			if(sumof_score(filestring,playerX.symbol,playerO.board) != 
			sumof_score(filestring,playerX.symbol,turnBoard)) {
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			else {
				playerO.turn = true;
				playerX.turn = false;
			}
		
			
		}
		if(playerO.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			playerO.board = player1_interior(filestring,playerO);
			playerO.turn = false;
			playerX.turn = true;
			playerX.board = playerO.board;
		}
		if(playerX.turn){
			//itterates from bottom to top
			for(int a = ydimension - 2; a > 0 ; a--) {
				//itterates from right to left
				for( int b = (xdimension*2)-3; b > 2 ; b--){
					if(playerX.board[a][b] == '.'){
						if(playerX.turn){
							playerX.board[a][b] = playerX.symbol;
							playerO.board = playerX.board;
							playerX.score += playerO.board[a][b - 1] - '0';
							playerX.turn = false;
							playerO.turn = true;
							fprintf(stdout,"Player %c placed at %d %d\n",playerX.symbol,a,b/2);
							print_stored_board(filestring,playerX.board);
						}
					}
				}
			}
		}
	}
	playerO.score = sumof_score(filestring,playerO.symbol,playerO.board);
	playerX.score = sumof_score(filestring,playerX.symbol,playerX.board);
	print_winner(playerO,playerX);
}
	
/**
*function that controls two human players
*/
void playerH_func(int argc,char** argv,char* filestring){
	//both the x and y dimensions of the board
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	//initiliases board, read from file
	char** board = file_board_array(filestring);
	// lines to store the input from stdin
	char* lineO = malloc(sizeof(char)*ydimension*xdimension);
	char* lineX = malloc(sizeof(char)*ydimension*xdimension);
	char* eoFX= malloc(sizeof(char)*xdimension*ydimension);//variables that check for EOF
	char* eoFO = malloc(sizeof(char)*xdimension*ydimension);
	char* saveO = malloc(sizeof(char)*xdimension*ydimension);
	char* saveX = malloc(sizeof(char)*xdimension*ydimension);
	//memset(lineO,'\0',xdimension*ydimension);
	//memset(lineX,'\0',xdimension*ydimension);
	// player types for the game
	struct Player playerO;
	struct Player playerX;
	playerO.symbol = 'O';
	playerX.symbol = 'X';
	// initilaies the players board
	playerO.board = board;
	playerX.board = playerO.board;
	// the R C values from stdin
	int row;
	int column;
	playerO.score = 0;
	playerX.score = 0;
	if(get_playertype(filestring) == 'O') {
			playerO.turn = true;
	}
	else {
		playerX.turn = true;
		playerO.turn = false;
	}
	//checks if both players are H type
	if((strcmp(argv[1],"H") == 0) && (strcmp(argv[2],"H") == 0) ){
		// for loop to run the game until a break
		for(int i = 0; i < ydimension*xdimension; i++) {
			//checks if players turn
			if(playerO.turn) {
				//if statment checks if it is a full board
				if(full_board(filestring,playerO.board)) {
					break;
				}
				fprintf(stdout,"%c:(R C)> ",playerO.symbol);
				//prompts the user to enter R C values and stores them in lineO
				eoFO = fgets(lineO,xdimension*ydimension,stdin);
				if(eoFO == NULL) {//checks if end of file is sent from player (control D)
					fprintf(stderr,"End of file\n");
					exit(5);
				}
				if(lineO[0] =='s'){//checks if player wants to save
					if(lineO[1] != '/') {//makes sure it is valid save 
						if(strlen(lineO) > 2) {//if the strlen is more than 2 ie not just a space after s
							for(int i = 1; i <strlen(lineO);i++) {//deep copy string lineO to saveO ignoring the 's'
								saveO[i-1] = lineO[i];
							}
							for(int a = 0; a< strlen(saveO);a++) {//goes through saveO and casts out all non printables
								if(isprint(saveO[a])==0) {
									saveO[a] = '\0';
								}
							}
							save_game(filestring,saveO,playerX,playerO);//saves the game with name being string in saveO
							if(feof(stdin)!=0) {
								fprintf(stderr,"End of file\n");
								exit(5);
							}
						} else {
							save_game(filestring,&lineO[1],playerX,playerO);//else saves with the name from lineO[1] onwards
						}
							
						
					}else {
							fprintf(stderr,"Save failed\n");// if invalid name ie if there is / save failed
					}
					
					
				}
				//checks for EOF after input put in
				if(feof(stdin)!=0) {
					fflush(stdin);//first control D flushes second sends EOF
					fprintf(stderr,"End of file\n");
					exit(5);
				}
				//convers the chars into ints
				if(check_spaces(lineO) > 1) {
					continue;
				}
				row = atoi(&lineO[0]);
				if(strlen(string_to_int(row)) == 1) {
					column = atoi(&lineO[2]);
				}
				else if(strlen(string_to_int(row)) == 2) {
					column = atoi(&lineO[3]);
				}
				else {
					column = atoi(&lineO[4]);
				}
					
				// checks if invalid entries where entered, reprompts player if it not
				if(!check_for_valid_entry(filestring,row,column,playerO.board)) {
					continue;
				}
				//all if's will check if its been placed on an edge,which then allows it to be controled as it should
				if(row == 0 && check_for_valid_edge(filestring,row,column,playerO.board)) {
					playerO.board = shift_down(filestring,playerO,row,column);
					print_stored_board(filestring,playerO.board);
					playerO.turn = false;
					playerX.turn = true;
					playerX.board = playerO.board;
				}
				if(row == ydimension-1 && check_for_valid_edge(filestring,row,column,playerO.board)) {
					playerO.board = shift_up(filestring,playerO,row,column);
					print_stored_board(filestring,playerO.board);
					playerO.turn = false;
					playerX.turn = true;
					playerX.board = playerO.board;
				}
				if(column == xdimension-1 && check_for_valid_edge(filestring,row,column,playerO.board)) {
					playerO.board = shift_right(filestring,playerO,row,column);
					print_stored_board(filestring,playerO.board);
					playerO.turn = false;
					playerX.turn = true;
					playerX.board = playerO.board;
				}
				if(column == 0 && check_for_valid_edge(filestring,row,column,playerO.board)) {
					playerO.board = shift_left(filestring,playerO,row,column);
					print_stored_board(filestring,playerO.board);
					playerO.turn = false;
					playerX.turn = true;
					playerO.board = playerX.board;
				}
				
				else if((row == 0 || row == ydimension-1|| column == 0 || column == xdimension -1)
					&&!check_for_valid_edge(filestring,row,column,playerO.board)) {
					continue;
				}
				else if (row > 0 && column > 0 && row!=ydimension-1 && column != xdimension-1){	
				// places the player at the designated area
				playerO.board[row][(column*2) + 1] = playerO.symbol;
				//adds to the players score
				playerO.score += playerO.board[row][column*2] - '0';
				//sets the player turn as false and other player as true
				playerO.turn = false;
				playerX.turn = true;
				//transfers the new board to other player
				playerX.board = playerO.board;
				//prints the board
				print_stored_board(filestring,playerO.board);
				}
			}
			//player X is the same as playerO
			if(playerX.turn) {
				if(full_board(filestring,playerX.board)) {
					break;
				}
				fprintf(stdout,"%c:(R C)> ",playerX.symbol);
				 eoFX = fgets(lineX,xdimension*ydimension,stdin);
				if(eoFX == '\0') {
					fprintf(stderr,"End of file\n");
					exit(5);
				}
				if(lineX[0] =='s'){
					if(lineX[1] != '/') {
						if(strlen(lineX) > 2) {
							for(int i = 1; i <strlen(lineX);i++) {
								saveX[i-1] = lineX[i];
							}
							for(int a = 0; a< strlen(saveX);a++) {
								if(isprint(saveX[a])==0) {
									saveX[a] = '\0';
								}
							}
							save_game(filestring,saveX,playerX,playerO);
							if(feof(stdin)!=0) {
								fprintf(stderr,"End of file\n");
								exit(5);
							}
						} else {
							save_game(filestring,&lineX[1],playerX,playerO);
						}
							
						
					}else {
							fprintf(stderr,"Save failed\n");
					}
					
					
				}
				if(feof(stdin)!=0) {
					fflush(stdin);
					fprintf(stderr,"End of file\n");
					exit(5);
				}
				//if row is muliple digits columns value starts higher	
				row = atoi(&lineX[0]);
				if(strlen(string_to_int(row)) == 1) {
					column = atoi(&lineX[2]);
				}
				else if(strlen(string_to_int(row)) == 2) {
					column = atoi(&lineX[3]);
				}
				else {
					column = atoi(&lineX[4]);
				}
				if(check_spaces(lineX) > 1) {
					playerX.turn = true;
					playerO.turn = false;
					continue;
				}
				if(!check_for_valid_entry(filestring,row,column,playerO.board)) {
					playerX.turn = true;
					playerO.turn = false;
					continue;
				}
			
				//all if's will check if its been placed on an edge,which then allows it to be controled as it should
				if(row == 0 && check_for_valid_edge(filestring,row,column,playerX.board)) {
					playerX.board = shift_down(filestring,playerX,row,column);
					print_stored_board(filestring,playerX.board);
					playerX.turn = false;
					playerO.turn = true;
					playerO.board = playerX.board;
				}
				if(row == ydimension-1 && check_for_valid_edge(filestring,row,column,playerX.board)) {
					playerX.board = shift_up(filestring,playerX,row,column);
					print_stored_board(filestring,playerX.board);
					playerX.turn = false;
					playerO.turn = true;
					playerO.board = playerX.board;
				}
				if(column == xdimension-1 && check_for_valid_edge(filestring,row,column,playerX.board)) {
					playerX.board = shift_right(filestring,playerX,row,column);
					print_stored_board(filestring,playerX.board);
					playerX.turn = false;
					playerO.turn = true;
					playerO.board = playerX.board;
				}
				if(column == 0 && check_for_valid_edge(filestring,row,column,playerX.board)) {
					playerX.board = shift_left(filestring,playerX,row,column);
					print_stored_board(filestring,playerX.board);
					playerX.turn = false;
					playerO.turn = true;
					playerO.board = playerX.board;
				}
				else if (row > 0 && column > 0 && row!=ydimension-1 && column != xdimension-1) {
				playerX.turn = false;
				playerO.turn = true;
				playerX.board[row][(column*2) +1 ] = playerX.symbol;
				playerX.score += playerX.board[row][column*2] - '0';
				playerO.board = playerX.board;
				print_stored_board(filestring,playerX.board);
				}
			}
		}
	}
	// once game func broken it prints the winner
	print_winner(playerO,playerX);
}

/*
* Function that controls player O as automated player type 0
*and Player X being a Human type player,same as player00 type O
*and player X from playerH
*/
void playerOH_func(int argc,char** argv,char* filestring) {
	  //both the x and y dimensions of the board
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	struct Player playerO;
	struct Player playerX;
	int row = 0;
	int column = 0;
	// line to store the input from stdin
	char* lineB = malloc(sizeof(char)*ydimension*xdimension);
	char* eoFX = malloc(sizeof(char)*ydimension*xdimension);
	char* saveX = malloc(sizeof(char)*ydimension*xdimension);
	playerO.board = file_board_array(filestring);
	playerX.board = playerO.board;
	playerO.symbol = 'O';
	playerX.symbol = 'X';
	if(get_playertype(filestring) == 'O') {
		playerO.turn = true;//sets playerO turn to be true
	}
	else {
		playerX.turn = true;
		playerO.turn = false;
	}
	for(int i = 0; i < xdimension*ydimension; i++) {
		if(playerO.turn) {
			//iterates through the rows top to bottom
			for( int m = 1; m < ydimension - 1; m++) {
			//iterates through columns left to right
				for( int k = 3; k < (xdimension*2) - 2; k++) {
				//checks if there is an empty space
					if(playerO.board[m][k] == '.'){
						if(playerO.turn){
							if(full_board(filestring,playerO.board)) {
								break;
							}
							//sets the position as players symbol
							playerO.board[m][k] = playerO.symbol;
							//sets the board as the same for other player
							playerX.board = playerO.board;
							//makes the players turn false
							playerO.turn = false;
							//makes other players turn true
							playerX.turn = true;
							//prints where player placed and prints the board
							fprintf(stdout,"Player %c placed at %d %d\n",playerO.symbol,m,k/2);
							print_stored_board(filestring,playerO.board);
						}
					}
				}
			}
		}
		if(playerX.turn) {
				if(full_board(filestring,playerX.board)) {
					break;
				}
				fprintf(stdout,"%c:(R C)> ",playerX.symbol);
				eoFX = fgets(lineB,ydimension*xdimension,stdin);
				if(eoFX == NULL) {
					fprintf(stderr,"End of file\n");
					exit(5);
				}
				if(lineB[0] =='s'){
					if(lineB[1] != '/') {
						if(strlen(lineB) > 2) {
							for(int i = 1; i <strlen(lineB);i++) {
								saveX[i-1] = lineB[i];
							}
							for(int a = 0; a< strlen(saveX);a++) {
								if(isprint(saveX[a])==0) {
									saveX[a] = '\0';
								}
							}
							save_game(filestring,saveX,playerX,playerO);
							if(feof(stdin)!=0) {
								fprintf(stderr,"End of file\n");
								exit(5);
							}
						} else {
							save_game(filestring,&lineB[1],playerX,playerO);
						}
							
						
					}else {
							fprintf(stderr,"Save failed\n");
					}
					
					
				}
				if(feof(stdin)!=0) {
					fflush(stdin);
					fprintf(stderr,"End of file\n");
					exit(5);
				}
				row = atoi(&lineB[0]);
				if(strlen(string_to_int(row)) == 1) {
					column = atoi(&lineB[2]);
				}
				else if(strlen(string_to_int(row)) == 2) {
					column = atoi(&lineB[3]);
				}
				else {
					column = atoi(&lineB[4]);
				}
				if(check_spaces(lineB) > 1) {
					playerX.turn = true;
					playerO.turn = false;
					continue;
				}
				if(!check_for_valid_entry(filestring,row,column,playerO.board)) {
					playerX.turn = true;
					playerO.turn = false;
					continue;
				}
				if(row == 0 && check_for_valid_edge(filestring,row,column,playerX.board)) {
					playerX.board = shift_down(filestring,playerX,row,column);
					print_stored_board(filestring,playerX.board);
					playerX.turn = false;
					playerO.turn = true;
					playerO.board = playerX.board;
				}
				if(row == ydimension-1 && check_for_valid_edge(filestring,row,column,playerX.board)) {
					playerX.board = shift_up(filestring,playerX,row,column);
					print_stored_board(filestring,playerX.board);
					playerX.turn = false;
					playerO.turn = true;
					playerO.board = playerX.board;
				}
				if(column == xdimension-1 && check_for_valid_edge(filestring,row,column,playerX.board)) {
					playerX.board = shift_right(filestring,playerX,row,column);
					print_stored_board(filestring,playerX.board);
					playerX.turn = false;
					playerO.turn = true;
					playerO.board = playerX.board;
				}
				if(column == 0 && check_for_valid_edge(filestring,row,column,playerX.board)) {
					playerX.board = shift_left(filestring,playerX,row,column);
					print_stored_board(filestring,playerX.board);
					playerX.turn = false;
					playerO.turn = true;
					playerO.board = playerX.board;
				}
				else if( row > 0 && column > 0 && row!=ydimension-1 && column != xdimension-1) {
				playerX.turn = false;
				playerO.turn = true;
				playerX.board[row][(column*2) +1 ] = playerX.symbol;
				playerO.board = playerX.board;
				print_stored_board(filestring,playerX.board);
			}
		
		
		}
	}
	playerO.score = sumof_score(filestring,playerO.symbol,playerO.board);
	playerX.score = sumof_score(filestring,playerX.symbol,playerX.board);
	print_winner(playerO,playerX);
	
}

/**
function that controls human and type 0 players, with player O being human
and player X being type 0 automated
*/
void playerHO_func(int argc,char** argv,char* filestring) {
	 //both the x and y dimensions of the board
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	struct Player playerO;
	struct Player playerX;
	int row = 0;
	int column = 0;
	// line to store the input from stdin
	char* lineO = malloc(sizeof(char)*ydimension*xdimension);
	char* eoFO = malloc(sizeof(char)*ydimension*xdimension);
	char* saveO = malloc(sizeof(char)*ydimension*xdimension);
	playerO.board = file_board_array(filestring);
	playerX.board = playerO.board;
	playerO.symbol = 'O';
	playerX.symbol = 'X';
	if(get_playertype(filestring) == 'O') {
		playerO.turn = true;//sets playerO turn to be true
	}
	else {
		playerX.turn = true;
		playerO.turn = false;
	}
	for(int i = 0; i  < xdimension*ydimension;i++) {
			//checks if players turn
		if(playerO.turn) {
			//if statment checks if it is a full board
			if(full_board(filestring,playerO.board)) {
				break;
			}
			fprintf(stdout,"%c:(R C)> ",playerO.symbol);
			//prompts the user to enter R C values and stores them in lineO
			eoFO = fgets(lineO,xdimension*ydimension,stdin);
			if(eoFO == NULL) {
					fprintf(stderr,"End of file\n");
					exit(5);
			}
			if(lineO[0] =='s'){
					if(lineO[1] != '/') {
						if(strlen(lineO) > 2) {
							for(int i = 1; i <strlen(lineO);i++) {
								saveO[i-1] = lineO[i];
							}
							for(int a = 0; a< strlen(saveO);a++) {
								if(isprint(saveO[a])==0) {
									saveO[a] = '\0';
								}
							}
							save_game(filestring,saveO,playerX,playerO);
							if(feof(stdin)!=0) {
								fprintf(stderr,"End of file\n");
								exit(5);
							}
						} else {
							save_game(filestring,&lineO[1],playerX,playerO);
						}
							
						
					}else {
							fprintf(stderr,"Save failed\n");
					}
					
					
				}
				if(feof(stdin)!=0) {
					fflush(stdin);
					fprintf(stderr,"End of file\n");
					exit(5);
				}
			//convers the chars into ints
			row = atoi(&lineO[0]);
			if(strlen(string_to_int(row)) == 1) {
				column = atoi(&lineO[2]);
			}
			else if(strlen(string_to_int(row)) == 2) {
				column = atoi(&lineO[3]);
			}
			else {
				column = atoi(&lineO[4]);
			}
			if(check_spaces(lineO) > 1) {
				continue;
			}
			// checks if invalid entries where entered
			if(!check_for_valid_entry(filestring,row,column,playerO.board)) {
				continue;
			}
					if(row == 0 && check_for_valid_edge(filestring,row,column,playerO.board)) {
					playerO.board = shift_down(filestring,playerO,row,column);
					print_stored_board(filestring,playerO.board);
					playerO.turn = false;
					playerX.turn = true;
					playerX.board = playerO.board;
				}
			if(row == ydimension-1 && check_for_valid_edge(filestring,row,column,playerO.board)) {
				playerO.board = shift_up(filestring,playerO,row,column);
				print_stored_board(filestring,playerO.board);
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			if(column == xdimension-1 && check_for_valid_edge(filestring,row,column,playerO.board)) {
				playerO.board = shift_right(filestring,playerO,row,column);
				print_stored_board(filestring,playerO.board);
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			if(column == 0 && check_for_valid_edge(filestring,row,column,playerO.board)) {
				playerO.board = shift_left(filestring,playerO,row,column);
				print_stored_board(filestring,playerO.board);
				playerO.turn = false;
				playerX.turn = true;
				playerO.board = playerX.board;
			}
			// places the player at the designated area
			else if( row > 0 && column > 0 && row!=ydimension-1 && column != xdimension-1) {
			playerO.board[row][(column*2) + 1] = playerO.symbol;
			//sets the player turn as false and other player as true
			playerO.turn = false;
			playerX.turn = true;
			//transfers the new board to other player
			playerX.board = playerO.board;
			//prints the board
			print_stored_board(filestring,playerO.board);
			}
		}
			if(playerX.turn){
				//itterates from bottom to top
				for(int a = ydimension - 2; a > 0 ; a--) {
					//itterates from right to left
					for( int b = (xdimension*2)-3; b > 2 ; b--){
						if(playerX.board[a][b] == '.'){
							if(playerX.turn){
								playerX.board[a][b] = playerX.symbol;
								playerO.board = playerX.board;
								playerX.turn = false;
								playerO.turn = true;
								fprintf(stdout,"Player %c placed at %d %d\n",playerX.symbol,a,b/2);
								print_stored_board(filestring,playerX.board);
							}
						}
					}
				}
			}
	}
	playerO.score = sumof_score(filestring,playerO.symbol,playerO.board);
	playerX.score = sumof_score(filestring,playerX.symbol,playerX.board);
	print_winner(playerO,playerX);
}

			
/*
controls type O being player 1 (same as type O from player1_func)
*and X as being human (same as playerH_func)
*/
void player1H_func(int argc,char** argv,char* filestring) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	int row = 0;
	int column = 0;
	// line to store the input from stdin
	char* lineB = malloc(sizeof(char)*ydimension*xdimension);
	char* eoFX = malloc(sizeof(char)*ydimension*xdimension);
	char* saveX = malloc(sizeof(char)*ydimension*xdimension);
	struct Player playerO;
	struct Player playerX;
	char** turnBoard = malloc(sizeof(char*)*ydimension);
	for(int y = 0; y < ydimension;y++ ) {
		turnBoard[y] = malloc(sizeof(char)*xdimension*2);
	}
	playerO.symbol = 'O';
	playerX.symbol = 'X';
	playerO.board = file_board_array(filestring);
	playerX.board = playerO.board;
	if(get_playertype(filestring) == 'O') {
		playerO.turn = true;
	}
	else {
		playerX.turn = true;
		playerO.turn = false;
	}
	for(int q = 0; q < 30; q++) {
		if(playerO.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerO.board[k][g];
				}
				
			}
			//turnBoard = realloc(turnBoard,sizeof(turnBoard));
			playerO.board = player1_toprow(filestring,playerO,playerX);
			if(sumof_score(filestring,playerX.symbol,playerO.board) != 
			sumof_score(filestring,playerX.symbol,turnBoard)) {
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			else {
				playerO.turn = true;
				playerX.turn = false;
			}
			
		}
		if(playerO.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerO.board[k][g];
				}
				
			}
			//turnBoard = realloc(turnBoard,sizeof(turnBoard));
			playerO.board = player1_lastcolumn(filestring,playerO,playerX);
			if(sumof_score(filestring,playerX.symbol,playerO.board) != 
			sumof_score(filestring,playerX.symbol,turnBoard)) {
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			else {
				playerO.turn = true;
				playerX.turn = false;
			}
			
		
			
		}
		if(playerO.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerO.board[k][g];
				}
			}
			//turnBoard = realloc(turnBoard,sizeof(turnBoard));
			playerO.board = player1_bottomrow(filestring,playerO,playerX);
			if(sumof_score(filestring,playerX.symbol,playerO.board) != 
			sumof_score(filestring,playerX.symbol,turnBoard)) {
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			else {
				playerO.turn = true;
				playerX.turn = false;
			}
			
			

		}
		if(playerO.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerO.board[k][g];
				}
			}
			//turnBoard = realloc(turnBoard,sizeof(turnBoard));
			playerO.board = player1_firstcolumn(filestring,playerO,playerX);
			if(sumof_score(filestring,playerX.symbol,playerO.board) != 
			sumof_score(filestring,playerX.symbol,turnBoard)) {
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			else {
				playerO.turn = true;
				playerX.turn = false;
			}
		
			
		}
		if(playerO.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			playerO.board = player1_interior(filestring,playerO);
			playerO.turn = false;
			playerX.turn = true;
			playerX.board = playerO.board;
		}
		if(playerX.turn) {
				if(full_board(filestring,playerX.board)) {
					break;
				}
				fprintf(stdout,"%c:(R C)> ",playerX.symbol);
				eoFX = fgets(lineB,ydimension*xdimension,stdin);
				if(eoFX == NULL ) {
					fprintf(stderr,"End of file\n");
					exit(5);
				}
				if(lineB[0] =='s'){
					if(lineB[1] != '/') {
						if(strlen(lineB) > 2) {
							for(int i = 1; i <strlen(lineB);i++) {
								saveX[i-1] = lineB[i];
							}
							for(int a = 0; a< strlen(saveX);a++) {
								if(isprint(saveX[a])==0) {
									saveX[a] = '\0';
								}
							}
							save_game(filestring,saveX,playerX,playerO);
							if(feof(stdin)!=0) {
								fprintf(stderr,"End of file\n");
								exit(5);
							}
						} else {
							save_game(filestring,&lineB[1],playerX,playerO);
						}
							
						
					}else {
							fprintf(stderr,"Save failed\n");
					}
					
					
				}
				if(feof(stdin)!=0) {
					fflush(stdin);
					fprintf(stderr,"End of file\n");
					exit(5);
				}
				
				row = atoi(&lineB[0]);
				if(strlen(string_to_int(row)) == 1) {
					column = atoi(&lineB[2]);
				
				}
				else if(strlen(string_to_int(row)) == 2) {
					column = atoi(&lineB[3]);
				}
				else {
					column = atoi(&lineB[4]);
				}
				if(check_spaces(lineB) > 1) {
					playerX.turn = true;
					playerO.turn = false;
					continue;
				}
				if(!check_for_valid_entry(filestring,row,column,playerO.board)) {
					playerX.turn = true;
					playerO.turn = false;
					continue;
				}
				
				if(row == 0 && check_for_valid_edge(filestring,row,column,playerX.board)) {
					playerX.board = shift_down(filestring,playerX,row,column);
					print_stored_board(filestring,playerX.board);
					playerX.turn = false;
					playerO.turn = true;
					playerO.board = playerX.board;
				}
				if(row == ydimension-1 && check_for_valid_edge(filestring,row,column,playerX.board)) {
					playerX.board = shift_up(filestring,playerX,row,column);
					print_stored_board(filestring,playerX.board);
					playerX.turn = false;
					playerO.turn = true;
					playerO.board = playerX.board;
				}
				if(column == xdimension-1 && check_for_valid_edge(filestring,row,column,playerX.board)) {
					playerX.board = shift_right(filestring,playerX,row,column);
					print_stored_board(filestring,playerX.board);
					playerX.turn = false;
					playerO.turn = true;
					playerO.board = playerX.board;
				}
				if(column == 0 && check_for_valid_edge(filestring,row,column,playerX.board)) {
					playerX.board = shift_left(filestring,playerX,row,column);
					print_stored_board(filestring,playerX.board);
					playerX.turn = false;
					playerO.turn = true;
					playerO.board = playerX.board;
				}
				else if( row > 0 && column > 0 && row!=ydimension-1 && column != xdimension-1) {
				playerX.turn = false;
				playerO.turn = true;
				playerX.board[row][(column*2) +1 ] = playerX.symbol;
				playerO.board = playerX.board;
				print_stored_board(filestring,playerX.board);
			}
		
		
		}
	}
	playerO.score = sumof_score(filestring,playerO.symbol,playerO.board);
	playerX.score = sumof_score(filestring,playerX.symbol,playerX.board);
	print_winner(playerO,playerX);
	free(lineB);
	free(eoFX);
}

/*
controls type X being player 1 (same as type X from player1_func)
*and O as being human (same as playerH_func)
*/
void playerH1_func(int argc,char** argv,char* filestring) {
	int ydimension = dimension_function(filestring)[0];
	int xdimension = dimension_function(filestring)[1];
	int row = 0;
	int column = 0;
	// line to store the input from stdin
	char* lineO = malloc(sizeof(char)*ydimension*xdimension);
	char* eoFO = malloc(sizeof(char)*ydimension*xdimension);
	char* saveO = malloc(sizeof(char)*ydimension*xdimension);
	struct Player playerO;
	struct Player playerX;
	char** turnBoard = malloc(sizeof(char*)*ydimension);
	for(int y = 0; y < ydimension;y++ ) {
		turnBoard[y] = malloc(sizeof(char)*xdimension*2);
	}
	playerO.symbol = 'O';
	playerX.symbol = 'X';
	playerO.board = file_board_array(filestring);
	playerX.board = playerO.board;
	if(get_playertype(filestring) == 'O') {
		playerO.turn = true;
	}
	else {
		playerX.turn = true;
		playerO.turn = false;
	}
	for(int i = 0; i  < xdimension*ydimension;i++) {
			//checks if players turn
		if(playerO.turn) {
			//if statment checks if it is a full board
			if(full_board(filestring,playerO.board)) {
				break;
			}
			fprintf(stdout,"%c:(R C)> ",playerO.symbol);
			//prompts the user to enter R C values and stores them in lineO
			eoFO = fgets(lineO,xdimension*ydimension,stdin);
			if(eoFO == NULL) {
					fprintf(stderr,"End of file\n");
					exit(5);
			}
			if(lineO[0] =='s'){
					if(lineO[1] != '/') {
						if(strlen(lineO) > 2) {
							for(int i = 1; i <strlen(lineO);i++) {
								saveO[i-1] = lineO[i];
							}
							for(int a = 0; a< strlen(saveO);a++) {
								if(isprint(saveO[a])==0) {
									saveO[a] = '\0';
								}
							}
							save_game(filestring,saveO,playerX,playerO);
							if(feof(stdin)!=0) {
								fprintf(stderr,"End of file\n");
								exit(5);
							}
						} else {
							save_game(filestring,&lineO[1],playerX,playerO);
						}
							
						
					}else {
							fprintf(stderr,"Save failed\n");
					}
					
					
				}
				if(feof(stdin)!=0) {
					fflush(stdin);
					fprintf(stderr,"End of file\n");
					exit(5);
				}
			//convers the chars into ints
			row = atoi(&lineO[0]);
			if(strlen(string_to_int(row)) == 1) {
				column = atoi(&lineO[2]);
			}
			else if(strlen(string_to_int(row)) == 2) {
				column = atoi(&lineO[3]);
			}
			else {
				column = atoi(&lineO[4]);
			}
			if(check_spaces(lineO) > 1) {
				continue;
			}
			// checks if invalid entries where entered
			if(!check_for_valid_entry(filestring,row,column,playerO.board)) {
				continue;
			}
					if(row == 0 && check_for_valid_edge(filestring,row,column,playerO.board)) {
					playerO.board = shift_down(filestring,playerO,row,column);
					print_stored_board(filestring,playerO.board);
					playerO.turn = false;
					playerX.turn = true;
					playerX.board = playerO.board;
				}
			if(row == ydimension-1 && check_for_valid_edge(filestring,row,column,playerO.board)) {
				playerO.board = shift_up(filestring,playerO,row,column);
				print_stored_board(filestring,playerO.board);
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			if(column == xdimension-1 && check_for_valid_edge(filestring,row,column,playerO.board)) {
				playerO.board = shift_right(filestring,playerO,row,column);
				print_stored_board(filestring,playerO.board);
				playerO.turn = false;
				playerX.turn = true;
				playerX.board = playerO.board;
			}
			if(column == 0 && check_for_valid_edge(filestring,row,column,playerO.board)) {
				playerO.board = shift_left(filestring,playerO,row,column);
				print_stored_board(filestring,playerO.board);
				playerO.turn = false;
				playerX.turn = true;
				playerO.board = playerX.board;
			}
			// places the player at the designated area
			else if( row > 0 && column > 0 && row!=ydimension-1 && column != xdimension-1) {
			playerO.board[row][(column*2) + 1] = playerO.symbol;
			//sets the player turn as false and other player as true
			playerO.turn = false;
			playerX.turn = true;
			//transfers the new board to other player
			playerX.board = playerO.board;
			//prints the board
			print_stored_board(filestring,playerO.board);
			}
		}
		if(playerX.turn) {
			if(full_board(filestring,playerO.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
			for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerX.board[k][g];
				}
			}
			playerX.board = player1_toprow(filestring,playerX,playerO);
			if(sumof_score(filestring,playerO.symbol,playerX.board) != 
			sumof_score(filestring,playerO.symbol,turnBoard)) {
				playerX.turn = false;
				playerO.turn = true;
				playerO.board = playerX.board;
			}
			else {
				playerX.turn = true;
				playerO.turn = false;
			}
		
		}
		
		
		if(playerX.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerX.board[k][g];
				}
			}
			playerX.board = player1_lastcolumn(filestring,playerX,playerO);
			if(sumof_score(filestring,playerO.symbol,playerX.board) != 
			sumof_score(filestring,playerO.symbol,turnBoard)) {
				playerX.turn = false;
				playerO.turn = true;
				playerO.board = playerX.board;
			}
			else {
				playerX.turn = true;
				playerO.turn = false;
			}
		}
		
		if(playerX.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerX.board[k][g];
				}
			}
			playerX.board = player1_bottomrow(filestring,playerX,playerO);
			if(sumof_score(filestring,playerO.symbol,playerX.board) != 
			sumof_score(filestring,playerO.symbol,turnBoard)) {
				playerX.turn = false;
				playerO.turn = true;
				playerO.board = playerX.board;
			}
			else {
				playerX.turn = true;
				playerO.turn = false;
			}
			
			
		}
		if(playerX.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			for(int k = 0; k < ydimension; k++) {
				for(int g = 0; g < xdimension*2;g++) {
					turnBoard[k][g] = playerX.board[k][g];
				}
			}
			playerX.board = player1_firstcolumn(filestring,playerX,playerO);
			if(sumof_score(filestring,playerO.symbol,playerX.board) != 
			sumof_score(filestring,playerO.symbol,turnBoard)) {
				playerX.turn = false;
				playerO.turn = true;
				playerO.board = playerX.board;
			}
			else {
				playerX.turn = true;
				playerO.turn = false;
			}
			
			
		}
		
		if(playerX.turn) {
			if(full_board(filestring,playerX.board)) {
				break;
			}
			playerX.board = player1_interior(filestring,playerX);
			playerO.board = playerX.board;
			playerX.turn = false;
			playerO.turn = true;
		}
		
	}
	playerO.score = sumof_score(filestring,playerO.symbol,playerO.board);
	playerX.score = sumof_score(filestring,playerX.symbol,playerX.board);
	print_winner(playerO,playerX);
	
}
	
	


/**function to run the functions that controls the overall game
*throws the arg errors, no file to load error,invalid file contents error
*checks the input and runs the apprporiate function based on it
*/
void run_game(int argc,char** argv){
	arg_error_check(argc, argv);
	dimension_function(argv[3]);
	check_playertype(argv[3]);
	check_for_full(argv[3]);
	print_board(argv[3]);
	if(strcmp(argv[1],"0") == 0 && strcmp(argv[2],"0") == 0){
		player00_func(argc,argv,argv[3]);
	}
	if((strcmp(argv[1],"0") == 0) && (strcmp(argv[2],"H") == 0)){
		playerOH_func(argc,argv,argv[3]);
	}
	if((strcmp(argv[1],"H") == 0) && (strcmp(argv[2],"0") == 0)){
		playerHO_func(argc,argv,argv[3]);
	}
	if((strcmp(argv[1],"H") == 0) && (strcmp(argv[2],"H") == 0)){
		playerH_func(argc,argv,argv[3]);
	}
	
	if((strcmp(argv[1],"1") == 0) && (strcmp(argv[2],"1") == 0)){
		player1_func(argc,argv,argv[3]);
	}
	if((strcmp(argv[1],"0") == 0) && (strcmp(argv[2],"1") == 0)){
		player01_func(argc,argv,argv[3]);
	}
	if((strcmp(argv[1],"1") == 0) && (strcmp(argv[2],"0") == 0)){
		player10_func(argc,argv,argv[3]);
	}
	if((strcmp(argv[1],"1") == 0) && (strcmp(argv[2],"H") == 0)){
		player1H_func(argc,argv,argv[3]);
	}
	if((strcmp(argv[1],"H") == 0) && (strcmp(argv[2],"1") == 0)){
		playerH1_func(argc,argv,argv[3]);
	}

}

	
	
	


int main(int argc, char** argv) {
	run_game(argc,argv);
}	