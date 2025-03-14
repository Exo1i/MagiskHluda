#include <fstream>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <algorithm>
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"
#include "rapidjson/document.h"
#include "utils.h"

namespace fs = std::filesystem;
const std::string basePath = "./module_template/";

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

    std::cout << "Starting To Downloaded florida for arch: " + aarch + "\n";

    std::string url = "https://github.com/ylarod/florida/releases/download/" + utils::latestTag +
                      "/florida-server-" + utils::latestTag + "-android-" + aarch + ".gz";

    std::unique_ptr<RestClient::Connection> pConnection(new RestClient::Connection(url));
    pConnection->FollowRedirects(true);
    RestClient::Response response = pConnection->get("/");

    if (response.code != 200) {
        throw std::runtime_error("Download failed: " + std::to_string(response.code) + " " + response.body);
    }
    std::string filename;
    if (aarch == "x86_64")
        filename = "bin/florida-x64.gz";
    else
        filename = "bin/florida-" + aarch + ".gz";

    std::ofstream downloadedFile(filename, std::ios::out | std::ios::binary);

    if (!downloadedFile) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    downloadedFile.write(response.body.c_str(), response.body.length());

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout
            << "Successfully Downloaded florida for arch: " + aarch + ". Took " + to_string(elapsed_seconds.count()) +
               "s\n";
}

void utils::downloadServers() {
    fs::create_directories("./bin");
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
    std::ofstream moduleProps(basePath + "module.prop");

    if (!moduleProps) {
        throw std::runtime_error("Failed to open module.prop for writing");
    }

    string versionCode = latestTag;
    versionCode.erase(std::remove(versionCode.begin(), versionCode.end(), '.'), versionCode.end());
    moduleProps << "id=magisk-hluda\n"
                << "name=Frida(Florida) Server on Boot\n"
                << "version=" << latestTag.substr(0, latestTag.find('-')) << '\n'
                << "versionCode=" << versionCode << '\n'
                << "author=The Community - Ylarod - Exo1i\n"
                << "description=Runs a stealthier frida-server on boot\n"
                << "updateJson=https://github.com/exo1i/magiskhluda/releases/latest/download/update.json";
}

void utils::createUpdateJson() {
    string versionCode = latestTag;
    versionCode.erase(std::remove(versionCode.begin(), versionCode.end(), '.'), versionCode.end());

    std::ofstream updateJson("update.json");
    if (!updateJson) {
        throw std::runtime_error("Failed to open update.json for writing");
    }

    updateJson << "{\n"
               << R"(  "version": ")" << latestTag << "\",\n"
               << "  \"versionCode\": " << versionCode << ",\n"
               << R"(  "zipUrl": "https://github.com/exo1i/magiskhluda/releases/download/)"
               << latestTag << "/Magisk-Florida-Universal-" << latestTag << ".zip\"\n"
               << "}\n";
}

