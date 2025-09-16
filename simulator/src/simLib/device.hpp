// base device class for all the simModules
// implements the basic IO structures and functions
// all devices should inherit from this class

#ifndef SIMLIB_DEVICE_HPP
#define SIMLIB_DEVICE_HPP

#include<cstdint>

class c_Device{
  public:
    c_Device(){
        m_simClock = 0;
    };
    virtual ~c_Device(){};

    virtual void init() = 0;
    virtual void run()  = 0;
    virtual void end()  = 0;
    void tick() { m_simClock++; }
    static inline uint64_t simClock() { return m_simClock; }

  protected:
    uint8_t deviceID;
    uint8_t deviceType;
    static uint64_t m_simClock;

}


#endif // SIMLIB_DEVICE_HPP