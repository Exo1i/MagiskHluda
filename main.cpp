//
// Created by youss on 1/25/2024.
//

#include "iostream"
#include "utils.h"

using namespace std;
string utils::latestFloridaTag = utils::getRecentTag();

int main() {
    try {
        utils::createModuleProps();
        utils::downloadFloridaServers();
        utils::copyModuleTemplate();
    } catch (const int &e) {
        return e;
    } catch (const string &e) {
        cout << e;
        return -1;
    }
}