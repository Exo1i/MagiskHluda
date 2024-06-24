#include <fstream>
#include <filesystem>
#include "restclient-cpp/connection.h"
#include "iostream"
#include "restclient-cpp/restclient.h"
#include "rapidjson/document.h"
#include "utils.h"

using namespace std;
namespace fs = std::filesystem;


utils::utils() {}

string utils::getRecentTag() {
    RestClient::Response response = RestClient::get(
            "https://api.github.com/repos/Exo1i/Florida/releases/latest");
    rapidjson::Document d;
    if (response.code == 200)
        d.Parse(response.body.c_str());
    else {
        cout << "An Error code has happen: " + to_string(response.code) + response.body << endl;
        throw response.code;
    }
    fstream("currentTag.txt", ios::out) << d["tag_name"].GetString();
    return d["tag_name"].GetString();

}

void utils::download(const string &aarch) {
    string url = "https://github.com/Exo1i/Florida/releases/download/" + latestFloridaTag +
                 "/florida-server-" + latestFloridaTag + "-android-" + aarch + ".gz";
    RestClient::init();
    RestClient::Connection *pConnection = new RestClient::Connection(url);
    pConnection->FollowRedirects(true);
    RestClient::Response response = pConnection->get("/");
    fstream downloadedFile = fstream("tmp/files/florida-" + aarch + ".gz",
                                     ios::out | ios::binary);
    if (downloadedFile.is_open() && downloadedFile.good() && response.code == 200) {
        downloadedFile << response.body;
        downloadedFile.close();
        delete pConnection;
    } else {
        delete pConnection;
        throw "An error occurred: baseURl:" + pConnection->GetInfo().baseUrl + "\nand response body: " + response.body +
              "\nand response code : " + to_string(response.code);
    };


};

void utils::downloadFloridaServers() {
    fs::create_directory("tmp/files");
    for (const string &aarch: {"arm", "arm64", "x86", "x86_64"}) {
        try {
            download(aarch);
        } catch (const string &errMsg) {
            cout << errMsg;
        }
    }
}

void utils::createModuleProps() {
    fs::create_directory("tmp");
    fstream moduleProps = fstream("tmp/module.prop", ios::out);
    if (moduleProps.is_open()) {
        moduleProps << "id=magisk-Florida\n"
                       "name=MagiskFlorida\n" <<
                    "version=" << latestFloridaTag.substr(0, latestFloridaTag.find('-')) << endl;
        moduleProps <<
                    "versionCode=" << latestFloridaTag.substr(0, latestFloridaTag.find('.')) << endl <<
                    "author=Exo1i - Enovella - Ylarod - The Community\n"
                    "description=Runs frida-server on boot\n";
    }
    moduleProps.close();
}

void utils::copyModuleTemplate() {
    string basePath = "../base/";
    fs::copy(basePath, "tmp/", fs::copy_options::recursive);
}

