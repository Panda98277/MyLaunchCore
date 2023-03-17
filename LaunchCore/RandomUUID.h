#pragma warning(suppress : 4996)
#pragma once
#include "pch.h"


namespace MyLaunchCore {
  namespace Toolkits {
    /*char* RandomUUID() {
      char a[16] = "123456789abcdef";
      char uuid[36] = "";
      int i;
      for (i = 0; i < 32; i++) {
        if (i == 7 or i == 11 or i == 15 or i == 19) {
          strncat(uuid, "-", 1);
        }
        switch (rand()%16) {
        case 0: strcat(uuid, "0"); break;
        case 1: strcat(uuid, "1"); break;
        case 2: strcat(uuid, "2"); break;
        case 3: strcat(uuid, "3"); break;
        case 4: strcat(uuid, "4"); break;
        case 5: strcat(uuid, "5"); break;
        case 6: strcat(uuid, "6"); break;
        case 7: strcat(uuid, "7"); break;
        case 8: strcat(uuid, "8"); break;
        case 9: strcat(uuid, "9"); break;
        case 10: strcat(uuid, "a"); break;
        case 11: strcat(uuid, "b"); break;
        case 12: strcat(uuid, "c"); break;
        case 13: strcat(uuid, "d"); break;
        case 14: strcat(uuid, "e"); break;
        case 15: strcat(uuid, "f"); break;
        }
      }
      return uuid;
    }*/
    char* RandomUUID();
  }
}