#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include<string.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

#define READ 0
#define WRITE 1
struct Player {
	int numofsites;
	char* deck;
	int pastposy;
	int pastposx;
	int positionx;
	int positiony;
	int playerid;
	pid_t playerpid;
	FILE* writefile;
	FILE* readfile;
	char** map;
	int Acounter;
	int Bcounter;
	int Ccounter;
	int Dcounter;
	int Ecounter;
	char* playertype;
	char* hand;
	int cardgain;
	int money;
	int points;
	int V1counter;
	int V2counter;
	int pointgain;
	int moneygain;
	bool turn;
};
int Ricounter ;

char* read_path(char* sitefile) {
	FILE* file;
	file = fopen(sitefile,"r");
	char* path = (char*)calloc(300,sizeof(char));
	int counter = 0;
	while(fgets(path,300,file)){//reads file till EOF
		if(counter == 0 ){
		}
	}
	return path;
}

void check_path(char* pathfile) {
	char* path = read_path(pathfile);
	if(isdigit(path[0]) == 0) {
		fprintf(stderr,"Error reading path\n");
		exit(3);
	}
	if(path == NULL) {
		fprintf(stderr,"Error reading path\n");
		exit(3);
	}
	for( int a = 0; a < strlen(path); a++) {
		if(path[a] == ';' && path[a+1]!= ':') {
			fprintf(stderr,"Error reading path\n");
			exit(3);
		}
	}
	for(int i = 0; i < strlen(path); i++) {
		if(path[i] == 'M' && (path[i+1] != 'o' || isdigit(path[i+2]) == 0)) {
			fprintf(stderr,"Error reading path\n");
			exit(3);
		}
		if(path[i] == 'R' && (path[i+1] != 'i' || isdigit(path[i+2]) == 0)) {
			fprintf(stderr,"Error reading path\n");
			exit(3);
		}
		if(path[i] == 'D' && (path[i+1] != 'o' || isdigit(path[i+2]) == 0)) {
			fprintf(stderr,"Error reading path\n");
			exit(3);
		}
		if(path[i] == 'V' && path[i+1] == '1' && isdigit(path[i+2]) == 0) {
			fprintf(stderr,"Error reading path\n");
			exit(3);
		}
		if(path[i] == 'V' && path[i+1] == '2' && isdigit(path[i+2]) == 0) {
			fprintf(stderr,"Error reading path\n");
			exit(3);
		}
		if(path[i] == 'V' && (path[i+1]!= '1' && path[i+1]!= '2' )) {
			fprintf(stderr,"Error reading path\n");
			exit(3);
		}
		if((path[i] == 'M' || path[i] == 'D' || path[i] == 'V' 
		|| path[i] == 'R') && (path[i+2] - '0') < 1) {
			fprintf(stderr,"Error reading path\n");
			exit(3);
		}
		if((path[i] == 'M' || path[i] == 'D' || path[i] == 'V' 
		|| path[i] == 'R') && (isdigit(path[i+3]) != 0)) {
			fprintf(stderr,"Error reading path\n");
			exit(3);
		}
		if(path[i] == ':' && (path[i+1] != '-' && path[i+2] != '-')) {
			fprintf(stderr,"Error reading path\n");
			exit(3);
		}
	}
}

int num_of_sites(char* sitefile) {
	char* path = malloc(sizeof(char)*100);
	path = calloc(300,sizeof(char));
	path = read_path(sitefile);
	char* pathsize = malloc(sizeof(char)*15);
	int pathlen = 0;
	for(int i = 0; i < strlen(path); i++) {
		if(isdigit(path[i]) != 0) {
			pathsize[i] = path[i];
		}else{
			break;
		}
	}
	pathlen = atoi(pathsize);
	return pathlen;
}
	

		
char* read_deck(char* filestring) {
	FILE* file;
	file = fopen(filestring,"r");
	char* deck = calloc(200,sizeof(char));
	int counter = 0;
	while(fgets(deck,200,file)){//reads file till EOF
		counter++;
		if(counter == 0 ){
			deck = (char*)realloc(deck,sizeof(deck));//realocates memory that is not used 
		}
	}
	return deck;
}



char* int_to_string(int number) {
	char* string = malloc(sizeof(char)*3);
	sprintf(string,"%d",number);
	return string;
}


