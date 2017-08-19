#include "unp.h"

// void str_cli(FILE *fp, int sockfd)
// {
//     char sendline[MAXLINE], recvline[MAXLINE];

//     while (Fgets(sendline, MAXLINE, fp) != NULL) {
//         Writen(sockfd, sendline, strlen(sendline));

//         if (Readline(sockfd, recvline, MAXLINE) == 0)
//             err_quit("str_cli: server terminated prematurely");
        
//             Fputs(recvline, stdout);
//     }
// }

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        Writen(sockfd, sendline, strlen(sendline));

        sleep(1);

        Writen(sockfd, sendline + 1, strlen(sendline) - 1);

        if (Readline(sockfd, recvline, MAXLINE) == 0)
            err_quit("str_cli: server terminated prematurely");
        
            Fputs(recvline, stdout);
    }
}