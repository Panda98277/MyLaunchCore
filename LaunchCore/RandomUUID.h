#pragma warning(suppress : 4996)
#pragma once
#include "pch.h"


using namespace std;


namespace MyLaunchCore {
  namespace Toolkits {
    static string RandomUUID() {
      stringstream stream;
      auto random_seed = std::chrono::system_clock::now().time_since_epoch().count();
      mt19937 seed_engine(random_seed);
      uniform_int_distribution<std::size_t> random_gen;
      size_t value = random_gen(seed_engine);
      stream << hex << value;

      return stream.str();
    }
  }
}