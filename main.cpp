//
// Created by youss on 1/25/2024.
//

#include "iostream"
#include "utils.h"

using namespace std;
string utils::lastestHludaTag = utils::getRecentTag();

int main() {
    try {
        utils::createModuleProps(); // first step
        utils::downloadHludaServers(); // second step -> downloads strong frida server
        utils::copyModuleTemplate();// 3rd step copies module's template from base directory
    } catch (const int &e) {
        return e;
    } catch (const string &e) {
        cout << e;
        return -1;
    }
}