int get_deck_length(char* filestring) {
	char* deck = malloc(sizeof(char)*100);
	deck = read_deck(filestring);
	deck = (char*)realloc(deck,sizeof(deck));
	char* decksize = malloc(sizeof(char)*15);
	int decklen = 0;
	for(int i = 0; i < strlen(deck); i++) {
		if(isdigit(deck[i]) != 0) {
			decksize[i] = deck[i];
		}else{
			break;
		}
	}
	
	decksize = (char*)realloc(decksize,sizeof(decksize));
	decklen = atoi(decksize);
	return decklen;
}

void check_deck(char* filestring) {
	char* deck = read_deck(filestring);
	if(isdigit(deck[0]) == 0) {
		fprintf(stderr,"Error reading deck\n");
		exit(2);
	}
	int decklen = get_deck_length(filestring);
	if(decklen < 10) {
		for(int i = 1; i <= decklen;i++) {
			if(deck[i] != 'A' && deck[i] != 'B' && deck[i] != 'C'
			&& deck[i] != 'D' && deck[i] != 'E') {
				fprintf(stderr,"Error reading deck\n");
				exit(2);
			}
		}
	}
	if(decklen >= 10 && decklen < 100) {
		for(int i = 2; i <= decklen; i++) {
			if(deck[i] != 'A' && deck[i] != 'B' && deck[i] != 'C'
			&& deck[i] != 'D' && deck[i] != 'E') {
				fprintf(stderr,"Error reading deck\n");
				exit(2);
			}
		}
	}
	if(decklen >= 100) {
		for(int i = 3; i <= decklen ; i++) {
			if(deck[i] != 'A' && deck[i] != 'B' && deck[i] != 'C'
			&& deck[i] != 'D' && deck[i] != 'E') {
				fprintf(stderr,"Error reading deck1\n");
				exit(2);
			}
		}
	}
}

char* deck_array( char* filestring) {
	int decklength = get_deck_length(filestring);
	char* deck_array = malloc(sizeof(char)*decklength);
	char* deck_string = malloc(sizeof(char)*(decklength+20));
	deck_string = read_deck(filestring);
	deck_string = realloc(deck_string,sizeof(deck_string));
	int deckindex = 0;
	for(int i = 0; i < strlen(deck_string); i++) {
		if(isdigit(deck_string[i]) == 0 ) {
			deck_array[deckindex]= deck_string[i];
			deckindex++;
		}
	}
	return deck_array;
}

int* limits_array(char** argv,struct Player* player) {
	int numofsites = player->numofsites;
	char* path = read_path(argv[2]);
	int* limits = malloc(sizeof(int)*numofsites);
	int limitindex = 0;
	for(int i = 1; i < strlen(path);i++) {
		switch(path[i]) {
			case 'M':
				limits[limitindex] = path[i+2] - '0';
				limitindex++;
				break;
			case 'V':
				limits[limitindex] = path[i+2] - '0';
				limitindex++;
				break;
			case 'D':
				limits[limitindex] = path[i+2] - '0';
				limitindex++;
				break;
			case 'R':
				limits[limitindex] = path[i+2] - '0';
				limitindex++;
				break;
			case ':':
				limits[limitindex] = 1000;
				limitindex++;
				i++;
				break;
		}
	}
	return limits;
}


char** path_array(int argc,char* sitefile) {
	char* path = calloc(300,sizeof(char));
	int mapindex = 0;
	path = read_path(sitefile);
	int numofsites = num_of_sites(sitefile);
	char** map = malloc(sizeof(char*)*(argc-3)+1);
	for(int i = 0; i < (argc-3)+1; i++) {
		map[i] = malloc(sizeof(char)*(numofsites*3)-1);
	}
	for(int i = 0; i < strlen(path);i++) {
		switch(path[i]) {
			case 'M':
				map[0][mapindex] = 'M';
				map[0][mapindex+1] = 'o';
				map[0][mapindex+2] = ' ';
				mapindex+=3;
				break;
			case 'V':
				if(path[i+1] == '1') {
					map[0][mapindex] = 'V';
					map[0][mapindex+1] = '1';
					map[0][mapindex+2] = ' ';
					mapindex+=3;
					break;
				}
				if(path[i+1] == '2') {
					map[0][mapindex] = 'V';
					map[0][mapindex+1] = '2';
					map[0][mapindex+2] = ' ';
					mapindex+=3;
					break;
				}
				
			case 'D':
				map[0][mapindex] = 'D';
				map[0][mapindex+1] = 'o';
				map[0][mapindex+2] = ' ';
				mapindex+=3;
				break;
			case 'R':
				map[0][mapindex] = 'R';
				map[0][mapindex+1] = 'i';
				map[0][mapindex+2] = ' ';
				mapindex+=3;
				break;
			case ':':
				map[0][mapindex] = ':';
				map[0][mapindex+1] = ':';
				map[0][mapindex+2] = ' ';
				mapindex+=3;
				i++;
				break;
		}
	}
	return map;
}

