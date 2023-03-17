#pragma once
#include "pch.h"
#include "GetTime.h"


using namespace std;

namespace MyLaunchCore {
  class Log {
  public:
    
    Log() {
      ofs.open(R"(Log\log.txt)", ios::app);
    }

    ~Log() {
      ofs.close();
    }
    ofstream ofs;
    int write(string _log) {
      ofs << Toolkits::GetTime() << _log << endl;
    }
  };
}