#include "utils.h"
#include <iostream>
#include "restclient-cpp/restclient.h"

std::string utils::latestHludaTag = utils::getRecentTag();

int main() {
    try {
        RestClient::init();
        utils::createModuleProps();
        utils::downloadFloridaServers();
        utils::copyModuleTemplate();
        RestClient::disable();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        RestClient::disable();
        return 1;
    }
}