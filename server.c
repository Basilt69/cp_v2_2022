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

void
