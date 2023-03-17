#include "pch.h"
#include "LaunchCore.h"
#include "replace_all.h"
#include "Format.h"
#include "Log.h"


using namespace std;

namespace MyLaunchCore {
  LaunchCore::LaunchCore(string _gamepath, Settings _settings) {
    settings = _settings;
    gamepath = _gamepath;
    if (!launchlog.ofs) {
      this->Error = "无法打开日志 目录:";
    }
  }

  LaunchCore::~LaunchCore() {

  };

  int LaunchCore::Launch(string gameid) {

    launchlog.write("开始启动游戏");
    launchlog.write("游戏路径: "+ gamepath);

    //启动参数
    string LaunchCommand;

#pragma region 读取版本Json文件

    ifstream infile;
    string file = gamepath + "\\versions\\"+gameid+ "\\"+ gameid + ".json";
    launchlog.write("版本Json路径: " + file);
    infile.open(file);
    if (!infile) {
      launchlog.write("Error: 不合法的游戏路径");
      launchlog.write("Exit");
      return 0;
    }


    delete(&file);

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
        launchlog.write("Error: 无法读取的版本Json文件");
        launchlog.write("Exit");
        return 0;
      }
    }
    else {
      launchlog.write("Error: 无法读取的版本Json文件");
      launchlog.write("Exit");
      return 0;
    }

    delete(in);
#pragma endregion

    launchargument.Name = settings.auth.Name;
    launchargument.Name = settings.auth.UUID;
    launchargument.Gamedir = gamepath;
    launchargument.AssetsDir =  gamepath + "\\assets";

    /*launchlog.write("启动参数: " + launchargument);*/

    if (settings.gamewindow.IsFullscreen) LaunchCommand += "--fullscreen ";

    LaunchCommand += "-Xmx ${Xmx} -Xms ${Xms} -Xmn ${Xmn} -XX:+UseG1GC -XX:-UseAdaptiveSizePolicy -XX:-OmitStackTraceInFastThrow -";

    launchlog.write("启动命令: " + LaunchCommand);

#pragma endregion

#pragma region 开始游戏
    system(LaunchCommand.data());
#pragma endregion

    return 0;
  }
}
