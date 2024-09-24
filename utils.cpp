#include <fstream>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <chrono>
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"
#include "rapidjson/document.h"
#include "utils.h"

namespace fs = std::filesystem;

std::string utils::getRecentTag() {
    const std::string url = "https://api.github.com/repos/hzzheyang/strongR-frida-android/releases/latest";
    RestClient::Response response = RestClient::get(url);

    if (response.code != 200) {
        throw std::runtime_error("HTTP Error: " + std::to_string(response.code) + " " + response.body);
    }

    rapidjson::Document d;
    d.Parse(response.body.c_str());

    if (!d.HasMember("tag_name") || !d["tag_name"].IsString()) {
        throw std::runtime_error("Invalid JSON response: missing or invalid 'tag_name'");
    }

    std::string tag = d["tag_name"].GetString();
    std::ofstream("currentTag.txt") << tag;
    return tag;
}

void download(const std::string &aarch) {

    auto start = std::chrono::system_clock::now();

    std::cout << "Starting To Downloaded hluda for arch: " + aarch + "\n";

    std::string url = "https://github.com/hzzheyang/strongR-frida-android/releases/download/" + utils::latestHludaTag +
                      "/hluda-server-" + utils::latestHludaTag + "-android-" + aarch + ".gz";

    std::unique_ptr<RestClient::Connection> pConnection(new RestClient::Connection(url));
    pConnection->FollowRedirects(true);
    RestClient::Response response = pConnection->get("/");

    if (response.code != 200) {
        throw std::runtime_error("Download failed: " + std::to_string(response.code) + " " + response.body);
    }
    std::string filename;
    if (aarch == "x86_64")
        filename = "tmp/bin/hluda-x64.gz";
    else
        filename = "tmp/bin/hluda-" + aarch + ".gz";

    std::ofstream downloadedFile(filename, std::ios::out | std::ios::binary);

    if (!downloadedFile) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    downloadedFile.write(response.body.c_str(), response.body.length());

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout
            << "Successfully Downloaded hluda for arch: " + aarch + ". Took " + to_string(elapsed_seconds.count()) +
               "s\n";
}

void utils::downloadServers() {
    fs::create_directories("tmp/bin");
    const std::vector<std::string> archs = {"arm", "arm64", "x86", "x86_64"};

    for (const auto &aarch: archs) {
        try {
            download(aarch);
        } catch (const std::exception &e) {
            std::cerr << "Error downloading " << aarch << ": " << e.what() << std::endl;
            throw std::runtime_error("Error downloading " + aarch + ": " + e.what());
        }
    }
}

void utils::createModuleProps() {
    fs::create_directory("tmp");
    std::ofstream moduleProps("tmp/module.prop");

    if (!moduleProps) {
        throw std::runtime_error("Failed to open module.prop for writing");
    }

    moduleProps << "id=magisk-hluda\n"
                << "name=StrongR-Frida Server on Boot\n"
                << "version=" << latestHludaTag.substr(0, latestHludaTag.find('-')) << '\n'
                << "versionCode=" << latestHludaTag.substr(0, latestHludaTag.find('.')) << '\n'
                << "author=The Community - hzzheyang - Ylarod - Exo1i\n"
                << "description=Runs frida-server on boot\n";
}

void utils::copyModuleTemplate() {
    const std::string basePath = "../base/";
    fs::copy(basePath, "tmp/", fs::copy_options::recursive);
}
