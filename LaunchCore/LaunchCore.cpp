#include "pch.h"
#include "LaunchCore.h"
#include "replace_all.h"
#include "Format.h"
#include "Log.h"
#include "quotation.h"


using namespace std;

namespace UnknownMinecraftLaunchCore {
  LaunchCore::LaunchCore(string _gamepath, Settings* _settings) {
    settings = _settings;
    gamepath = getcwd(NULL, 0) + _gamepath;
    Toolkits::replace_all(gamepath, "\\", "/");

    if (!launchlog->ofs) {
      this->Error = "无法打开日志 目录:";
    }
  }

  LaunchCore::~LaunchCore() {

  };

  int LaunchCore::Launch(string gameid) {

    launchlog->write("开始启动游戏");
    launchlog->write("游戏路径: " + Toolkits::Quotation(gamepath));

    //启动参数
    string LaunchCommand = "${JavaPath} ";

#pragma region 读取版本Json文件

    ifstream infile;
    string file = gamepath + "/versions/" + gameid + "/" + gameid + ".json";
    launchlog->write("版本Json路径: " + Toolkits::Quotation(file));
    infile.open(file);
    if (!infile) {
      launchlog->write("Error: 不合法的游戏路径");
      launchlog->write("Exit");
      return 0;
    }

    infile.seekg(0, ios::end);
    int length = infile.tellg();
    infile.seekg(0, ios::beg);

    char* in = new char[length];

    infile.read(in, length);
    infile.close();

    Json::Reader reader;
    Json::Value root;

    if (reader.parse(in, root)) {
      launchargument->MainClass = root["mainClass"].asString();
      launchargument->MinecraftArguments = root["minecraftArguments"].asString();

      if (root["arguments"]) {
        int i;
        if (root["arguments"]) {
          for (i = 0; i < root["arguments"]["game"].size(); i++) {
            if (root["arguments"]["game"][i]["rules"]) {
              bool IsApplicable;
              auto rules = root["arguments"]["game"][i]["rules"];
              int j;
              if (IsApplicable and root["arguments"]["game"][i]["rules"]["action"].asString() != "allow") IsApplicable = false;
              if (IsApplicable and root["arguments"]["game"][i]["rules"]["os"]["name"].asString() != this->os->name) IsApplicable = false;
              if (IsApplicable and root["arguments"]["game"][i]["rules"]["os"]["version"]) {
                regex ver_reg(root["arguments"]["game"][i]["rules"]["os"]["version"].asString());
                smatch result;
                if (!regex_match(this->os->version, result, ver_reg)) {
                  IsApplicable = false;
                }
              }
              if (IsApplicable and root["arguments"]["game"][i]["rules"]["os"]["arch"].asString() != this->os->arch) IsApplicable = false;

              if (IsApplicable) {
                int k;
                for (k = 0; k < root["arguments"]["game"][i]["values"].size(); k++) {
                  launchargument->MinecraftArguments += root["arguments"]["game"][i]["values"]["k"].asString();
                }
              }
            }
            else {
              launchargument->MinecraftArguments += root["arguments"]["game"][i].asString();
            }
          }

        }
      }

      int i;
      for (i = 0; i < root["libraries"].size(); i++) {
        if (root["libraries"][i]["downloads"]["artifact"]) {
          launchargument->ClassPath += gamepath + "/libraries/" + root["libraries"][i]["downloads"]["artifact"]["path"].asString() + ";";
        }
        else if (root["libraries"][i]["downloads"]["classifiers"]) {
          //string j = root["libraries"][i].asString();
          string natives = root["libraries"][i]["natives"]["windows"].asString();
          Toolkits::replace_all(natives, "${arch}", "64");
          launchargument->ClassPath += gamepath + "/libraries/" + root["libraries"][i]["downloads"]["classifiers"][natives]["path"].asString() + ";";
        }
      }
      launchargument->ClassPath += gamepath + "/versions/" + gameid + "/" + gameid + ".jar";
    }
    else {
      launchlog->write("Error: 无法读取的版本Json文件");
      launchlog->write("Exit");
      return 0;
    }

    delete(in);
#pragma endregion

    if (launchargument->Arguments.empty()) {
      LaunchCommand += " -Djava.library.path=${natives_directory} -XX:+UseG1GC -XX:-UseAdaptiveSizePolicy -XX:-OmitStackTraceInFastThrow -XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump -Xmx${Xmx} -Xmn${Xmn}  -cp ${classpath} -jar ${jar} ${mainClass} " + launchargument->MinecraftArguments;
    }
    else {
      LaunchCommand += launchargument->Arguments + launchargument->MinecraftArguments;
    }

    map<string, string> m;

    m.insert(make_pair("${JavaPath}", Toolkits::Quotation(settings->jvm->JavaPath)));//Java
    m.insert(make_pair("${Xmx}", settings->jvm->Xmx));//Xmx
    m.insert(make_pair("${Xms}", settings->jvm->Xms));//Xms
    m.insert(make_pair("${Xmn}", settings->jvm->Xmn));//Xmn
    //m.insert(make_pair( "${jar}", Toolkits::quotation(gamepath + "/versions/" + gameid + "/" + gameid + "->jar")));//jar
    m.insert(make_pair("${jar}", Toolkits::Quotation(R"(C:\Users\55343\AppData\Roaming\PCL\JavaWrapper->jar)")));//jar
    m.insert(make_pair("${natives_directory}", Toolkits::Quotation(gamepath + "/versions/" + gameid + "/" + gameid + "-natives")));
    m.insert(make_pair("${classpath}", Toolkits::Quotation(launchargument->ClassPath)));
    m.insert(make_pair("${mainClass}", launchargument->MainClass));//版本名
    m.insert(make_pair("${version_name}", gameid));//版本名
    m.insert(make_pair("${auth_player_name}", settings->auth->Name));//玩家名
    m.insert(make_pair("${assets_root}", Toolkits::Quotation(gamepath + "/assets")));//assets路径
    m.insert(make_pair("${assets_index_name}", gameid));
    m.insert(make_pair("${game_directory}", Toolkits::Quotation(gamepath)));//游戏路径
    m.insert(make_pair("${user_type}", settings->auth->UserType));
    m.insert(make_pair("${auth_uuid}", settings->auth->UUID));
    m.insert(make_pair("${auth_access_token}", settings->auth->Token));
    m.insert(make_pair("${user_properties}", "{}"));

    Toolkits::replace_all(LaunchCommand, m);
    if (settings->gamewindow->IsFullscreen) LaunchCommand += "--fullscreen";//全屏


    launchlog->write("启动命令: " + LaunchCommand);

    ofstream of;
    of.open("start.bat", ios::ate);
    of << LaunchCommand;

#pragma region 开始游戏
    system("start.bat");
#pragma endregion

    return 0;
  }
}
