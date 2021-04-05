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

char planes[79][79];
int planecounter = 0;
int threadcount = 0;
char* info;
char* airportId;
char* portString;
pthread_mutex_t mutex;
char* int_to_string(int number) {
	char* string = malloc(sizeof(char)*20);
	sprintf(string,"%d",number);
	return string;
}

int open_socket( void) {
	portString = calloc(50,sizeof(char));
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
		portnumber = (int) ntohs(ad.sin_port);
	}
	strcpy(portString,int_to_string(portnumber));
    fprintf(stdout,"%d\n",portnumber);
	fflush(stdout);
    listen(serv, 10);
    return serv;
}

int connect_mapper(const char *port) {
	int error_check = 0;
    struct addrinfo* addr = 0;
    struct addrinfo addrhints;
    memset(&addrhints, 0, sizeof(struct addrinfo));
    addrhints.ai_family=AF_INET;        // IPv6  for generic could use AF_UNSPEC
    addrhints.ai_socktype=SOCK_STREAM;
    getaddrinfo("localhost", port, &addrhints, &addr);
    int fd = socket(AF_INET, SOCK_STREAM, 0); // 0 == use default protocol
    error_check = connect(fd, (struct sockaddr*)addr->ai_addr, sizeof(struct sockaddr));
	if(error_check == -1) {
		fprintf(stderr,"Can not connect to map\n");
		exit(4);
	}
    return fd;    
}

bool check_for_invalid_chars(char chr) {
	if(chr == '\r' || chr == '\n' || chr == ':') {
		return true;
	
	}
	return false;
}
char* read_information(FILE* file) {
	char *information = malloc(80);
    int next, pos;
    pos = 0;
	while (true) {
        next = fgetc(file);
		if(feof(file)!=0) {
			fclose(file);
			return NULL;
		}
        if (next == EOF && pos == 0) {
            free(information);
            return NULL;
        }
        if (next == EOF || next == '\n') {
            information[pos] = '\0';
            break;
        }
        information[pos++] = (char) next;
    }
    return information;
}

void arg_check(int argc, char** argv) {
	if(argc < 3) {
		fprintf(stderr,"Usage: control2310 id info [mapper]\n");
		exit(1);
	}
	for(int i = 0; i < argc; i++) {
		if(strcmp(argv[i],"") == 0) {
			fprintf(stderr,"Usage: control2310 id info [mapper]\n");
			exit(1);
		}
	}
	if(argc > 3) {
		for(int i = 3 ; i < argc; i++) {
			if(atoi(argv[i]) <= 0) {
				fprintf(stderr,"Invalid port\n");
				exit(3);
			}
			if(atoi(argv[i]) >= 65535) {
				fprintf(stderr,"Invalid port\n");
				exit(3);
			}
		}
	}
	for(int i = 0; i < strlen(argv[1]); i++) {
		if(check_for_invalid_chars(argv[1][i])) {
			fprintf(stderr,"Invalid char in parameter\n");
			exit(2);
		}
	}
	for(int i = 0; i < strlen(argv[2]); i++) {
		if(check_for_invalid_chars(argv[2][i])) {
			fprintf(stderr,"Invalid char in parameter\n");
			exit(2);
		}
	}
}

char** store_lexogrphical(char** names) {
	names = calloc(79,sizeof(char*));
	char* temporary = calloc(200,sizeof(char));
	for(int i = 0; i < planecounter; i++) {
		names[i] = calloc(79,sizeof(char));
	}
	for(int j = 0; j < planecounter; j++) {
		strcpy(names[j],planes[j]);
	}
	if(planecounter > 1) {
		for(int a = 0; a < planecounter; ++a) {
			for(int b = a + 1; b < planecounter; ++b) {
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
void* write_server(void* arg) {
	int* fdread = (int*) arg;
	char* plane = calloc(50,sizeof(char));
	FILE* readfile = fdopen(*fdread,"r");
	while(feof(readfile) == 0) {
		char** names = calloc(79,sizeof(char*));
		for(int i = 0; i < planecounter; i++) {
			names[i] = calloc(79,sizeof(char));
		}
		pthread_mutex_lock(&mutex);
		names = store_lexogrphical(names);
		plane = read_information(readfile);
		pthread_mutex_unlock(&mutex);
		if(feof(readfile) != 0) {
			break;
		}
		int fdwrite = dup(*fdread);
		FILE* writefile = fdopen(fdwrite,"w");
		if(strcmp("log",plane) != 0) {
			fprintf(writefile,"%s\n",info);
			fflush(writefile);
			pthread_mutex_lock(&mutex);
			strcpy(planes[planecounter],plane);
			planecounter++;
			pthread_mutex_unlock(&mutex);
		}
		if(strcmp("log",plane) == 0) {
			for(int i = 0; i < planecounter; i++) {
				pthread_mutex_lock(&mutex);
				fprintf(writefile,"%s\n",names[i]);
				fflush(writefile);
				pthread_mutex_unlock(&mutex);
			}
			pthread_mutex_lock(&mutex);
			fprintf(writefile,".\n");
			fflush(writefile);
			pthread_mutex_unlock(&mutex);
		}
	}
	return NULL;
}

/**void signal_handler(int sig) {
	char** names = calloc(79,sizeof(char*));
	for(int i = 0; i < planecounter; i++) {
		names[i] = calloc(79,sizeof(char));
	}
	names = store_lexogrphical(names);
	for(int i = 0; i < planecounter; i++) {*/
		
void* write_to_map(void* arg) {
	int* fdwrite = (int*) arg;
	FILE* writefile = fdopen(*fdwrite,"w");
	fprintf(writefile,"!%s:%s\n",airportId,portString);
	fflush(writefile);
	return NULL;
}
		

int main(int argc, char** argv) {
	arg_check(argc,argv);
	airportId = calloc(79,sizeof(char));
	stpcpy(airportId,argv[1]);
	info = calloc(50,sizeof(char));
	stpcpy(info,argv[2]);
	pthread_t thread[20];
	pthread_mutex_init(&mutex,NULL);
	int fdread;
	int write;
	int server = open_socket();
	if(argc > 3) {
		for(int i = 3; i < argc; i++) {
			write = connect_mapper(argv[i]);
			write_to_map(&write);
		}
	}
	 while(fdread = accept(server,0,0),fdread >= 0) {
		pthread_create(&thread[threadcount], NULL,write_server,&fdread);
		pthread_detach(thread[threadcount]);
		threadcount++;
	 }
}