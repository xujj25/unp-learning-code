#include <unp.h>

// void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen) {
//     int n;
//     char sendline[MAXLINE], recvline[MAXLINE + 1];

//     while (Fgets(sendline, MAXLINE, fp) != NULL) {
//         Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

//         n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);

//         recvline[n] = 0;  /* null terminate */
//         Fputs(recvline, stdout);
//     }
// }

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen) {
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    socklen_t len;
    struct sockaddr *preply_addr;

    preply_addr = (struct sockaddr *)Malloc(servlen);

    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

        len = servlen;
        n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);  // have modification here
        if (len != servlen || memcmp(pservaddr, preply_addr, len) != 0) {
            printf("reply from %s (ignored)\n", Sock_ntop(preply_addr, len));
            continue;
        }

        recvline[n] = 0;  /* null terminate */
        Fputs(recvline, stdout);
    }
}