#pragma once
#include "pch.h"
#include "RandomUUID.h"
#include "Log.h"

#ifdef DLLI
#define DLLI __declspec(dllexport)
#else 
#define DLLI __declspec(dllimport)
#endif



using namespace std;

namespace UnknownMinecraftLaunchCore {

  //extern "C" DLLI class LaunchCore;
  //extern "C" DLLI class Settings;
  //extern "C" DLLI class OfflineAuth;
  //extern "C" DLLI class MicrosoftAuth;
  //extern "C" DLLI class Settings;
  //extern "C" DLLI class Server;
  //extern "C" DLLI class GameWindow;
  //extern "C" DLLI class JVM;
  //extern "C" DLLI class Auth;

#pragma region ÑéÖ¤

  extern "C" DLLI class OfflineAuth {
  public:
    OfflineAuth(string _name) {
      UUID = Toolkits::RandomUUID();
      Token = UUID;
      Name = _name;
    }

    string Token;
    string UserType = "Legacy";
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

    string Token;
    string UserType = "Legacy";
    string Name;
    string UUID;
  };

  extern "C" DLLI class Jvm {
  public:
    Jvm(int _xmx, int _xms, string _javapath) {
      this->Xmx = std::to_string(_xmx) + "M";
      this->Xms = std::to_string(_xms) + "M";
      this->JavaPath = _javapath;
      //std::to_chars(&Xmn, &Xmn + sizeof(Xmn), (_xmx.getNumericValue() * 0.375));
      this->Xmn = to_string(int(_xmx * 0.375)) + "M";
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
      auth->Name = _auth.Name;
      auth->UUID = _auth.UUID;
    }

    //Settings(OfflineAuth _auth, Jvm _jvm, GameWindow _gamewindow, Server _server) {
    //  auth.Name = _auth.Name;
    //  auth.UUID = _auth.UUID;
    //  jvm = _jvm;
    //  gamewindow = _gamewindow;
    //  server = _server;
    //}

    Settings(OfflineAuth* _auth, Jvm* _jvm, GameWindow* _gamewindow, Server* _server) {
      cout << _auth->Name;
      auth->Name = _auth->Name;
      auth->UUID = _auth->UUID;
      auth->UserType = _auth->UserType;
      auth->Token = _auth->Token;
      jvm = _jvm;
      gamewindow = _gamewindow;
      server = _server;
    }

    Settings() {

    }

    ~Settings() {

    }

    Auth* auth;
    Jvm* jvm;
    GameWindow* gamewindow;
    Server* server;
  };
#pragma endregion

  class LaunchArgument {
  public:
    string MinecraftArguments;
    string Arguments;
    string MainClass;
    string LibraryPath;
    string ClassPath;
    string Xmx;
    string Xmn;
    string Xms;
  };

  extern "C" DLLI class LaunchCore {
  public:
    DLLI LaunchCore(string _gamepath, Settings* _settings);
    DLLI ~LaunchCore();
    int DLLI Launch(string gameid);

    string Error;
    struct OS {
      string name = "windows";
      string version = "10.00";
      string arch = "x64";
    }* os;

    Log* launchlog;
    Settings* settings;
    string gamepath;
    LaunchArgument* launchargument;
  private:
  };
}
