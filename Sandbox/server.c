#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <errno.h>

#define BUF_SIZE 1024

int main(int argc, char ** argv)
{

	
	int listenfd, client_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	
	char buf[1024] = {0};
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1)
	{
		printf("Error: socket\n");
		//perror("socket");
		exit(1);	
	}	
		
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(843); // 服务器监听端口
	
	
	// 解决 bind failed address already in use 问题
	int on = 1;
	if( (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0))
	{
		//perror("setsockopt");
		exit(1);	
	}	
	
	if( bind(listenfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
	{
		printf("Error: bind\n");
		perror("bind fail.");
		exit(1);
	}	
	
	// 监听listen	
	if( listen(listenfd, 5) < 0)
	{
		printf("Error: listen\n");
		//perror("listen");
		exit(1);
	}	
	
	int sin_size = sizeof(struct sockaddr_in);

	
	char rbuf[256] = "<?xml version=\"1.0\"?>\r\n"
			 "<cross-domain-policy> \r\n"
				"<allow-access-from domain=\"*\" to-ports=\"*\"/>\r\n"
			 "</cross-domain-policy> ";

	while(1)
	{
		printf("Start to accept\n");
		// accept
		client_fd = accept( listenfd, (struct sockaddr *)&client_addr, &sin_size );
		if( client_fd < 0 )
		{
			printf("Error: %d %s\n", errno, strerror(errno) );
			exit(1);
		}
		printf("accept client %s\n", inet_ntoa(client_addr.sin_addr) );
		
		int nLen;
			
		GO_RECV:
		// recv
		memset(buf, 0, 1024);
		nLen = recv(client_fd, buf, 1024, 0);
		if( nLen > 0 )
		{
			buf[nLen] = '\0';
			
			if( 0 == strcmp("<policy-file-request/>", buf) )
			{
				if( strlen(rbuf) == send( client_fd, rbuf, strlen(rbuf) + 1, 0 ) )
				{
					printf("# Send policy xml string success!\n");
					// goto GO_RECV;
				}
				else
				{
					printf("Error: Send policy xml fail\n");
				}
				close(client_fd);
			}
		}
		else
		{
			printf("Recv Error.\n");
		}
	}

	
	close(listenfd);
	close(client_fd);

	return 0;	
}
