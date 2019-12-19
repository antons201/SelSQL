//// Created by sapiest on 05.10.2019.//#pragma once#include <chrono>#include <filesystem>#include <thread>#include "../../src/Utils/Headers/CommonUtils.h"#include "../../src/Utils/Headers/Constants.h"#include "gtest/gtest.h"#ifdef __WIN32#define platform(lin, win) win#else#define platform(lin, win) lin#endifstd::string serv_dir = "../Server";std::string test_dir = "../Tests";class TestUtils {    static void clearDir(const std::string &dir) {        for (const auto &file : std::filesystem::directory_iterator(dir)) {            std::string path = file.path().string();            path.erase(path.begin(), path.begin() + dir.size() + 1);            if (std::filesystem::is_directory(file.path()) and path != "CMakeFiles") {                std::filesystem::remove_all(file.path());            }        }    }   public:    static void clear() {        std::string command;        clearDir(serv_dir);        clearDir(test_dir);    }    static void kill() {        const std::string name_ = "Server";        std::string commandWin = "taskkill /IM " + name_ + ".exe /F";        std::string commandLin = "killall " + name_;        std::system(platform(commandLin, commandWin).c_str());#ifdef __linux        std::system("killall konsole");#endif    }    static void run() {        const std::string name_ = "Server";        const std::string directory_ = "..\\Server\\";        std::string commandWin = "start cmd.exe /c \"" + directory_ + name_ + ".exe\"";        std::string commandLin = "konsole -e ";        for (auto file : std::filesystem::directory_iterator(serv_dir)) {            std::string path = file.path().string();            path.erase(path.begin(), path.begin() + serv_dir.size() + 1);            if (path == "Server") {                commandLin += std::filesystem::absolute(file.path()).string();                commandLin += " &";                break;            }        }        std::system(platform(commandLin, commandWin).c_str());    }    static void checkRequests(const std::vector<std::pair<std::string, std::string>> &requests) {        Client client;        for (const auto &request : requests) {            client.execRequest(request.first);            EXPECT_EQ(client.response, request.second);        }    }    static void checkRequestsClient(const std::vector<std::pair<std::string, std::string>> &requests, Client &client) {        for (const auto &request : requests) {            client.execRequest(request.first);            EXPECT_EQ(client.response, request.second);        }    }    static void    checkDrop(const std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>> &request) {        bool runTest = true;        int timeKill = 0;        while (runTest) {            bool checkTest = false;            Client client;            client.sendMessage(request.first.first);            std::this_thread::sleep_for(std::chrono::milliseconds(timeKill));            kill();            run();            Client clientNew;            clientNew.execRequest(request.first.second);            if (!(clientNew.response == request.second.first or clientNew.response == request.second.second)) {                EXPECT_EQ("Bad file", "");                break;            }            if (clientNew.response == request.second.second) {                break;            }            if (timeKill > 500) {                EXPECT_EQ("Time out", "");                break;            }            timeKill += 1;        }    }};