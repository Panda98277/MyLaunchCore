#include "pch.h"
#include "LaunchCore.h"
#include "replace_all.h"
#include "Format.h"

using namespace std;

namespace MyLaunchCore {
  LaunchCore::LaunchCore(string _gamepath, Settings _settings) {
    settings = _settings;
    gamepath = _gamepath;
  }

  LaunchCore::~LaunchCore() {

  };

  int LaunchCore::Launch(string gameid) {

    //启动参数
    string LaunchCommand;

#pragma region 读取版本Json文件

    ifstream infile;
    string file = Toolkits::Format(R"({0}\versions\\{1}\{1}.json)", gamepath, gameid);
    infile.open(file);

    infile.seekg(0, ios::end);
    int length = infile.tellg();
    infile.seekg(0, ios::beg);

    char* in = new char[length];

    infile.read(in, length);
    infile.close();
    //std::cout << in << std::endl;

    Json::Reader reader;
    Json::Value root;

    if (reader.parse(in, root)) {
      if (root["minecraftArguments"]) {
        launchargument.MinecraftArguments = root["minecraftArguments"].asString().data();
      }
      if (root["arguments"]["arguments"]) {
        int i;
        for (i = 0; i < size(root["arguments"]["arguments"]); i++) {
          launchargument.MinecraftArguments += root["arguments"]["arguments"][i].asString();
        }
      }
      else {
        return 0;
      }
    }
    delete(in);
#pragma endregion


    launchargument.Name = settings.auth.Name;
    launchargument.Name = settings.auth.UUID;
    launchargument.Gamedir = gamepath;
    launchargument.AssetsDir = Toolkits::Format(R"({0}\assets)", gamepath).data();

    LaunchCommand += "-Xmx ${Xmx} -Xms ${Xms} -Xmn ${Xmn} -XX:+UseG1GC -XX:-UseAdaptiveSizePolicy -XX:-OmitStackTraceInFastThrow";

    if (settings.gamewindow.IsFullscreen) LaunchCommand += "--fullscreen ";
    //Toolkits::replace_all();

#pragma endregion


#pragma region 开始游戏
    system(LaunchCommand.data());
#pragma endregion

    return 0;
  }
}