int sizeof_array(int argc,int* arr) {
	int counter = 1;
	int numofplayers = argc -3 ;
	for(int i = 0; i < numofplayers + 1; i++) {
		if(arr[i] > 0 && arr[i] <= numofplayers + 1) {
			counter++;
		}
	}
	return counter;
}
void print_map(int argc,char* sitefile,char** map) {
	int numofplayers = argc -3;
	int counter = 0;
	int numofsites = num_of_sites(sitefile);
	int* validrows = calloc((numofplayers +1),sizeof(int));
	for(int i = 0; i < (numofplayers + 1);i++) {
		for(int j = 0; j < (numofsites*3); j++) {
			if(map[i][j] != ' ') {
				validrows[counter] = i;
				counter++;
				break;
			}
		}
		
	}
	int len = sizeof_array(argc,validrows);
	for(int g = 0; g < len; g++) {
		for(int a = 0; a < (numofsites*3); a++) {
		fprintf(stdout,"%c",map[validrows[g]][a]);
		}
		fprintf(stdout,"\n");
	}
}

pid_t send_path_to_player(char* sitefile,char* type,char* numofplayers,char* playerid,struct Player* player) {
	char* path = read_path(sitefile);
	pid_t childpid;
	int playertodealer[2];
	int dealertoplayer[2];
	int exec = 0;
	int supress[2];
	int pipeval = 0;
	int pipeval2 = 0;
	if(fopen(type,"r") == NULL) {
		fprintf(stderr,"Error starting process\n");
		exit(4);
	}
	pipeval = pipe(playertodealer);
	pipeval2 = pipe(dealertoplayer);
	pipe(supress);
    if(pipeval == -1 || pipeval2 == -1){ 
      fprintf(stderr,"Error starting process\n");
      exit(4);
    }
	switch(childpid = fork()){ 
        case -1: 
			fprintf(stderr,"Error starting process\n");
			exit(4);
		case 0:
			close(dealertoplayer[WRITE]);
			close(playertodealer[READ]);
			dup2(playertodealer[WRITE],STDOUT_FILENO);
			close(playertodealer[WRITE]);
			dup2(dealertoplayer[READ],STDIN_FILENO);
			close(dealertoplayer[READ]);
			dup2(supress[WRITE],STDERR_FILENO);
			close(supress[WRITE]);
			dup2(supress[READ],STDERR_FILENO);
			close(supress[READ]);
			exec = execlp(type,&type[2],numofplayers,playerid,NULL);
		default:
			close(dealertoplayer[READ]);
			close(playertodealer[WRITE]);
			player->readfile = fdopen(playertodealer[READ],"r");
			player->writefile = fdopen(dealertoplayer[WRITE],"w");
			if(fgetc(player->readfile) == '^'); {
				fprintf(player->writefile,"%s",path);
				fflush(player->writefile);
			}
	}
	if(exec == -1) {
		fprintf(stderr,"Error starting process\n");
		exit(4);
	}
		
	return childpid;
}

bool check_for_barrier(int playerposx, int Place, char* str) {
	for(int i = playerposx + 3; i < Place; i+=3) {
		if(str[i] == ':') {
			return true;
		}
	}
	return false;
}

void prompt_player(struct Player* player) {
	fprintf(player->writefile,"YT\n");
	fflush(player->writefile);
}

