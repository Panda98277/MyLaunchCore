#pragma once
#include "pch.h"
#include "GetTime.h"


using namespace std;

namespace MyLaunchCore {
  class Log {
  public:
    Log() {
      string file = getcwd(NULL, 0); 
      file += R"(\Log\log.txt)";
      ofs.open(file, ios::app | ios::out);
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