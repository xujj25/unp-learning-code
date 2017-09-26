#include "unp.h"
#include <time.h>
#include <sockfd_to_family.h>

void doit(struct sockaddr_in cliaddr, int connfd) {
    char buff[MAXLINE];
    time_t ticks;

    printf("connection from %s, port %d\n", 
    Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
    ntohs(cliaddr.sin_port));

    // add test for sockfd_to_family
    printf("connection sockfd family: %d\n", sockfd_to_family(connfd));


    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    Write(connfd, buff, strlen(buff));
}

int main(int argc, char const *argv[])
{   
    pid_t pid;
	int listenfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cliaddr;

  	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(13); // daytime server

	Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		len = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *)&cliaddr, &len);  // probably blocks
        if ((pid = Fork()) == 0) {
            Close(listenfd);  // child closes listening socket
            doit(cliaddr, connfd);  // process the request
            Close(connfd);  // done with this client
            exit(0);
        }
	}
	
	exit(0);
}