void send_HAP(int argc, struct Player* players,struct Player* player) {
	int numofplayers = argc - 3;
	for(int i = 0; i < numofplayers ; i++) {
		fprintf(players[i].writefile,"Hap%d,%d,%d,%d,%d\n",player->playerid,
		player->positionx/3,player->pointgain,player->moneygain,player->cardgain);
		fflush(players[i].writefile);
	}
}

void end_game(int argc,struct Player* players) {
	int numofplayers = argc - 3;
	for(int i = 0; i < numofplayers;i++) {
		fprintf(players[i].writefile,"DONE\n");
		fflush(players[i].writefile);
		fclose(players[i].readfile);
		fclose(players[i].writefile);
	}
}

void end_early(int argc, struct Player* players) {
	int numofplayers = argc - 3;
	for(int i = 0; i < numofplayers;i++) {
		fprintf(players[i].writefile,"EARLY\n");
		fflush(players[i].writefile);
		fclose(players[i].readfile);
		fclose(players[i].writefile);
	}
}

void get_move(int argc,char** argv,struct Player* player,struct Player* players) {
	bool stop = false;
	int* limits = limits_array(argv,player);
	int moveindex = 0;
	int numofplayers = argc - 3;
	char* move = calloc(10,sizeof(char));
	fgets(move,10,player->readfile);
	moveindex = atoi(&move[2]);
	if(moveindex*3 <= player->positionx) {
		end_early(argc,players);
		fprintf(stderr,"Communications error\n");
		exit(5);
	}
	if(check_for_barrier(player->positionx,moveindex*3,player->map[0])) {
		end_early(argc,players);
		fprintf(stderr,"Communications error\n");
		exit(5);
	}
	player->positionx = (moveindex*3);
	for(int j = 1; j < numofplayers+1 && !stop;j++) {
		if(player->map[j][player->positionx] == ' ') {
			player->positiony = j;
			stop = true;
		}
	}
	if(player->positiony > limits[player->positionx/3]) {
		end_early(argc,players);
		fprintf(stderr,"Communications error\n");
		exit(5);
	}
	
}
	
int num_of_players(int argc) {
	int numofplayers = argc -3;
	return numofplayers;
}



void update_playerpos(int argc,char** argv,struct Player* player) {
	int numofplayers = argc - 3;
	int numofsites = player->numofsites;
	for(int i = 0; i < numofplayers + 1; i++) {
		for(int j = 0; j < numofsites*3 ; j++) {
			if(player->map[i][j] == player->playerid +'0') {
				player->positiony = i;
				player->positionx = j;
			}
		}
	}
}

int player_turn(int argc,char** argv,struct Player* players) {
	int numofplayers = argc - 3;
	int numofsites = players[0].numofsites;
	int playerid = 0;
	int posy = 0;
	int highestposx = numofsites*3;
	for(int i = 0; i < numofplayers; i++) {
		update_playerpos(argc,argv,&players[i]);
	}
	for(int i = 0; i < numofplayers;i++) {
		if(players[i].positionx < highestposx) {
			highestposx = players[i].positionx;
			posy = players[i].positiony;
			playerid = i;
		}
		else if(players[i].positionx == highestposx) {
			if(players[i].positiony > posy) {
				posy = players[i].positiony;
				playerid = i;
			}
		}
	}
	return playerid;
}

void initialise_game(int argc , char** argv,char** map,struct Player* players) {
	int numofsites = num_of_sites(argv[2]);
	int numofplayers = argc - 3;
	int positiony = numofplayers;
	char* deck = deck_array(argv[1]);
	char* numberofplayers = int_to_string(numofplayers);
	map = path_array(argc,argv[2]);
	for(int i = 0; i < numofplayers; i++) {
		players[i].deck = deck;
		players[i].numofsites = numofsites;
		players[i].playerid = i;
		players[i].playertype = argv[i+3];
		players[i].positionx = 0;
		players[i].map = map;
		players[i].money = 7;
		players[i].points = 0;
		players[i].V1counter = 0;
		players[i].V2counter = 0;
		players[i].turn = false;
	}
	for(int i = 1; i < numofplayers+1;i++) {
		for(int j = 0; j < (numofsites*3)+1;j++) {
			map[i][j] = ' ';
		}
	}
	int j = numofplayers - 1;
	for(int i = 1; i < numofplayers+1;i++) {
		map[i][0] = players[j].playerid + '0';
		j--;
	}
	for(int i = 0 ; i < numofplayers;i++) {
		players[i].positiony = positiony;
		positiony--;
	}
	for(int i = 0; i < numofplayers; i++) {
		char* playerID = int_to_string(players[i].playerid);
		players[i].playerpid = send_path_to_player(argv[2],players[i].playertype,numberofplayers,playerID,&players[i]);	
	}	
	print_map(argc,argv[2],map);
}

