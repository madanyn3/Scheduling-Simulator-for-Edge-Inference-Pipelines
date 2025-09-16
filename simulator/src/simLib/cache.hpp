// simple implementation of a cache
// base class models a fully associative cache with fixed latency
// approximate LRU replacement policy

#ifndef SIMULATOR_SRC_SIMLIB_CACHE_HPP
#define SIMULATOR_SRC_SIMLIB_CACHE_HPP

#include <cstddef>
#include <cstdint>
#include "simLib/device.hpp"
#include "simLib/simpleLink.hpp"

class c_cacheLine {
    public:
        c_cacheLine(uint64_t x_tag, uint64_t x_initTime, bool x_valid=1): m_tag(x_tag), m_lastUsed(x_initTime), m_valid(x_valid) {}
        uint64_t m_tag;
        uint64_t m_lastUsed; 
        bool m_valid;
}

struct s_cacheReq {
    ENUM {
        READ,
        WRITE,
        GET_WAY
    } e_ReqType;

    uint64_t addr;
    e_ReqType reqType;
}

struct s_cacheResp {
    ENUM {
        READ,
        WRITE,
        GET_WAY
    } e_ReqType;

    uint64_t addr;
    int8_t way;
    e_ReqType reqType;
    c_cacheLine line;
}

template <typename T>
class c_bufferEntry {
    public:
        c_bufferEntry(uint64_t x_reqId, T x_req, uint64_t x_arrivalTime) {
            m_reqId = x_reqId;
            m_req   = x_req;
            m_arrivalTime = x_arrivalTime;
        }
        uint64_t m_reqId;
        uint64_t m_arrivalTime;
        T m_req;
}

class c_baseCache: public c_Device {
    public:
        c_baseCache(std::size_t x_size, std::size_t x_latency);
        int8_t getWay(uint64_t addr); // backdoor lookup to check if a line hits in the cache, no real access;
        void access(s_cacheReq x_req);

    private:
        void addLine(uint64_t addr);
        c_cacheLine getLine(uint64_t addr);
        void processReq();
        void processResp();

        c_cacheLine* m_array;
        std::size_t m_size;
        std::size_t m_latency;
        std::vector<c_bufferEntry> m_reqBuffer;
        std::vector<c_bufferEnrty> m_respBuffer;
        uint64_t m_nextReqId;
        uint64_t m_occupancy;
}

#endif // SIMULATOR_SRC_SIMLIB_CACHE_HPP
