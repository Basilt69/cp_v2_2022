#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

#define BUF_SIZE 256
#define PORT 8080
#define SERVER "127.0.0.1"
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
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER, &serv_addr.sin_addr.s_addr) == 0) {
        perror("inet_pton error\n");
        cleanup();
        return -1;
    }

    if (connect(sFd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1 &&
        errno != EINPROGRESS) {
        perror("connect error\n");
        cleanup();
        return -1;
    }

    struct epoll_event events[MAX_EPOLL_EVENTS];
    int epfd = epoll_create(1);
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;
    event.data.fd = sFd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sFd, &event);

    if (signal(SIGINT, sighandler) == SIG_ERR) {
        perror("Signal error\n");
        cleanup();
        return -1;
    }

    char msgto[BUF_SIZE];
    char msgfrom[BUF_SIZE];

    snprintf(msgto, BUF_SIZE, "процесс %d", getpid());
    int num_ready;
    for (;;) {
        num_ready = epoll_wait(epfd, events, MAX_EPOLL_EVENTS, 1000);
        if (num_ready == -1) {
            perror("Epoll_wait error\n");
            cleanup();
            return -1;
        }
        for (int i = 0; i < num_ready; i++) {
            if (events[i].events & EPOLLOUT) {
                if (send(events[i].data.fd, msgto, sizeof(msgto), 0) == -1) {
                    perror("Send error\n");
                    cleanup();
                    return -1;
                } else {
                    printf("Отправлено сообщение: %s\n", msgto);
                }
                int bytes;

                if ((bytes = recv(events[i].data.fd, msgfrom, BUF_SIZE, 0)) == -1) {
                    perror("recv error");
                    cleanup();
                    return -1;
                } else {
                    msgfrom[bytes] = '\0';
                    printf("Получено сообщение: %s\n", msgfrom);
                }
            }
        }
        sleep(5);
    }
}