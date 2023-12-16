//
// Created by 16182 on 11/29/2023.
//

#ifndef ALGO_PROJECT_MEM_H
#define ALGO_PROJECT_MEM_H
#include "windows.h"
#include "psapi.h"
#include<iostream>

inline double get_mem(){
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    return (virtualMemUsedByMe / (double)1000000);
}

#endif //ALGO_PROJECT_MEM_H
