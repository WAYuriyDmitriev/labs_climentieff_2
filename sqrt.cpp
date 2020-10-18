//
// Created by yuriydmitriev on 9/22/20.
//
#include <iostream>
#include <unistd.h>
#include "ProcessUtils.h"

#include <spawn.h>
#include <semaphore.h>
#include <math.h>


int main() {
    init_process(SEM_SQRT, Main_sqrt, [](double a, double b) { return sqrt(a); });

    return 0;
}
