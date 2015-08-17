#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
	int client_sockfd;
	int len;
	char buf[BUFSIZ];
	struct sockaddr_in server_addr;
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&(server_addr.sin_zero), 8);

	client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sockfd == -1)
	{
		perror("socket error");
		exit(1);
	}

	if (connect(client_sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect error");
		exit(1);
	}

	printf("Connect to server\n");
	len = recv(client_sockfd, buf, BUFSIZ, 0);
	buf[len] = '\0';
	printf("%s\n", buf);

	while (1) {
		printf("Input some string >> ");
		scanf("%s", buf);

		if (!strcmp(buf, "quit"))
			break;
		len = send(client_sockfd, buf, strlen(buf), 0);
		len = recv(client_sockfd, buf, BUFSIZ, 0);
		buf[len] = '\0';
		printf("%s\n", buf);
	}
	close(client_sockfd);
	return 0;
}
