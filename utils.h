//
// Created by youss on 1/25/2024.
//
#include <iostream>

#ifndef MAGISKHLUDA_UTILS_H
#define MAGISKHLUDA_UTILS_H

using namespace std;

class utils {
public:
    static string latestTag;

    static void copyModuleTemplate();

    static void createModuleProps();

    static string getRecentTag();


    static void downloadServers();

    static void createUpdateJson();
};

#endif //MAGISKHLUDA_UTILS_H
