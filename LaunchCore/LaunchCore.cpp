#include "pch.h"
#include "LaunchCore.h"
#include "replace_all.h"
#include "Format.h"
#include "Log.h"
#include "quotation.h"


using namespace std;

namespace UnknownLaunchCore {
  LaunchCore::LaunchCore(string _gamepath, Settings _settings) {
    settings = _settings;
    gamepath = getcwd(NULL, 0) + _gamepath;
    Toolkits::replace_all(gamepath, "\\", "/");

    if (!launchlog.ofs) {
      this->Error = "�޷�����־ Ŀ¼:";
    }
  }

  LaunchCore::~LaunchCore() {

  };

  int LaunchCore::Launch(string gameid) {

    launchlog.write("��ʼ������Ϸ");
    launchlog.write("��Ϸ·��: " + Toolkits::Quotation(gamepath));

    //��������
    string LaunchCommand = "${JavaPath} ";

#pragma region ��ȡ�汾Json�ļ�

    ifstream infile;
    string file = gamepath + "/versions/" + gameid + "/" + gameid + ".json";
    launchlog.write("�汾Json·��: " + Toolkits::Quotation(file));
    infile.open(file);
    if (!infile) {
      launchlog.write("Error: ���Ϸ�����Ϸ·��");
      launchlog.write("Exit");
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
      launchargument.MainClass = root["mainClass"].asString();
      if (root["minecraftArguments"]) {
        launchargument.MinecraftArguments = root["minecraftArguments"].asString();
      }
      else if (root["arguments"]["arguments"]) {
        int i;
        for (i = 0; i < root["arguments"]["game"].size(); i++) {
          if (root["arguments"]["arguments"]["game"][i]) {
            if (root["arguments"]["arguments"]["game"][i]["rule"]) {
              
            }
          }
        }
      }
      else {
        launchlog.write("Error: �޷���ȡ�İ汾Json�ļ�");
        launchlog.write("Exit");
        return 0;
      }
      int i;
      for (i = 0; i < root["libraries"].size(); i++) {
        //if (root["libraries"][i]["downloads"]["artifact"]) {
        //  launchargument.ClassPath += gamepath + "/libraries/" + root["libraries"][i]["downloads"]["artifact"]["path"].asString() + ";";
        //}
        //else if (root["libraries"][i]["downloads"]["classifiers"]) {
        //  if (root["libraries"][i]["downloads"]["classifiers"]["natives-windows-64"])
        //    launchargument.ClassPath += gamepath + "/libraries/" + root["libraries"][i]["downloads"]["classifiers"]["natives-windows-64"]["path"].asString() + ";";
        //  else if (root["libraries"][i]["downloads"]["classifiers"]["natives-window"])
        //    launchargument.ClassPath += gamepath + "/libraries/" + root["libraries"][i]["downloads"]["classifiers"]["natives-windows-64"]["path"].asString() + ";";
        //}
        if (root["libraries"][i]["downloads"]["artifact"]) {
          launchargument.ClassPath += gamepath + "/libraries/" + root["libraries"][i]["downloads"]["artifact"]["path"].asString() + ";";
        }
        else if (root["libraries"][i]["downloads"]["classifiers"]) {
          //string j = root["libraries"][i].asString();
          string natives = root["libraries"][i]["natives"]["windows"].asString();
          Toolkits::replace_all(natives, "${arch}", "64");
          launchargument.ClassPath += gamepath + "/libraries/" + root["libraries"][i]["downloads"]["classifiers"][natives]["path"].asString() + ";";
        }
      }
      launchargument.ClassPath += gamepath + "/versions/" + gameid + "/" + gameid + ".jar";
    }
    else {
      launchlog.write("Error: �޷���ȡ�İ汾Json�ļ�");
      launchlog.write("Exit");
      return 0;
    }

    delete(in);
#pragma endregion

    LaunchCommand += " -Djava.library.path=${natives_directory} -XX:+UseG1GC -XX:-UseAdaptiveSizePolicy -XX:-OmitStackTraceInFastThrow -XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump -Xmx${Xmx} -Xmn${Xmn}  -cp ${classpath} --add-exports cpw.mods.bootstraplauncher/cpw.mods.bootstraplauncher=ALL-UNNAMED -jar ${jar} ${mainClass} " + launchargument.MinecraftArguments;

#pragma region JVM

    Toolkits::replace_all(LaunchCommand, "${JavaPath}", Toolkits::Quotation(settings.jvm.JavaPath));//Java
    Toolkits::replace_all(LaunchCommand, "${Xmx}", settings.jvm.Xmx);//Xmx
    Toolkits::replace_all(LaunchCommand, "${Xms}", settings.jvm.Xms);//Xms
    Toolkits::replace_all(LaunchCommand, "${Xmn}", settings.jvm.Xmn);//Xmn
    //Toolkits::replace_all(LaunchCommand, "${jar}", Toolkits::quotation(gamepath + "/versions/" + gameid + "/" + gameid + ".jar"));//jar
    Toolkits::replace_all(LaunchCommand, "${jar}", Toolkits::Quotation(R"(C:\Users\55343\AppData\Roaming\PCL\JavaWrapper.jar)"));//jar
    Toolkits::replace_all(LaunchCommand, "${natives_directory}", Toolkits::Quotation(gamepath + "/versions/" + gameid + "/" + gameid + "-natives"));
    Toolkits::replace_all(LaunchCommand, "${classpath}", Toolkits::Quotation(launchargument.ClassPath));

#pragma endregion

#pragma region Game Arguments
    Toolkits::replace_all(LaunchCommand, "${mainClass}", launchargument.MainClass);//�汾��
    Toolkits::replace_all(LaunchCommand, "${version_name}", gameid);//�汾��
    Toolkits::replace_all(LaunchCommand, "${auth_player_name}", settings.auth.Name);//�����
    Toolkits::replace_all(LaunchCommand, "${assets_root}", Toolkits::Quotation(gamepath + "/assets"));//assets·��
    Toolkits::replace_all(LaunchCommand, "${assets_index_name}", gameid);
    Toolkits::replace_all(LaunchCommand, "${game_directory}", Toolkits::Quotation(gamepath));//��Ϸ·��
    Toolkits::replace_all(LaunchCommand, "${user_type}", settings.auth.UserType);
    Toolkits::replace_all(LaunchCommand, "${auth_uuid}", settings.auth.UUID);
    Toolkits::replace_all(LaunchCommand, "${auth_access_token}", settings.auth.Token);
    Toolkits::replace_all(LaunchCommand, "${user_properties}", "{}");


    if (settings.gamewindow.IsFullscreen) LaunchCommand += "--fullscreen";//ȫ��
#pragma endregion

    launchlog.write("��������: " + LaunchCommand);

    ofstream of;
    of.open("start.bat",ios::ate);
    of << LaunchCommand;

#pragma region ��ʼ��Ϸ
    system("start.bat");
#pragma endregion

    return 0;
  }
}
