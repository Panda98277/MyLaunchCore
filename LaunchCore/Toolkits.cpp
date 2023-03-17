#include "Toolkits.h"

Toolkits::Toolkits(void) {
}

char* Toolkits::StringToChar(System::String^ str) {
  return (char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str)).ToPointer();
}

Toolkits::~Toolkits(void) {
}
