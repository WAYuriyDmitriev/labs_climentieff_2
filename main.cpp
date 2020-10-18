#include <iostream>
#include <unistd.h>
#include "ProcessUtils.h"
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>


/*
 * ADJM
 * A – независимые процессы;
 * D – семафоры;
 * J – сокеты;
 * gipotenuza po dvum katetam
 *
 */
pid_t plus, pow2, sqrt, main_PID;

struct sockaddr_in mains{}, operations{};

double send_sem_operation(sem_t *semaphore, double &a, double &b, int main, int operation);

void startProcess(const std::string &name) {
    std::string way = "..";
    execv(way.append(name).c_str(), {});
}


int main() {
    sem_t *sem_plus, *sem_pow2, *sem_sqrt;
    sem_unlink(SEM_SQRT.c_str());
    sem_unlink(SEM_POW2.c_str());
    sem_unlink(SEM_PLUS.c_str());

    auto el = sem_plus = sem_open(SEM_PLUS.c_str(), O_CREAT, SEMAPHORE_MODE, 0);
    std::cout << el;
    sem_sqrt = sem_open(SEM_SQRT.c_str(), O_CREAT, SEMAPHORE_MODE, 0);
    sem_pow2 = sem_open(SEM_POW2.c_str(), O_CREAT, SEMAPHORE_MODE, 0);

    double a, b;
    std::cout << "Input a: ";
    std::cin >> a;

    std::cout << "Input b: ";
    std::cin >> b;

    main_PID = fork();

    if (main_PID) {
        int sock_plus, operation, sock_sqrt, sock_pow;
        sock_plus = init_socket(Connect, Main_plus, {});
        sock_sqrt = init_socket(Connect, Main_sqrt, {});
        sock_pow = init_socket(Connect, Main_pow2, {});
        operation = init_socket(Bind, Operation, operations);

        wait_semaphore(sem_plus, 1);
        std::cout << "Plus process init" << std::endl;

        wait_semaphore(sem_sqrt, 1);
        std::cout << "Sqrt process init" << std::endl;

        wait_semaphore(sem_pow2, 1);
        std::cout << "Pow2 process init" << std::endl;

        std::cout << "All process initialize" << std::endl;


        auto a2 = send_sem_operation(sem_pow2, a, a, sock_pow, operation);
        auto b2 = send_sem_operation(sem_pow2, b, b, sock_pow, operation);

        auto aPlusB = send_sem_operation(sem_plus, a2, b2, sock_plus, operation);
        auto sqrtAPlusB = send_sem_operation(sem_sqrt, aPlusB, aPlusB, sock_sqrt, operation);

        std::cout << sqrtAPlusB << std::endl;

        kill(plus, SIGKILL);
        kill(pow2, SIGKILL);
        kill(sqrt, SIGKILL);

        close(sock_plus);
        close(sock_pow);
        close(sock_sqrt);
        close(operation);

        return 0;
    }


    plus = fork();
    if (plus) {
        startProcess(SEM_PLUS);
        return 0;
    }

    pow2 = fork();
    if (pow2) {
        startProcess(SEM_POW2);
        return 0;
    }

    sqrt = fork();
    if (sqrt) {
        startProcess(SEM_SQRT);
        return 0;
    }


    return 0;
}

double send_sem_operation(sem_t *semaphore, double &a, double &b, int main, int operation) {
    send(main, &a, sizeof(a), 0);
    dec_sem_and_wait(semaphore, 1);

    send(main, &b, sizeof(b), 0);
    dec_sem_and_wait(semaphore, 1);

    double res;
    res = recvFromSocket(operation, res);
    return res;
}


