#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFF_SIZE 1024

void error_handling(char *message);
 
int main(int argc, char* argv[]){
	int sock;
	struct sockaddr_in serv_addr;
	char buffer[BUFF_SIZE];
	char recv_buffer[BUFF_SIZE];
	int recv_len;

	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

while(1){
	printf("type your message : ");
	scanf("%s", buffer);

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        	perror("socket ");
        	return 1;
	}

	memset(&serv_addr, 0x00, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        	perror("connect ");
        	return 1;
 	}

	
	if(send (sock, buffer, strlen(buffer), 0) < 0)
		error_handling("send() error!");

	recv_len = recv(sock, recv_buffer, BUFF_SIZE, 0);
	if(recv < 0)
		error_handling("read() error!");

	recv_buffer[recv_len] = '\0';

	printf("Message from server : %s\n", recv_buffer);
}
	close(sock);

	return 0;
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