void handle_site(char** argv,struct Player* player) {
	int decklen =  get_deck_length(argv[1]);
	if(Ricounter >= decklen) {
		Ricounter = 0;
	}
	switch(player->map[0][player->positionx]) {
		case 'M':
			player->money += 3;
			player->pointgain = 0;
			player->moneygain = 3;
			player->cardgain = 0;
			break;
		case 'V':
			if(player->map[0][player->positionx+1] == '1') {
				player->pointgain = 0;
				player->V1counter+=1;
				player->cardgain = 0;
				player->moneygain = 0;
				break;
			}
			if(player->map[0][player->positionx+1] == '2') {
				player->pointgain = 0;
				player->V2counter += 1;
				player->cardgain = 0;
				player->moneygain = 0;
				break;
			}
		case 'D':
			player->points+=(player->money/2);
			player->pointgain =(player->money/2);
			player->moneygain -= (player->money);
			player->money = 0;
			player->cardgain = 0;
			break;
		case 'R':
			if(player->deck[Ricounter] == 'A') {
				player->cardgain = 1;
				player->pointgain = 0;
				player->moneygain = 0;
				player->Acounter++;
			}
			if(player->deck[Ricounter] == 'B') {
				player->cardgain = 2;
				player->pointgain = 0;
				player->moneygain = 0;
				player->Bcounter++;
			}
			if(player->deck[Ricounter] == 'C') {
				player->cardgain = 3;
				player->pointgain = 0;
				player->moneygain = 0;
				player->Ccounter++;
			}
			if(player->deck[Ricounter] == 'D') {
				player->cardgain = 4;
				player->pointgain = 0;
				player->moneygain = 0;
				player->Dcounter++;
			}
			if(player->deck[Ricounter] == 'E') {
				player->cardgain = 5;
				player->pointgain = 0;
				player->moneygain = 0;
				player->Ecounter++;
			}
			Ricounter++;
			break;
		default:
			player->money += 0;
			player->cardgain = 0;
			player->pointgain = 0;
			player->moneygain = 0;
			
	}
}

void print_output(struct Player player) {
	fprintf(stdout,"Player %d Money=%d V1=%d V2=%d Points=%d A=%d B=%d C=%d D=%d E=%d\n"
	,player.playerid,player.money,player.V1counter,player.V2counter,player.points,
	player.Acounter,player.Bcounter,player.Ccounter,player.Dcounter,player.Ecounter);
}

int max_hand(struct Player* player) {
	int maxnums[5];
	int max = 0;
	maxnums[0] = player->Acounter;
	maxnums[1] = player->Bcounter;
	maxnums[2] = player->Ccounter;
	maxnums[3] = player->Dcounter;
	maxnums[4] = player->Ecounter;
	for(int i =0; i < 5; i++) {
		if(maxnums[i] > max) {
			max = maxnums[i];
		}
	}
	return max;
}

int sumate_hands(char* str) {
	int spacecounter = 0;
	for(int i = 0; i < 5; i++) {
		if(str[i] == ' ') {
			spacecounter++;
		}
	}
	switch(spacecounter) {
		case 0 :
			return 10;
			break;
		case 1 :
			return 7;
			break;
		case 2 :
			return 5;
			break;
		case 3 :
			return 3;
			break;
		case 4:
			return 1;
			break;
	}
	return 0;
}

