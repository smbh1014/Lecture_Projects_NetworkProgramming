#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[]){
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	char buffer[BUFF_SIZE];
	int len, addr_len, recv_len;
	int option = 1;

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0x00, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error_handling("bind() error");

	if(listen(serv_sock, 5) < 0)
		error_handling("listen() error");
		
	addr_len = sizeof(clnt_addr);

	printf("waiting for clinet..\n");

	while((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &addr_len)) > 0){
		recv_len = recv(clnt_sock, buffer, BUFF_SIZE, 0);
		if(recv_len == -1)
			error_handling("read() error!");
		    
		buffer[recv_len] = '\0';
		printf("Message from Client : %s\n", buffer);
		send(clnt_sock, buffer, strlen(buffer), 0);
	
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

