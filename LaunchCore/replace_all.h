#pragma once
#include "pch.h"

using namespace std;
namespace UnknownMinecraftLaunchCore {
  namespace Toolkits {
    string& replace_all(string& src, const string& old_value, const string& new_value) {
      // ÿ�����¶�λ��ʼλ�ã���ֹ�����滻����ַ����γ��µ�old_value
      for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
        if ((pos = src.find(old_value, pos)) != string::npos) {
          src.replace(pos, old_value.length(), new_value);
        }
        else break;
      }
      return src;
    }
  }
}
