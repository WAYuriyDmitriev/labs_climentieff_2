//
// Created by yuriydmitriev on 9/22/20.
//
#include <iostream>
#include <unistd.h>
#include "ProcessUtils.h"

#include <spawn.h>
#include <semaphore.h>


int main() {
    init_process(SEM_PLUS, Main_plus, [](double a, double b) { return a + b; });

    return 0;
}
