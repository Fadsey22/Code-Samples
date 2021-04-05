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

// global string to store messages
char* plane;
// global 2D array to store the airports
char** loginfo;
int logCounter = 0;
int connect_socket(const char* port) {
    int errorCheck = 0;
    struct addrinfo* addr = 0;
    struct addrinfo addrhints;
    memset(&addrhints, 0, sizeof(struct addrinfo));
    addrhints.ai_family = AF_INET;
    addrhints.ai_socktype = SOCK_STREAM;
    getaddrinfo("localhost", port, &addrhints, &addr);
    int fd = socket(AF_INET, SOCK_STREAM, 0); // 0 == use default protocol
    errorCheck = connect(fd, 
	(struct sockaddr*)addr->ai_addr, sizeof(struct sockaddr));
    if(errorCheck == -1) {
		return - 1;
    }
    return fd;    
}

int connect_mapper(const char* port) {
    int errorCheck = 0;
    struct addrinfo* addr = 0;
    struct addrinfo addrhints;
    memset(&addrhints, 0, sizeof(struct addrinfo));
    addrhints.ai_family = AF_INET;
    addrhints.ai_socktype = SOCK_STREAM;
    getaddrinfo("localhost", port, &addrhints, &addr);
    int fd = socket(AF_INET, SOCK_STREAM, 0); // 0 == use default protocol
    errorCheck = connect(fd, 
	(struct sockaddr*)addr->ai_addr, sizeof(struct sockaddr));
	if(errorCheck == -1) {
		fprintf(stderr, "Failed to connect to mapper\n");
		exit(4);
	}
    return fd;    
}

bool check_destination(char* dest) {
	for(int i = 0; i < strlen(dest); i++) {
		if(isdigit(dest[i]) == 0) {
			return false;
		}
	}
	return true;
}

void arg_check(int argc, char** argv) {
	if(argc < 3) {
	fprintf(stderr, "Usage: roc2310 id mapper {airports}\n");
	exit(1);
	}
	if(strcmp(argv[2], "-") != 0 && (atoi(argv[2]) <= 0 
	|| atoi(argv[2]) >= 65535)) { 
		fprintf(stderr, "Invalid mapper port\n");
		exit(2);
	}
	if(strcmp(argv[2], "") == 0) {
		fprintf(stderr, "Invalid mapper port\n");
		exit(2);
	}
	for(int i = 3; i < argc; i++) {
		if(strcmp(argv[2], "-") == 0 
		&& (!check_destination(argv[i]))) {
			fprintf(stderr, "Mapper required\n");
			exit(3);
		}
	}
	for(int i = 3; i < argc; i++) {
		if(strcmp(argv[2], "-") == 0 && (atoi(argv[i]) == 0 
		|| atoi(argv[i]) < 0 || atoi(argv[i]) >= 65535)) {  
			fprintf(stderr, "Mapper required\n");
			exit(3);
		}
	}
}

char* read_information(FILE* file) {
	char* information = malloc(80);
    int next, pos;
    pos = 0;
	while (true) {
        next = fgetc(file);
		if(feof(file) != 0) {
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

void* interpret(void* arg) {
	int* fdwrite = (int*) arg;
	FILE* writefile = fdopen(*fdwrite, "w");
	fprintf(writefile, "%s\n", plane);
	fflush(writefile);
	char* information = calloc(30, sizeof(char));
	int fdread = dup(*fdwrite);
	FILE* readfile = fdopen(fdread, "r");
	information = read_information(readfile);
	strcpy(loginfo[logCounter],information);
	logCounter++;
	//printf("%s\n", information);
	//fflush(stdout);
	return NULL;
}

char* ask_mapper_for_port(void* arg, char* argv) {
	char* port = calloc(50, sizeof(char));
	int* fdwrite = (int*) arg;
	FILE* writefile = fdopen(*fdwrite, "w");
	fprintf(writefile, "?%s\n", argv);
	fflush(writefile);
	int fdread = dup(*fdwrite);
	FILE* readfile = fdopen(fdread, "r");
	port = read_information(readfile);
	if(feof(readfile) != 0) {
		fprintf(stderr, "No map entry for destination\n");
		exit(5);
	}
	if(strcmp(port, ";") == 0) {
		fprintf(stderr, "No map entry for destination\n");
		exit(5);
	}
	return port;
}

int main(int argc, char** argv) {
	arg_check(argc, argv);
	int badConnection = 0;
	int portCounter = 0;
	char** ports = calloc(50, sizeof(char*));
	for(int i = 0; i < 50; i++) {
		ports[i] = calloc(50, sizeof(char));
	}
	loginfo = calloc(argc, sizeof(char*));
	for(int i = 0; i < argc; i++) {
		loginfo[i] = calloc(79, sizeof(char));
	}
	plane = calloc(50, sizeof(char));
	stpcpy(plane, argv[1]);
	int fdwrite;
	int write;
	if(strcmp(argv[2], "-") != 0) {
		for(int i = 3; i < argc; i++) {
			write = connect_mapper(argv[2]);
			ports[portCounter] = ask_mapper_for_port(&write, argv[i]);
			portCounter++;
		}
		for(int i = 0; i < portCounter; i++) {
			fdwrite = connect_socket(ports[i]);
			if(fdwrite == -1) {
				badConnection++;
				continue;
			}
			interpret(&fdwrite);
		}
		if(badConnection != 0) {
			for(int i = 0; i < logCounter; i++) {
				fprintf(stdout,"%s\n",loginfo[i]);
				fflush(stdout);
			}
			fprintf(stderr,
			"Failed to connect to at least one destination\n");
			exit(6);
		}
		for(int i = 0; i < logCounter; i++) {
			fprintf(stdout,"%s\n",loginfo[i]);
			fflush(stdout);
		}
		
		return 1;
	}
	if(strcmp(argv[2], "-") == 0) {
		for(int i = 3; i < argc; i++) {
			fdwrite = connect_socket(argv[i]);
			if(fdwrite == -1) {
				badConnection++;
				continue;
			}
			interpret(&fdwrite);
		}
		if(badConnection != 0) {
			for(int i = 0; i < logCounter; i++) {
				fprintf(stdout,"%s\n",loginfo[i]);
				fflush(stdout);
			}
			fprintf(stderr,
			"Failed to connect to at least one destination\n");
			exit(6);
		}
		for(int i = 0; i < logCounter; i++) {
			fprintf(stdout,"%s\n",loginfo[i]);
			fflush(stdout);
		}
	}
}