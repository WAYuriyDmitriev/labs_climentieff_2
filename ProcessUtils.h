//
// Created by yuriydmitriev on 10/18/20.
//

#ifndef UNTITLED2_PROCESSUTILS_H
#define UNTITLED2_PROCESSUTILS_H

#include "semaphore.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include <fcntl.h>
#include "functional"

const std::string SEM_PLUS = "/plus";
const std::string SEM_POW2 = "/pow2";
const std::string SEM_SQRT = "/sqrt";
const int SEMAPHORE_MODE = 0660;

enum SOCKET_ACTION {
    Bind,
    Connect
};

enum SOCKET_PORT {
    Main_plus = 2468,
    Main_sqrt = 2467,
    Main_pow2 = 2466,
    Operation = 2469
};

sem_t *wait_init_semaphore(const char *name);

int init_socket(SOCKET_ACTION action, int port, sockaddr_in addr);

void wait_semaphore(sem_t *sem, int wait_value);

double recvFromSocket(int socket, double value);

void sendToSocket(int socket, double value);

void dec_sem_and_wait(sem_t *sem, int wait_value);

void inc_sem_and_wait(sem_t *sem, int wait_value);

void init_process(const std::string semaphore_name, int port, const std::function<double(double, double)> &lambda);

#endif //UNTITLED2_PROCESSUTILS_H