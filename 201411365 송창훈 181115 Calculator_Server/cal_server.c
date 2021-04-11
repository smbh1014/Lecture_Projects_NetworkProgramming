#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFF_SIZE 1024


//---------------------------------------
//	output is bounded (-176 ~ 79)
//---------------------------------------



void error_handling(char *message);

int main(int argc, char *argv[]){
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	char buffer[3][BUFF_SIZE];
	char err_mess[] = "Wrong input\n";
	char send_mess[3] = "000";
	int number[2];
	char oper;
	int len, addr_len, recv_len;
	int option = 1, i = 0, j;

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
		recv_len = recv(clnt_sock, buffer[i], BUFF_SIZE, 0);

		if(recv_len == -1)
			error_handling("read() error!");
		
		buffer[i][recv_len] = '\n';
		//printf("Message frome Client : %s\n", buffer[i]);
		printf("%c...\n", buffer[i][0]);		

		if(buffer[i][0] >= 48 && buffer[i][0] <= 57){		
			number[i] = atoi(buffer[i]);
			printf("number[%d] = %d\n", i, number[i]);
			i++;
		}
		else if(buffer[i][0] == '*' || buffer[i][0] == '+' || 
			buffer[i][0] == 47 || buffer[i][0] == 45){

			oper = buffer[i][0];
			printf("oper = %c\n", oper);
			i++;
		}
		else{
			printf("%s", err_mess);
			send(clnt_sock, err_mess, strlen(err_mess), 0);
			i = 0;
		}

		if (i == 3)
			if(oper == '*'){
				send_mess[0] = number[0] * number[1] + 48;
				i = 0;
				send_mess[1] = 1;
				send_mess[2] = '\0';
				printf("Result = %d\n", send_mess[0] - 48);
				printf("Result of Calculator : %d\n", send_mess[0] - 48);
				send(clnt_sock, send_mess, strlen(send_mess), 0);
				send_mess[1] = 0;
			}
			else if(oper == '/'){
				send_mess[0] = (float)number[0] / (float)number[1] + 48;
				i = 0;
				send_mess[1] = 1;
				send_mess[2] = '\0';
				printf("Result = %d\n", send_mess[0] - 48);
				printf("Result of Calculator : %d\n", send_mess[0] - 48);
				send(clnt_sock, send_mess, strlen(send_mess), 0);
				send_mess[1] = 0;
			}
			else if(oper == '+'){
				send_mess[0] = number[0] + number[1] + 48;
				i = 0;
				send_mess[1] = 1;
				send_mess[2] = '\0';
				printf("Result = %d\n", send_mess[0] - 48);
				printf("Result of Calculator : %d\n", send_mess[0] - 48);
				send(clnt_sock, send_mess, strlen(send_mess), 0);
				send_mess[1] = 0;
			}
			else if(oper == '-'){
				send_mess[0] = number[0] - number[1] + 48;
				i = 0;
				send_mess[1] = 1;
				send_mess[2] = '\0';
				printf("Result = %d\n", send_mess[0] - 48);
				printf("Result of Calculator : %d\n", send_mess[0] - 48);
				send(clnt_sock, send_mess, strlen(send_mess), 0);
				send_mess[1] = 0;
			}
			
	
		close(clnt_sock);
		for(j = 0; j < 3; j++){
			buffer[j][0] = ' ';
			send_mess[j] = '0';
		}
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

