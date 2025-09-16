// top level class for all NPU compute tile operations
// Encapsulates the vector processor, program memory and data memory

#ifndef NPU_COMPUTETILE_HPP
#define NPU_COMPUTETILE_HPP

#include "simLib/device.hpp"

class c_computeTile: public c_Device {
  public:
    c_computeTile();

  private:
    c_vectorProcessor m_vp;
    c_cache m_programMem;
    c_cache m_dataMem;
    c_vector<c_SimpleLink<c_npuInstr>> m_inputLinks;
}

#endif // NPU_COMPUTETILE_HPP