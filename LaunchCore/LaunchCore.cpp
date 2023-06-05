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


    JsonReader* jsonreader = new JsonReader(this);
    jsonreader->Read(gameid);
    
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
    m.insert(make_pair("${jar}", Toolkits::Quotation(R"(C:\Users\55343\AppData\Roaming\PCL\JavaWrapper.jar)")));//jar
    m.insert(make_pair("${natives_directory}", Toolkits::Quotation(gamepath + "/versions/" + gameid + "/" + gameid + "-natives")));
    m.insert(make_pair("${classpath}", Toolkits::Quotation(launchargument->ClassPath)));
    m.insert(make_pair("${mainClass}", launchargument->MainClass));//版本名
    m.insert(make_pair("${version_name}", gameid));//版本名
    m.insert(make_pair("${auth_player_name}", settings->auth->Name));//玩家名
    m.insert(make_pair("${assets_root}", Toolkits::Quotation(gamepath + "/assets")));//assets路径
    m.insert(make_pair("${assets_index_name}", launchargument->AssetIndex));
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
    of.close();

#pragma region 开始游戏
    system("cmd.exe /c start start.bat");
#pragma endregion

    return 0;
  }

  LaunchCore::JsonReader::JsonReader(LaunchCore* _lc) {
    this->lc = _lc;
    this->ruler = new Ruler(lc);
  }

  LaunchCore::JsonReader::~JsonReader() {

  }

  int LaunchCore::JsonReader::Read(string gameid) {
    try{
      ifstream infile;
      string file = lc->gamepath + "/versions/" + gameid + "/" + gameid + ".json";
      lc->launchlog->write("版本Json路径: " + Toolkits::Quotation(file));
      infile.open(file);
      if (!infile) {
        lc->launchlog->write("Error: 不合法的游戏路径");
        lc->launchlog->write("Exit");
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
        lc->launchargument->MainClass = root["mainClass"].asString();
        lc->launchargument->AssetIndex = root["assetIndex"]["id"].asString();
        if (!root["minecraftArguments"].isNull()) lc->launchargument->MinecraftArguments = root["minecraftArguments"].asString();

        int i;
        if (root["arguments"]) {
          if (root["arguments"]["game"]) {
            for (i = 0; i < root["arguments"]["game"].size(); i++) {
              if (root["arguments"]["game"][i].isString()) {
                lc->launchargument->MinecraftArguments += " " + root["arguments"]["game"][i].asString();
              }
              else {
                bool IsApplicable = true;
                auto rule = root["arguments"]["game"][i]["rules"][0];
                IsApplicable =  this->ruler->Rule(rule);
                if (IsApplicable) {
                  int k;
                  for (k = 0; k < root["arguments"]["game"][i]["values"].size(); k++) {
                    lc->launchargument->MinecraftArguments += " " + root["arguments"]["game"][i]["values"]["k"].asString();
                  }
                }
              }
            }
          }
        }
        for (i = 0; i < root["libraries"].size(); i++) {
          if (root["libraries"][i]["downloads"]["artifact"]) {
            bool IsApplicable = true;
            if (!root["libraries"][i]["rules"].isNull()) {
              auto rule = root["libraries"][i]["rules"][0];
              IsApplicable = ruler->Rule(rule);
            }

            if (IsApplicable) {
              string path = lc->gamepath + "/libraries/" + root["libraries"][i]["downloads"]["artifact"]["path"].asString() + ";";

              lc->launchargument->ClassPath += path;

            }
          }
          else if (!root["libraries"][i]["downloads"]["classifiers"].isNull()) {
            //string j = root["libraries"][i].asString();          
            string natives = root["libraries"][i]["natives"]["windows"].asString();
            Toolkits::replace_all(natives, "${arch}", "64");
            lc->launchargument->ClassPath += lc->gamepath + "/libraries/" + root["libraries"][i]["downloads"]["classifiers"][natives]["path"].asString() + ";";
          }
        }
        lc->launchargument->ClassPath += lc->gamepath + "/versions/" + gameid + "/" + gameid + ".jar";
      }
      else {
        lc->launchlog->write("Error: 无法读取的版本Json文件");
        lc->launchlog->write("Exit");
        return 0;
      }

      delete(in);
    }
    catch(...){
      return -1;
    }
    return 0;
  }

  LaunchCore::JsonReader::Ruler::Ruler(LaunchCore* _lc) {
    this->lc = _lc;
  }

  LaunchCore::JsonReader::Ruler::~Ruler() {

  }

  bool LaunchCore::JsonReader::Ruler::Rule(Json::Value root) {
    bool IsApplicable = true;
    auto rule = root;
    if (IsApplicable and !rule["action"].isNull() and rule["action"].asString() != "allow") IsApplicable = false;
    //string a = rule["os"]["name"].asString();
    if (IsApplicable and !rule["os"]["name"].isNull() and rule["os"]["name"].asString() != lc->os->name) IsApplicable = false;
    if (IsApplicable and !rule["os"]["version"].isNull()) {
      regex ver_reg(rule["os"]["version"].asString());
      smatch result;
      if (!regex_match(lc->os->version, result, ver_reg)) {
        IsApplicable = false;
      }
    }
    string a = rule["os"]["arch"].asString();
    if (!a.empty()) {
      DebugBreak();
    }
    if (IsApplicable and !rule["os"]["arch"].isNull() and rule["os"]["arch"].asString() != lc->os->arch) IsApplicable = false;
    
    return IsApplicable;
  }
}