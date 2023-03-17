#pragma once
#include "pch.h"

#ifdef DLLI
#define DLLI __declspec(dllexport)
#else 
#define DLLI __declspec(dllimport)
#endif

#include "RandomUUID.h"


using namespace std;

namespace MyLaunchCore {

  //extern "C" DLLI class LaunchCore;
  //extern "C" DLLI class Settings;
  //extern "C" DLLI class OfflineAuth;
  //extern "C" DLLI class MicrosoftAuth;
  //extern "C" DLLI class Settings;
  //extern "C" DLLI class Server;
  //extern "C" DLLI class GameWindow;
  //extern "C" DLLI class JVM;
  //extern "C" DLLI class Auth;

#pragma region 验证

  extern "C" DLLI class OfflineAuth {
  public:
    OfflineAuth(string _name) {
      UUID = Toolkits::RandomUUID();
      Name = _name;
    }
    string Name;
    string UUID;
  };

  extern "C" DLLI class MicrosoftAuth {
  public:
    MicrosoftAuth() {

    }

    string Name;
    string UUID;
  };

#pragma endregion


#pragma region Settings
  extern "C" DLLI class Auth {
  public:
    Auth() {

    }

    ~Auth() {

    }

    string Name;
    string UUID;
  };

  extern "C" DLLI class Jvm {
  public:
    Jvm(int _xmx, int _xms, string _javapath) {
      this->Xmx = std::to_string(_xmx);
      this->Xms = std::to_string(_xms);
      this->JavaPath = _javapath;
      //std::to_chars(&Xmn, &Xmn + sizeof(Xmn), (_xmx.getNumericValue() * 0.375));
      this->Xmn = to_string(_xmx * 0.375);
    }

    Jvm() {

    }

    ~Jvm() {

    }

    string Xmx;
    string Xms;
    string Xmn;
    string JavaPath;
  private:
  };

  extern "C" DLLI class GameWindow {
  public:
    GameWindow(int _width, int _height, bool _isfullscreen) {
      Width = std::to_string(_width).data();
      Height = std::to_string(_height).data();
      IsFullscreen = _isfullscreen;
    }

    GameWindow() {

    }

    string Width;
    string Height;
    bool IsFullscreen;
  };

  extern "C" DLLI class Server {
  };

  extern "C" DLLI class Settings {
  public:
    Settings(Auth _auth, Jvm _jvm, GameWindow _gamewindow, Server _server) {
      auth.Name = _auth.Name;
      auth.UUID = _auth.UUID;
    }

    Settings() {

    }

    ~Settings() {

    }

    Auth auth;
    Jvm jvm;
    GameWindow gamewindow;
    Server server;
  };
#pragma endregion

  class LaunchArgument {
  public:
    string Name;
    string Version;
    string Gamedir;
    string AssetsDir;
    string MinecraftArguments;
  };

  extern "C" DLLI class LaunchCore {
  public:
    LaunchCore(string _gamepath, Settings _settings);
    ~LaunchCore();
    int Launch(string gameid);

    string Error;

  private:
    Settings settings;
    string gamepath;
    LaunchArgument launchargument;
  };
}
