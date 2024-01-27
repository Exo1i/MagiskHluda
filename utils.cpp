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
            "https://api.github.com/repos/hzzheyang/strongR-frida-android/releases/latest");
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
    string url = "https://github.com/hzzheyang/strongR-frida-android/releases/download/" + lastestHludaTag +
                 "/hluda-server-" + lastestHludaTag + "-android-" + aarch + ".xz";
    RestClient::init();
    RestClient::Connection *pConnection = new RestClient::Connection(url);
    pConnection->FollowRedirects(true);
    RestClient::Response response = pConnection->get("/");
    fstream downloadedFile = fstream("tmp/files/hluda-" + aarch + ".xz",
                                     ios::out | ios::binary);
    if (downloadedFile.is_open() && downloadedFile.good() && response.code == 200) {
        downloadedFile << response.body;
        downloadedFile.close();
        delete pConnection;
    } else {
        delete pConnection;
        throw "An error occured: baseURl:" + pConnection->GetInfo().baseUrl + "\nand response body: " + response.body +
              "\nand response code : " + to_string(response.code);
    };


};

void utils::downloadHludaServers() {
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
        moduleProps << "id=magisk-hluda\n"
                       "name=MagiskHluda\n" <<
                    "version=" << lastestHludaTag.substr(0, lastestHludaTag.find('-')) << endl;
        moduleProps <<
                    "versionCode=" << lastestHludaTag.substr(0, lastestHludaTag.find('.')) << endl <<
                    "author=Exoi8\n"
                    "description=Run frida-server on boot\n";
    }
    moduleProps.close();
}

void utils::copyModuleTemplate() {
    string basePath = "../base/";
    fs::copy(basePath, "tmp/", fs::copy_options::recursive);
}

