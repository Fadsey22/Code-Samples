#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
struct Connections {
	int portNumber;
	int fileDescriptor;
};
struct Airport {
	char* AirportID;
	char* portnumber;
};
struct Connections connections[50];
struct Airport airports[50];
int airportcounter = 0;
int sockets = 0;
int change  = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char* read_message(FILE* file) {
	char *message = malloc(80);
    int next, pos;
    pos = 0;
	while (true) {
        next = fgetc(file);
		if(feof(file)!=0) {
			fclose(file);
			return NULL;
		}
        if (next == EOF && pos == 0) {
            free(message);
            return NULL;
        }
        if (next == EOF || next == '\n') {
            message[pos] = '\0';
            break;
        }
        message[pos++] = (char) next;
    }
    return message;
}

char* int_to_string(int number) {
	char* string = malloc(sizeof(char)*20);
	sprintf(string,"%d",number);
	return string;
}


int open_socket( void) {
	int portnumber;
    struct addrinfo* addr = 0;
    struct addrinfo port;
    memset(&port, 0, sizeof(struct addrinfo));
    port.ai_family = AF_INET;        // IPv6  for generic could use AF_UNSPEC
    port.ai_socktype = SOCK_STREAM;
    port.ai_flags = AI_PASSIVE;  // Because we want to bind with it    
    getaddrinfo("localhost", 0, &port, &addr);
    int serv = socket(AF_INET, SOCK_STREAM, 0); // 0 == use default protocol
    bind(serv, (struct sockaddr*)addr->ai_addr, sizeof(struct sockaddr));
    struct sockaddr_in ad;
    memset(&ad, 0, sizeof(struct sockaddr_in));
    socklen_t len=sizeof(struct sockaddr_in);
    getsockname(serv, (struct sockaddr*)&ad, &len);
	for(int i = 0; i < 20; i++) {
		connections[sockets].portNumber = (int) ntohs(ad.sin_port);
		portnumber = connections[sockets].portNumber;
	}
    fprintf(stdout,"%d\n",portnumber);
	fflush(stdout);
    listen(serv, 10);
    return serv;
}

int connect_socket(const char *port) {
    struct addrinfo* addr = 0;
    struct addrinfo addrhints;
    memset(&addrhints, 0, sizeof(struct addrinfo));
    addrhints.ai_family=AF_INET;        // IPv6  for generic could use AF_UNSPEC
    addrhints.ai_socktype=SOCK_STREAM;
    getaddrinfo("localhost", port, &addrhints, &addr);
    int fd=socket(AF_INET, SOCK_STREAM, 0); // 0 == use default protocol
    connect(fd, (struct sockaddr*)addr->ai_addr, sizeof(struct sockaddr));
    return fd;    
}
int find_char(char* message,char chr) {
	for(int i = 1; i <strlen(message); i++) {
		if(message[i] == chr) {
			return i;
		}
	}
	return -1;
}

char** store_lexogrphical(char** names) {
	names = calloc(airportcounter,sizeof(char*));
	char* temporary = calloc(200,sizeof(char));
	for(int i = 0; i < airportcounter; i++) {
		names[i] = calloc(strlen(airports[i].AirportID),sizeof(char));
	}
	for(int j = 0; j < airportcounter; j++) {
		strcpy(names[j],airports[j].AirportID);
	}
	if(airportcounter > 1) {
		for(int a = 0; a < airportcounter; ++a) {
			for(int b = a + 1; b < airportcounter; ++b) {
				if(strcmp(names[a],names[b]) > 0) {
					strcpy(temporary,names[a]);
					strcpy(names[a],names[b]);
					strcpy(names[b],temporary);
				}
			}
		}
	}
	return names;
}

bool check_for_invalid_chars(char chr) {
	if(chr == '\r' || chr == '\n' || chr == ':') {
		return true;
	
	}
	return false;
}

char* find_portnumber(char* name) {
	for(int i = 0; i < airportcounter; i++) {
		if(strcmp(airports[i].AirportID,name) == 0) {
			return airports[i].portnumber;
		}
	}
	return NULL;
}

