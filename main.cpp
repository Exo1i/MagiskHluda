#include "utils.h"
#include <iostream>
#include "restclient-cpp/restclient.h"

std::string utils::latestTag = utils::getRecentTag();

int main() {
    try {
        RestClient::init();
        utils::createModuleProps();
        utils::createUpdateJson();
        utils::downloadServers();
        RestClient::disable();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        RestClient::disable();
        return 1;
    }
}