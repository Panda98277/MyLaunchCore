#pragma once
#include "pch.h"
#include "GetTime.h"


using namespace std;

namespace MyLaunchCore {
  class Log {
  public:
    Log() {
      char s[MAX_PATH];
      getcwd(s, MAX_PATH); 
      string file = s;
      ofs.open(file + R"(\Log\log.txt)", ios::app | ios::out);
      if (!ofs) {
        ofs.close();
        mkdir((file + "\\Log").data());
        ofs.open(file + R"(\Log\log.txt)", ios::app | ios::out);
      }
      
    }
    ~Log() {
      ofs.close();
    }
    ofstream ofs;
    void write(string _log) {
      ofs/* << Toolkits::GetTime()*/ << _log << endl;
    }
  };
}