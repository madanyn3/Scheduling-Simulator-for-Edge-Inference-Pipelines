// top level module encapsulation all vector int/float operations
// Implements AVX, AVX2, AVX512, FMA instructions guarded by runtime cpu knobs

#ifndef FP_MODULE_HPP
#define FP_MODULE_HPP

#include<vector>
#include "simLib/device.hpp"
#include "simLib/simpleLink.hpp"
#include "fpTypes.hpp"

class c_fpModule: c_Device {
  public:
    c_fpModule();

  private:
    std::vector<c_SimpleLink<c_fpInstr>> m_input;
    
}

#endif // FP_MODULE_HPP