bool check_for_ID(char* name) {
	for(int i = 0; i < airportcounter; i++) {
		if(strcmp(name,airports[i].AirportID) == 0) {
			return true;
		}
	}
	return false;
}

bool colon_check(char* str) {
	int counter = 0;
	for(int i = 0; i < strlen(str); i++) {
		if(str[i] == ':') {
			counter++;
		}
	}
	if(counter == 1) {
		return true;
	}
	return false;
}
				
void* interpret_message(void* arg) {
	int* fdread = (int*) arg;
	FILE* readfile = fdopen(*fdread,"r");
	while(feof(readfile) == 0) {
		char* message = calloc(20,sizeof(char));
		char* airPortName = calloc(20,sizeof(char));
		char** names = calloc(airportcounter,sizeof(char*));
		for(int i = 0; i < airportcounter; i++) {
			names[i] = calloc(strlen(airports[i].AirportID),sizeof(char));
		}
		message = read_message(readfile);
		if(feof(readfile) != 0) {
			break;
		}
		int fdwrite = dup(*fdread);
		FILE* writefile = fdopen(fdwrite,"w");
		if(airportcounter > 0) {
			names = store_lexogrphical(names);
		}
		if(message[0] == '!') {
			bool valid = true;
			int counter = 0;
			int name = 0;
			int port = 0;
			int colon = find_char(message,':');
			airports[airportcounter].AirportID = calloc(79,sizeof(char));
			airports[airportcounter].portnumber = calloc(50,sizeof(char));
			if(!colon_check(message)) {
				valid = false;
			}
			if(colon == strlen(message) - 1) {
				valid = false;
			}
			for(int b = 1; b < colon; b++) {
				airPortName[counter] = message[b];
				counter++;
			}
			for(int g = colon + 1; g < strlen(message); g++) {
				if(isdigit(message[g]) == 0) {
					valid = false;
				}
			}
			for(int c = 1; c < colon; c++) {
				if(check_for_invalid_chars(message[c])) {
					valid = false;
				}
			}
			for(int g = colon + 1; g < strlen(message); g++) {
				if(check_for_invalid_chars(message[g])) {
					valid = false;
				}
			}
			if((!check_for_ID(airPortName) || airportcounter == 0) && valid) {
				pthread_mutex_lock(&mutex);
				for(int i = 1; i < colon; i++) {
					airports[airportcounter].AirportID[name] = message[i];
					name++;
				}
				for(int i = colon + 1; i < strlen(message); i++) {
					airports[airportcounter].portnumber[port] = message[i];
					port++;
				}
				airportcounter++;
				
			}
			pthread_mutex_unlock(&mutex);
		}
		if (message[0] == '?') {
			if(airportcounter == 0 ) {
				fprintf(writefile,";\n");
				fflush(writefile);
			}
			for(int i = 0; i < airportcounter; i++) {
				if(strcmp(&message[1],airports[i].AirportID) == 0) {
					fprintf(writefile,"%s\n",airports[i].portnumber);
					fflush(writefile);
					break;
				}
				else if( i == airportcounter - 1) {
					fprintf(writefile,";\n");
					fflush(writefile);
					break;
				}
			}
		}
		if( message[0] == '@' && strlen(message) == 1) {
			for(int i = 0; i < airportcounter; i++) {
				fprintf(writefile,"%s:%s\n",names[i],find_portnumber(names[i]));
				fflush(writefile);
			}
		}
	}
	return NULL;
}

int main(int argc, char** argv) {
	int server = open_socket();
	pthread_t thread[20];
	while(connections[sockets].fileDescriptor = 
	accept(server,0,0),connections[sockets].fileDescriptor >= 0) {
		pthread_create(&thread[sockets], NULL, interpret_message,&connections[sockets].fileDescriptor);
		pthread_detach(thread[sockets]);
		sockets++;
	}
	//pthread_exit(NULL);
	
}