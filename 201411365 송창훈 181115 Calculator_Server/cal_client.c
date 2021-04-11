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
	int recv_len, result, i;

	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	printf("type numbers and operator : ");

	while(1){
		scanf("%s", buffer);

				/*Do Not Modify*/
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
				/*Do Not Modify*/
		result = recv_buffer[0] - 48;
		recv_buffer[recv_len] = '\0';
		if(recv_buffer[1] == 1){
			fflush(stdin);
			for(i = 0; i < 3; i++)
				recv_buffer[i] = ' ';
			printf("Message from server : %d\n", result);
			printf("type numbers and operator : ");
		}
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