void hand_split(struct Player* player) {
	int maxhand = max_hand(player);
	char** sets = malloc(sizeof(char*)*maxhand);
	for(int i = 0; i < maxhand; i++) {
		sets[i] = malloc(sizeof(char)*5);
	}
	for(int a = 0; a < maxhand; a++) {
		for(int b = 0; b < 5; b++) {
			sets[a][b] = ' ';
		}
	}
	for(int i = 0; i < player->Acounter; i++) {
		sets[i][0] = 'A';
	}
	for(int i = 0; i < player->Bcounter; i++) {
		sets[i][1] = 'B';
	}
	for(int i = 0; i < player->Ccounter; i++) {
		sets[i][2] = 'C';
	}
	for(int i = 0; i < player->Dcounter; i++) {
		sets[i][3] = 'D';
	}
	for(int i = 0; i < player->Ecounter; i++) {
		sets[i][4] = 'E';
	}
	
	for(int k = 0; k < maxhand; k++) {
		player->points += sumate_hands(sets[k]);
	}
}
void summate_V1V2(struct Player* player) {
	player->points += player->V1counter;
	player->points  += player->V2counter;
}

void sumate_scores(int argc, struct Player* players) {
	fprintf(stdout,"Scores: ");
	int numofplayers = argc - 3;
	for(int i = 0; i < numofplayers - 1; i++) {
		summate_V1V2(&players[i]);
		hand_split(&players[i]);
		fprintf(stdout,"%d,",players[i].points);
	}
	summate_V1V2(&players[numofplayers-1]);
	hand_split(&players[numofplayers-1]);
	fprintf(stdout,"%d",players[numofplayers-1].points);
	fprintf(stdout,"\n");
	fflush(stdout);
}

bool check_for_endgame(int argc,char** argv,struct Player* players) {
	int numofplayers = argc -3;
	int numofsites = players[0].numofsites;
	int counter = 0;
	for(int i = 0; i < numofplayers; i++) {
		update_playerpos(argc,argv,&players[i]);
		if(players[i].positionx == (numofsites*3)-3) {
			counter++;
		}
	}
	if(counter == numofplayers) {
		return true;
	}
	return false;
}




void game_func(int argc, char** argv) {
	int playerindex = 0;
	int numofplayers = argc - 3;
	int numofsites = num_of_sites(argv[2]);
	struct Player* players = malloc(sizeof(struct Player)*numofplayers);
	char** map = calloc((argc-3)+1,sizeof(char*));
	for(int i = 0; i < numofplayers+1; i++) {
		map[i] = calloc((numofsites*3) -1,sizeof(char));
	}
	initialise_game(argc,argv,map,players);
	for(int a = 0; a < numofplayers*numofsites;a++)	{
		for(int i = numofplayers - 1; i >= 0; i--) {
			playerindex = player_turn(argc,argv,players);
			players[playerindex].turn = true;
			i = playerindex;
			if(check_for_endgame(argc,argv,players)) {
				break;
			}
			if(players[i].turn) {
				update_playerpos(argc,argv,&players[i]);
				players[i].pastposy = players[i].positiony;
				players[i].pastposx = players[i].positionx;
				prompt_player(&players[i]);
				get_move(argc,argv,&players[i],players);
				handle_site(argv,&players[i]);
				send_HAP(argc,players,&players[i]);
				players[i].map[players[i].positiony][players[i].positionx] = players[i].playerid + '0';
				players[i].map[players[i].pastposy][players[i].pastposx] = ' ';
				print_output(players[i]);
				print_map(argc,argv[2],players[i].map);
			}
		}
		if(check_for_endgame(argc,argv,players)) {
			break;
		}
		
	}
	end_game(argc,players);
	sumate_scores(argc,players);
	
}



void error_check(int argc , char** argv) {
	if(argc < 4) {
		fprintf(stderr,"Usage: 2310dealer deck path p1 {p2}\n");
		exit(1);
	}
	
	if(fopen(argv[1],"r") == NULL) {
		fprintf(stderr, "Error reading deck\n");
		exit(2);
	}
	check_deck(argv[1]);
	check_path(argv[2]);
	if(fopen(argv[2],"r") == NULL) {
		fprintf(stderr, "Error reading path\n");
		exit(3);
	}
	
	if(get_deck_length(argv[1]) < 4) {
		fprintf(stderr, "Error reading deck\n");
		exit(2);
	}
	
		
}

void control_game(int argc, char** argv) {
	error_check(argc, argv);
	game_func(argc,argv);
}
	
int main(int argc, char** argv) {
	control_game(argc,argv);
}