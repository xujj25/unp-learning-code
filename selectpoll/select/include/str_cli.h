#include "unp.h"

// void
// str_cli(FILE *fp, int sockfd)
// {
//     int maxfdp1;
//     fd_set rset;
//     char sendline[MAXLINE], recvline[MAXLINE];

//     FD_ZERO(&rset);
//     for ( ; ; ) {
//         FD_SET(fileno(fp), &rset);
//         FD_SET(sockfd, &rset);
//         maxfdp1 = max(fileno(fp), sockfd) + 1;
//         Select(maxfdp1, &rset, NULL, NULL, NULL);

//         if (FD_ISSET(sockfd, &rset)) {  /* socket is readable */
//             if (Readline(sockfd, recvline, MAXLINE) == 0)
//                 err_quit("str_cli: server terminated prematurely");
//             Fputs(recvline, stdout);
//         }

//         if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
//             if (Fgets(sendline, MAXLINE, fp) == NULL)
//                 return;  /* all done */
//             Writen(sockfd, sendline, strlen(sendline));
//         }
//     }
// }



//下面是str_cli函数的改进（且正确）的版本。它使用了select和shutdown，
//其中前者只要服务器关闭它那一端的连接就会通知我们，后者允许我们正确地处理批量输入。
//这个版本还废弃了以文本为中心的代码，改而针对缓冲区操作，从而消除了6.5节中提出的复杂性问题。
void str_cli(FILE *fp, int sockfd)
{
    int maxfdp1, stdineof;
    fd_set rset;
    char buf[MAXLINE];
    int n;

    stdineof = 0;
    FD_ZERO(&rset);
    for ( ; ; ) {
        if (stdineof == 0)
            FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {  /* socket is readable */
            if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
                if (stdineof == 1)
                    return;  /* normal termination */
                else
                    err_quit("str_cli: server terminated prematurely");
            }
            Write(fileno(stdout), buf, n);
        }
        if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
            if ( (n = Read(fileno(fp), buf, MAXLINE)) == 0) {
                stdineof = 1;
                Shutdown(sockfd, SHUT_WR);  /* send FIN */
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            Writen(sockfd, buf, n);
        }
    }
}