#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char** argv)
{
	int server_sockfd, client_sockfd;
	int len, sin_size;
	char buf[BUFSIZ];
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = (INADDR_ANY);
	bzero(&(server_addr.sin_zero), 8);

	if ((bind(server_sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr))) == -1)
	{
		perror("bind error");
		exit(1);
	}

	if (listen(server_sockfd, 10) == -1)
	{
		perror("listen error");
		exit(1);
	}

	sin_size = sizeof(struct sockaddr_in);
	client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &sin_size);
	if (client_sockfd == -1)
	{
		perror("accept error");
		exit(1);
	}

	printf("Client addr is >> %s\n", inet_ntoa(client_addr.sin_addr));
	len = send(client_sockfd, "welcome to server\n", strlen("welcome to server\n"), 0);

	while ((len = recv(client_sockfd, buf, BUFSIZ, 0)) > 0) {
		buf[len] = '\0';
		printf("%s\n", buf);

		if (send(client_sockfd, buf, len, 0) == -1)
		{
			perror("send error");
			exit(1);
		}
	}
	close(client_sockfd);
	close(server_sockfd);
	return 0;
}
