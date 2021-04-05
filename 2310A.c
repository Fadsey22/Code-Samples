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
struct Player {
	int pastposy;
	int pastposx;
	int positionx;
	int positiony;
	int playerid;
	char** map;
	int Acounter;
	int Bcounter;
	int Ccounter;
	int Dcounter;
	int Ecounter;
	char* playertype;
	char* hand;
	int money;
	int points;
	int V1counter;
	int V2counter;
	int pointgain;
	int moneygain;
	bool turn;
};

char* pathstring;

void check_path(char* path) {
	if(isdigit(path[0]) == 0) {
		fprintf(stderr,"Invalid path\n");
		exit(3);
	}
	if(path == NULL) {
		fprintf(stderr,"Invalid path\n");
		exit(4);
	}
	for( int a = 0; a < strlen(path); a++) {
		if(path[a] == ';' && path[a+1]!= ':') {
			fprintf(stderr,"Invalid path\n");
			exit(4);
		}
	}
	for(int i = 0; i < strlen(path); i++) {
		if(path[i] == 'M' && (path[i+1] != 'o' || isdigit(path[i+2]) == 0)) {
			fprintf(stderr,"Invalid path\n");
			exit(4);
		}
		if(path[i] == 'R' && (path[i+1] != 'i' || isdigit(path[i+2]) == 0)) {
			fprintf(stderr,"Invalid path\n");
			exit(4);
		}
		if(path[i] == 'D' && (path[i+1] != 'o' || isdigit(path[i+2]) == 0)) {
			fprintf(stderr,"Invalid path\n");
			exit(4);
		}
		if(path[i] == 'V' && path[i+1] == '1' && isdigit(path[i+2]) == 0) {
			fprintf(stderr,"Invalid path\n");
			exit(4);
		}
		if(path[i] == 'V' && path[i+1] == '2' && isdigit(path[i+2]) == 0) {
			fprintf(stderr,"Invalid path\n");
			exit(4);
		}
		if(path[i] == 'V' && (path[i+1]!= '1' && path[i+1]!= '2' )) {
			fprintf(stderr,"Invalid path\n");
			exit(4);
		}
		if((path[i] == 'M' || path[i] == 'D' || path[i] == 'V' 
		|| path[i] == 'R') && (path[i+2] - '0') < 1) {
			fprintf(stderr,"Invalid path\n");
			exit(4);
		}
		if((path[i] == 'M' || path[i] == 'D' || path[i] == 'V' 
		|| path[i] == 'R') && (isdigit(path[i+3]) != 0)) {
			fprintf(stderr,"Invalid path\n");
			exit(4);
		}
		if(path[i] == ':' && (path[i+1] != '-' && path[i+2] != '-')) {
			fprintf(stderr,"Invalid path\n");
			exit(4);
		}
	}
}

void get_path(void) {
	pathstring = calloc(300,sizeof(char));
	char* path = calloc(300,sizeof(char));
	fgets(path,300,stdin); 
	strcpy(pathstring,path);
	check_path(pathstring);
}

char* end_string(char* string) {
	for(int i = 0; i < strlen(string); i++) {
		if(isprint(string[i]) == 0) {
			string[i] = '\0';
		}
	}
	return string;
}


int num_of_sites(void) {
	char* path = malloc(sizeof(char)*100);
	stpcpy(path,pathstring);
	path = (char*)realloc(path,sizeof(path));
	char* pathsize = malloc(sizeof(char)*15);
	int pathlen = 0;
	for(int i = 0; i < strlen(path); i++) {
		if(isdigit(path[i]) != 0) {
			pathsize[i] = path[i];
		}else{
			break;
		}
	}
	
	pathsize = (char*)realloc(pathsize,sizeof(pathsize));
	pathlen = atoi(pathsize);
	return pathlen;
}
int sizeof_array(char** argv,int* arr) {
	int counter = 1;
	int numofplayers = atoi(argv[1]);
	for(int i = 0; i < numofplayers + 1; i++) {
		if(arr[i] > 0 && arr[i] <= numofplayers + 1) {
			counter++;
		}
	}
	return counter;
}

