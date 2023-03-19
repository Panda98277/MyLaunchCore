#pragma warning(disable : 4996)
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <json/json.h>
#include <random>
#include <__msvc_chrono.hpp>
#include <fstream>
#include <LaunchCore.h>
#include <windows.h>
#include <Winuser.h>

using namespace std;
using namespace MyLaunchCore;

namespace Toolkits {
  class ArgBase {
  public:
    ArgBase() {}
    virtual ~ArgBase() {}
    virtual void Format(std::ostringstream& ss, const std::string& fmt) = 0;
  };

  template <class T>
  class Arg : public ArgBase {
  public:
    Arg(T arg) : m_arg(arg) {}
    virtual ~Arg() {}
    virtual void Format(std::ostringstream& ss, const std::string& fmt) {
      ss << m_arg;
    }
  private:
    T m_arg;
  };

  class ArgArray : public std::vector < ArgBase* > {
  public:
    ArgArray() {}
    ~ArgArray() {
      std::for_each(begin(), end(), [](ArgBase* p) { delete p; });
    }
  };

  static void FormatItem(std::ostringstream& ss, const std::string& item, const ArgArray& args) {
    int index = 0;
    int alignment = 0;
    std::string fmt;

    char* endptr = nullptr;
    index = strtol(&item[0], &endptr, 10);
    if (index < 0 || index >= args.size()) {
      return;
    }

    if (*endptr == ',') {
      alignment = strtol(endptr + 1, &endptr, 10);
      if (alignment > 0) {
        ss << std::right << std::setw(alignment);
      }
      else if (alignment < 0) {
        ss << std::left << std::setw(-alignment);
      }
    }

    if (*endptr == ':') {
      fmt = endptr + 1;
    }

    args[index]->Format(ss, fmt);

    return;
  }

  template <class T>
  static void Transfer(ArgArray& argArray, T t) {
    argArray.push_back(new Arg<T>(t));
  }

  template <class T, typename... Args>
  static void Transfer(ArgArray& argArray, T t, Args&&... args) {
    Transfer(argArray, t);
    Transfer(argArray, args...);
  }

  template <typename... Args>
  std::string Format(const std::string& format, Args&&... args) {
    if (sizeof...(args) == 0) {
      return format;
    }

    ArgArray argArray;
    Transfer(argArray, args...);
    size_t start = 0;
    size_t pos = 0;
    std::ostringstream ss;
    while (true) {
      pos = format.find('{', start);
      if (pos == std::string::npos) {
        ss << format.substr(start);
        break;
      }

      ss << format.substr(start, pos - start);
      if (format[pos + 1] == '{') {
        ss << '{';
        start = pos + 2;
        continue;
      }

      start = pos + 1;
      pos = format.find('}', start);
      if (pos == std::string::npos) {
        ss << format.substr(start - 1);
        break;
      }

      FormatItem(ss, format.substr(start, pos - start), argArray);
      start = pos + 1;
    }

    return ss.str();
  }
}


static std::string RandomUUID() {
  std::stringstream stream;
  auto random_seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 seed_engine(random_seed);
  std::uniform_int_distribution<std::size_t> random_gen;
  std::size_t value = random_gen(seed_engine);
  stream << std::hex << value;

  return stream.str();
}




namespace Toolkits {
  string& replace_all(char* a, const string& old_value, const string& new_value);
  string& replace_all(string& src, const string& old_value, const string& new_value);

  string& replace_all(string& src, const string& old_value, const string& new_value) {
    // 每次重新定位起始位置，防止上轮替换后的字符串形成新的old_value
    for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
      if ((pos = src.find(old_value, pos)) != string::npos) {
        src.replace(pos, old_value.length(), new_value);
      }
      else break;
    }
    return src;
  }

  string& replace_all(char* a, const string& old_value, const string& new_value) {

    string src;
    src = a;
    // 每次重新定位起始位置，防止上轮替换后的字符串形成新的old_value
    for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
      if ((pos = src.find(old_value, pos)) != string::npos) {
        src.replace(pos, old_value.length(), new_value);
      }
      else break;
    }
    return src;
  }
}



int main() {

  //char a[] = "shabi";
  //
  //LaunchCore launch(
  //  a,
  //  Settings{
  //    JVM{
  //      *"2048m",
  //      *"1024m",
  //      *(R"(C:\Program Files\Java\jre1.8.0_291\bin\javaw.exe)")
  //    },
  //
  //    GameWindow{
  //
  //    },
  //    Server{
  //
  //    }
  //  }
  //);

  // char gamepath[] = R"(H:\C C++ C#\LaunchCore\LaunchCore\x64\Debug\.minecraft)";
 // char gameid[] = R"(1.7.10)";
 // 
  //ifstream infile;
 // string file = Toolkits::Format(R"({0}\versions\\{1}\{1}.json)", gamepath, gameid);
 // infile.open(file);
 // 
 // infile.seekg(0, std::ios::end);
 // int length = infile.tellg();
 // infile.seekg(0, std::ios::beg);
 // 
 // char* in = new char[length];
 // 
 // infile.read(in, length);
 // 
 // //std::cout << in << std::endl;
 // 
 // Json::Reader reader;
 // Json::Value root;
 // 
 // if (reader.parse(in, root)) {
 //   std::cout << root["mainClass"].asString();
 // }
 // 
 // infile.close();
 // delete(in);
  //
  //OfflineAuth auth("shabi");
  //
  //Jvm jvm(512,1024, R"(C:\Users\55343\.jdks\corretto-18.0.2\bin\java.exe)");
  //
  //GameWindow gamewindow;
  //
  //Server server;
  //
  //Settings settings(auth,jvm,gamewindow,server);
  //
  //LaunchCore core("/" ,settings);

  LaunchCore core{
    R"(\.minecraft)",
    Settings{
      OfflineAuth{
        "shabi"
      },
      Jvm{
        512,
        1024,
        R"(C:\Users\55343\.jdks\corretto-18.0.2\bin\java.exe)"
      },
      GameWindow{
        800,
        600,
        false
      },
      Server{

      }
    }
  };
  core.Launch("1.7.10");

//  string json = R"(
//{
//  "libraries": [
//    {
//      "downloads": {
//        "artifact": {
//          "path": "com/mojang/netty/1.8.8/netty-1.8.8.jar",
//          "sha1": "0a796914d1c8a55b4da9f4a8856dd9623375d8bb",
//          "size": 15966,
//          "url": "https://libraries.minecraft.net/com/mojang/netty/1.8.8/netty-1.8.8.jar"
//        }
//      },
//      "name": "com.mojang:netty:1.8.8"
//    },
//    {
//      "downloads": {
//        "artifact": {
//          "path": "com/mojang/realms/1.3.5/realms-1.3.5.jar",
//          "sha1": "0807ae355ee63583becd7ea60e76aab1532bb42e",
//          "size": 353818,
//          "url": "https://libraries.minecraft.net/com/mojang/realms/1.3.5/realms-1.3.5.jar"
//        }
//      },
//      "name": "com.mojang:realms:1.3.5"
//    }
//  ]
//}
//  )";
//  Json::Reader reader;
//  Json::Value root;
//
//  if (reader.parse(json, root)) {
//
//    int i;
//    for (i = 0; i < root["libraries"].size(); i++) {
//      if (root["libraries"][i]["downloads"]["artifact"]) {
//        cout << root["libraries"][i]["downloads"]["artifact"]["path"] << endl;
//      }
//    }
//  }
//  else {
//    cout << endl;
//  }
}
