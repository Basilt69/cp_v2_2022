#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>

#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 256
#define PORT 8080
#define MAX_EPOLL_EVENTS 32
static int sFd;

void cleanup() {
    close(sFd);
}

void sighandler(int sig) {
    cleanup();
    printf("\nЗавершение работы...\n");
    exit(0);
}

int main() {
    sFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sFd == -1) {
        printf("Socket error\n");
        return -1;
    }
    struct epoll_event events[MAX_EPOLL_EVENTS];
    int epfd = epoll_create(1);
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLOUT;
    event.data.fd = sFd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sFd, &event);
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    if (bind(sFd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        printf("Bind error\n");
        cleanup();
        return -1;
    }
}
