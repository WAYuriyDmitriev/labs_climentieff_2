//
// Created by yuriydmitriev on 10/18/20.
//
#include "semaphore.h"
#include <iostream>
#include <unistd.h>
#include <functional>
#include "ProcessUtils.h"

sem_t *wait_init_semaphore(const char *name) {
    sem_t *sem;
    do {
        sem = sem_open(name, 0, SEMAPHORE_MODE, 0);
    } while (sem == NULL);

    return sem;
}


int init_socket(SOCKET_ACTION action, int port, sockaddr_in addr) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(action == Bind ? INADDR_ANY : INADDR_LOOPBACK);

    if (action == Bind && bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        return -1;
    }

    if (action == Connect && connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        return -1;
    }

    return sock;
}

void wait_semaphore(sem_t *sem, int wait_value) {
    int value;
    do {
        sem_getvalue(sem, &value);
    } while (value != wait_value);

}


double recvFromSocket(int socket, double value) {
    recvfrom(socket, reinterpret_cast<void *>(&value), sizeof(value), 0, NULL, NULL);
    return value;
}

void sendToSocket(int socket, double value) {
    send(socket, &value, sizeof(value), 0);
}

void dec_sem_and_wait(sem_t *sem, int wait_value) {
    sem_wait(sem);
    wait_semaphore(sem, wait_value);
}

void inc_sem_and_wait(sem_t *sem, int wait_value) {
    sem_post(sem);
    wait_semaphore(sem, wait_value);
}

void init_process(const std::string semaphore_name, int port, const std::function<double(double, double)>  &lambda) {
    sem_t *sem = wait_init_semaphore(semaphore_name.c_str());

    int main, operation;
    main = init_socket(Bind, port, {});
    operation = init_socket(Connect, Operation, {});

    int again = 1;
    do {
        inc_sem_and_wait(sem, 0);

        double a, b;
        a = recvFromSocket(main, a);
        inc_sem_and_wait(sem, 0);

        b = recvFromSocket(main, b);
        sendToSocket(operation, lambda(a, b));


    } while (true);

    close(main);
    close(operation);

}