void print_map(char** argv,char** map) {
	int counter = 0;
	int numofplayers = atoi(argv[1]);
	int numofsites = num_of_sites();
	int* validrows = malloc(sizeof(int)*(numofplayers+1));
	for(int i = 0; i < (numofplayers + 1);i++) {
		for(int j = 0; j < (numofsites*3); j++) {
			if(map[i][j] != ' ') {
				validrows[counter] = i;
				counter++;
				break;
			}
		}
		
	}
	int len = sizeof_array(argv,validrows);
	for(int g = 0; g < len; g++) {
		for(int a = 0; a < (numofsites*3); a++) {
		fprintf(stderr,"%c",map[validrows[g]][a]);
		}
		fprintf(stderr,"\n");
	}
}

char** path_to_array(char** argv) {
	int numofplayers = atoi(argv[1]);
	int mapindex = 0;
	int numofsites = num_of_sites();
	char** map = malloc(sizeof(char*)*numofplayers+1);
	for(int i = 0; i < numofplayers+1; i++) {
		map[i] = malloc(sizeof(char)*(numofsites*3)-1);
	}
	for(int i = 0; i < strlen(pathstring);i++) {
		switch(pathstring[i]) {
			case 'M':
				map[0][mapindex] = 'M';
				map[0][mapindex+1] = 'o';
				map[0][mapindex+2] = ' ';
				mapindex+=3;
				break;
			case 'V':
				if(pathstring[i+1] == '1') {
					map[0][mapindex] = 'V';
					map[0][mapindex+1] = '1';
					map[0][mapindex+2] = ' ';
					mapindex+=3;
					break;
				}
				if(pathstring[i+1] == '2') {
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
	if(map[0][(numofsites*3) - 2] !=':'){
		fprintf(stderr,"Invalid path\n");
		exit(4);
	}
	return map;
}

int* limits_array(char** argv) {
	int numofsites = num_of_sites();
	int numofplayers = atoi(argv[1]);
	char* path = malloc(sizeof(char)*300);
	path = pathstring;
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
				limits[limitindex] = numofplayers;
				limitindex++;
				i++;
				break;
		}
	}
	return limits;
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
	player->hand = end_string(player->hand);
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



void update_playerpos(char** argv,struct Player* player) {
	int numofplayers = atoi(argv[1]);
	int numofsites = num_of_sites();
	for(int i = 0; i < numofplayers + 1; i++) {
		for(int j = 0; j < numofsites*3 ; j++) {
			if(player->map[i][j] == player->playerid +'0') {
				player->positiony = i;
				player->positionx = j;
			}
		}
	}
}
				
void initialise_game(int argc, char** argv,struct Player* players, char** map) {
	int numofplayers = atoi(argv[1]);
	int numofsites = num_of_sites();
	int positiony = numofplayers;
	int j = numofplayers - 1;
	map = path_to_array(argv);
	for(int i = 0; i < numofplayers; i++) {
		players[i].playerid = i;
		players[i].positionx = 0;
		players[i].playertype = argv[0];
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
	for(int i = 0 ; i < numofplayers;i++) {
		players[i].positiony = positiony;
		positiony--;
	}
	for(int i = 1; i < numofplayers+1;i++) {
		map[i][0] = players[j].playerid + '0';
		j--;
	}
	players[numofplayers-1].turn = true;
	print_map(argv,map);
}

int search_for_DO(char** argv,int playerxpos,char** map) {
	int numofsites = num_of_sites();
	int numofplayers = atoi(argv[1]);
	int* limits = limits_array(argv);
	for(int i = playerxpos + 3; i < (numofsites*3); i+=3) {
		for(int j = 1; j < numofplayers + 1; j++) {
			if(map[0][i] == 'D' && map[j][i] == ' '&& j<= limits[i/3]) {
				return i;
			}
			else {
				continue;
			}
		}
	}
	return -1;
}

int check_for_barrier(int playerposx, int Place, char* str) {
	for(int i = playerposx + 3; i < Place; i+=3) {
		if(str[i] == ':') {
			return i;
		}
	}
	return -1;
}

int search_for_V1V2(char** argv,int playerxpos,char** map) {
	int numofsites = num_of_sites();
	int numofplayers = atoi(argv[1]);
	int* limits = limits_array(argv);
	for(int i = playerxpos + 3; i < (numofsites*3); i+=3) {
		for(int j = 1; j < numofplayers + 1; j++) {
			if(map[0][i] == 'V' && map[j][i] == ' '&& j<= limits[i/3]) {
				return i;
			}
			else {
				continue;
			}
		}
	}
	return -1;
}

int search_for_barrier(int playerxpos,char* str) {
	int numofsites = num_of_sites();
	for(int i = playerxpos + 3; i < (numofsites*3); i+=3) {
		if(str[i] == ':') {
			return i;
		}
	}
	return -1;
}	
char* store_printable(char* str) {
	char* printable = calloc(20,sizeof(char));
	int printindex = 0;
	for(int i = 0; i < strlen(str); i++) {
		if(isprint(str[i])!=0) {
			printable[printindex] = str[i];
			printindex++;
		}
	}
	printable = realloc(printable,sizeof(printable));
	return printable;
}

void get_move(char** argv,struct Player* player) {
	bool stop = false;
	int* limits = limits_array(argv);
	int DOPlace = 0;
	int BarPlaceDO = 0 ;
	int BarPlaceV = 0;
	int Bar = 0;
	int VPlace = 0;
	bool Mostop = false;
	char* prompt = calloc(20,sizeof(char));
	int numofplayers = atoi(argv[1]);
	fgets(prompt,20,stdin);
	prompt = store_printable(prompt);
	if(feof(stdin)!=0) {
		fprintf(stderr,"Communications error\n");
		exit(6);
	}
	if(strcmp(prompt,"YT") == 0) {
		DOPlace = search_for_DO(argv,player->positionx,player->map);
		if(DOPlace != -1) {
			BarPlaceDO = check_for_barrier(player->positionx,DOPlace,player->map[0]);
		}
		VPlace = search_for_V1V2(argv,player->positionx,player->map);
		if(VPlace != -1) {
			BarPlaceV = check_for_barrier(player->positionx,VPlace,player->map[0]);
		}
		Bar = search_for_barrier(player->positionx,player->map[0]);
		
		if(DOPlace !=-1 && player->money > 0 && BarPlaceDO == -1) {
			for( int j = 1; j < numofplayers+1 && !stop;j++) {
				if(j <= limits[DOPlace/3]  && player->map[j][DOPlace] == ' ') {
					player->positionx = DOPlace;
					player-> positiony = j;
					fprintf(stdout,"DO%d\n",DOPlace/3);
					fflush(stdout);
					stop = true;
				}
				else {
					continue;
				}
			}
		}
		if(player->map[0][player->positionx+3] == 'M' && !Mostop) {
			for( int c = 1; c < numofplayers+1 && !stop;c++) {
				if(c <= limits[(player->positionx+3)/3]) {
					if(player->map[c][player->positionx+3] == ' ') {
						player->positionx+=3;
						player-> positiony = c;
						fprintf(stdout,"DO%d\n",player->positionx/3);
						fflush(stdout);
						stop = true;
					}
					else {
						continue;
					}
				}
				else {
					Mostop = true;
				}
			}
		}
		if(VPlace != -1 && BarPlaceV == -1) {
			for( int j = 1; j < numofplayers+1 && !stop;j++) {
				if(j <= limits[VPlace/3]  && player->map[j][VPlace] == ' ') {
					player->positionx = VPlace;
					player-> positiony = j;
					fprintf(stdout,"DO%d\n",VPlace/3);
					fflush(stdout);
					stop = true;
				}
				else {
					continue;
				}
			}
		}
		if(Bar != -1 && !stop)  {
			for(int h = 1 ; h < numofplayers + 1 && !stop; h++) {
				if(player->map[h][Bar] == ' ') {
				player->positionx = Bar;
				player-> positiony = h;
				fprintf(stdout,"DO%d\n",Bar/3);
				fflush(stdout);
				stop = true;
				}
				else {
				continue;
				}
			}
		}
	}
	else if(strcmp(prompt,"EARLY") == 0) {
		fprintf(stderr,"Early game over\n");
		exit(5);
	}
	else {
		fprintf(stderr,"Communications error\n");
		exit(6);
	}
}

void get_moveother(char** argv,struct Player* player) {
	bool stop = false;
	int* limits = limits_array(argv);
	int DOPlace = 0;
	int BarPlaceDO = 0 ;
	int BarPlaceV = 0;
	int Bar = 0;
	int VPlace = 0;
	bool Mostop = false;
	int numofplayers = atoi(argv[1]);
	DOPlace = search_for_DO(argv,player->positionx,player->map);
	if(DOPlace != -1) {
		BarPlaceDO = check_for_barrier(player->positionx,DOPlace,player->map[0]);
	}
	VPlace = search_for_V1V2(argv,player->positionx,player->map);
	if(VPlace != -1) {
		BarPlaceV = check_for_barrier(player->positionx,VPlace,player->map[0]);
	}
	Bar = search_for_barrier(player->positionx,player->map[0]);
	
	if(DOPlace !=-1 && player->money > 0 && BarPlaceDO == -1) {
		for( int j = 1; j < numofplayers+1 && !stop;j++) {
			if(j <= limits[DOPlace/3]  && player->map[j][DOPlace] == ' ') {
				player->positionx = DOPlace;
				player-> positiony = j;
				fprintf(stdout,"DO%d\n",DOPlace/3);
				fflush(stdout);
				stop = true;
			}
			else {
				continue;
			}
		}
	}
	if(player->map[0][player->positionx+3] == 'M' && !Mostop) {
		for( int c = 1; c < numofplayers+1 && !stop;c++) {
			if(c <= limits[(player->positionx+3)/3]) {
				if(player->map[c][player->positionx+3] == ' ') {
					player->positionx+=3;
					player-> positiony = c;
					fprintf(stdout,"DO%d\n",player->positionx/3);
					fflush(stdout);
					stop = true;
				}
				else {
					continue;
				}
			}
			else {
				Mostop = true;
			}
		}
	}
	if(VPlace != -1 && BarPlaceV == -1) {
		for( int j = 1; j < numofplayers+1 && !stop;j++) {
			if(j <= limits[VPlace/3]  && player->map[j][VPlace] == ' ') {
				player->positionx = VPlace;
				player-> positiony = j;
				fprintf(stdout,"DO%d\n",VPlace/3);
				fflush(stdout);
				stop = true;
			}
			else {
				continue;
			}
		}
	}
	if(Bar != -1 && !stop)  {
		for(int h = 1 ; h < numofplayers + 1 && !stop; h++) {
			if(player->map[h][Bar] == ' ') {
			player->positionx = Bar;
			player-> positiony = h;
			fprintf(stdout,"DO%d\n",Bar/3);
			fflush(stdout);
			stop = true;
			}
			else {
			continue;
			}
		}
	}
}

void handle_site(struct Player* player) {
	switch(player->map[0][player->positionx]) {
		case 'V':
			if(player->map[0][player->positionx+1] == '1') {
				player->pointgain = 0;
				player->V1counter+=1;
				break;
			}
			if(player->map[0][player->positionx+1] == '2') {
				player->pointgain = 0;
				player->V2counter+=1;
				break;
			}
	}
}

int player_turn(char** argv,struct Player* players) {
	int numofplayers = atoi(argv[1]);
	int numofsites = num_of_sites();
	int playerid = 0;
	int posy = 0;
	int highestposx = numofsites*3;
	for(int i = 0; i < numofplayers; i++) {
		update_playerpos(argv,&players[i]);
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

int break_HAP(char* Hap) {
	char* broken = malloc(sizeof(char)*10);
	int Hapnumber = 0;
	int brokencounter = 0;
	for(int i = 0; i < strlen(Hap); i++) {
		if(Hap[i] != ',') {
			broken[brokencounter] = Hap[i];
			brokencounter++;
		}
		else {
			break;
		}
	}
	broken = realloc(broken,sizeof(broken));
	if(strcmp(broken,"0") == 0 ){
		Hapnumber = atoi(broken);
		return Hapnumber;
	}
	if(atoi(broken) == 0) {
		fprintf(stderr,"Communications error\n");
		exit(6);
	}
	Hapnumber = atoi(broken);
	return Hapnumber;
}
		

void interpret_HAP(char** argv,struct Player* player) {
	update_playerpos(argv,player);
	int numofsites = num_of_sites();
	int numofplayers = atoi(argv[1]);
	char* Hap = malloc(sizeof(char)*30);
	player->hand = malloc(sizeof(char)*50);
	int counter   = 0;
	fgets(Hap,30,stdin);
	Hap = store_printable(Hap);
	if(feof(stdin)!=0) {
		fprintf(stderr,"Communications error\n");
		exit(6);
	}
	if(strcmp(Hap,"EARLY") == 0) {
		fprintf(stderr,"Early game over\n");
		exit(5);
	}
	if(strcmp(Hap,"YT") == 0) {
		get_moveother(argv,player);
		fprintf(stderr,"Communications error\n");
		exit(6);
	}
	char card;
	Hap = end_string(Hap);
	for(int b = 0; b < strlen(Hap); b++) {
		if(Hap[b] == ',') {
			counter++;
			if(counter == 1) {
				if(break_HAP(&Hap[b+1]) > numofsites || break_HAP(&Hap[b+1]) < 0 ) {
					fprintf(stderr,"Communications error\n");
					exit(6);
				}
				player->positionx = 3*(break_HAP(&Hap[b+1]));
			}
			if(counter == 2) {
				
				player->points+= break_HAP(&Hap[b+1]);
			}
			if( counter == 3) {
				player->money+=break_HAP(&Hap[b+1]);
			}
		}
	}
	if(Hap[3] - '0' > numofplayers-1 || Hap[3] - '0' < 0 ) {
		fprintf(stderr,"Communications error\n");
		exit(6);
	}
	if(counter !=4 ) {
		fprintf(stderr,"Communications error\n");
		exit(6);
	}
		
	for(int u = 1; u < numofplayers + 1; u++) {
		if(player->map[u][player->positionx] == ' ') {
			player->positiony = u;
			break;
		}
	}
	card = Hap[strlen(Hap)-1];
	if(isdigit(card) == 0) {
		fprintf(stderr,"Communications error\n");
		exit(6);
	}
	switch(card) {
		case '0':
			break;
		case '1':
			player->Acounter+=1;
			strcat(player->hand,"A");
			break;
		case '2':
			player->Bcounter+=1;
			strcat(player->hand,"B");
			break;
		case '3':
			player->Ccounter+=1;
			strcat(player->hand,"C");
			break;
		case '4':
			player->Dcounter+=1;
			strcat(player->hand,"D");
			break;
		case '5':
			player->Ecounter+=1;
			strcat(player->hand,"E");
			break;
		default:
			fprintf(stderr,"Communications error\n");
			exit(6);
	}
}

void print_output(struct Player player) {
	fprintf(stderr,"Player %d Money=%d V1=%d V2=%d Points=%d A=%d B=%d C=%d D=%d E=%d\n"
	,player.playerid,player.money,player.V1counter,player.V2counter,player.points,
	player.Acounter,player.Bcounter,player.Ccounter,player.Dcounter,player.Ecounter);
}

void summate_V1V2(struct Player* player) {
	player->points += player->V1counter;
	player->points  += player->V2counter;
}
void sumate_scores(char** argv, struct Player* players) {
	fprintf(stderr,"Scores: ");
	int numofplayers = atoi(argv[1]);
	for(int i = 0; i < numofplayers - 1; i++) {
		summate_V1V2(&players[i]);
		hand_split(&players[i]);
		fprintf(stderr,"%d,",players[i].points);
	}
	summate_V1V2(&players[numofplayers-1]);
	hand_split(&players[numofplayers-1]);
	fprintf(stderr,"%d",players[numofplayers-1].points);
	fprintf(stderr,"\n");
}

bool check_for_endgame(char** argv,struct Player* players) {
	int numofplayers = atoi(argv[1]);
	int numofsites = num_of_sites();
	int counter = 0;
	for(int i = 0; i < numofplayers; i++) {
		update_playerpos(argv,&players[i]);
		if(players[i].positionx == (numofsites*3)-3) {
			counter++;
		}
	}
	if(counter == numofplayers) {
		return true;
	}
	return false;
}
		
void game_end() {
	char* endgame = calloc(5,sizeof(char));
	fgets(endgame,5,stdin);
}
		
		
void game_func(int argc, char** argv) {
	int playerindex = 0;
	int numofplayers = atoi(argv[1]);
	int numofsites = num_of_sites();
	struct Player* players = malloc(sizeof(struct Player)* numofplayers);
	char** map = malloc(sizeof(char*)*numofplayers+1);
	for(int i = 0; i < numofplayers+1; i++) {
		map[i] = malloc(sizeof(char)*(numofsites*3)-1);
	}
	initialise_game(argc,argv,players,map);
	players[numofplayers-1].turn = true;
	while(!check_for_endgame(argv,players)) {
		for(int i = numofplayers - 1; i >= 0; i--) {
			playerindex = player_turn(argv,players);
			players[playerindex].turn = true;
			i = playerindex;
			if(check_for_endgame(argv,players)) {
				break;
			}
			if(players[i].turn) {
				update_playerpos(argv,&players[i]);
				players[i].pastposy = players[i].positiony;
				players[i].pastposx = players[i].positionx;
				if(i == atoi(argv[2])) {
					get_move(argv,&players[i]);
				}
				interpret_HAP(argv,&players[i]);
				players[i].map[players[i].positiony][players[i].positionx] = players[i].playerid + '0';
				players[i].map[players[i].pastposy][players[i].pastposx] = ' ';
				handle_site(&players[i]);
				print_output(players[i]);
				print_map(argv,players[i].map);
				players[i].turn = false;
			}
		}
	}
	game_end();
	sumate_scores(argv,players);
}
	
void arg_check(int argc, char** argv) {
	if(argc != 3) {
		fprintf(stderr,"Usage: player pcount ID\n");
		exit(1);
	}
	if(atoi(argv[1]) < 1) {
		fprintf(stderr,"Invalid player count\n");
		exit(2);
	}
	if(atoi(argv[2]) < 0 || atoi(argv[2]) > atoi(argv[1])) {
		fprintf(stderr,"Invalid ID\n");
		exit(3);
	}
	if(strcmp(argv[2],"0") != 0 && atoi(argv[2]) == 0) {
		fprintf(stderr,"Invalid ID\n");
		exit(3);
	}
}



void control_player(int argc,char** argv) {
	arg_check(argc,argv);
	fprintf(stdout,"^");
	fflush(stdout);
	get_path();
	if(num_of_sites() < 2) {
		fprintf(stderr,"Invalid path\n");
		exit(4);
	}
		
	game_func(argc,argv);
	
}
	
int main(int argc, char** argv) {
	control_player(argc,argv);
}
	