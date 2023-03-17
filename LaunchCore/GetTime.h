#pragma once
#include "pch.h"
#include "Format.h"


using namespace std;

namespace MyLaunchCore {
  namespace Toolkits {
    string GetTime() {
      LPSYSTEMTIME system;
      GetLocalTime(system);
      return Format("[{0}:{1}:{2}:{3}:{4}]",system->wYear,system->wMonth,system->wDay,system->wHour,system->wMinute);
    }
  }
}