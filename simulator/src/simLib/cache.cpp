// implementation of cache modules

#include<cstdint>
#include "cache.hpp"

c_baseCache::c_baseCache (std::size_t x_size, std::size_t x_latency) {
    m_size = x_size;
    m_latency = x_latency;
    m_occupancy = 0;

    int l_lineSize = 64;
    assert((m_size % 64) == 0);
    int l_numLines = (m_size / 64);

    m_array = new c_CacheLine[l_numLines];
}

int8_t c_baseCache::getWay (uint64_t x_addr) {
    // returns 0 if line is present in the cache
    // returns -1 otherwise;
    for (int idx = 0; idx < m_array.size(); i++) {
        if (m_array[idx].m_tag == x_addr) return 0;
    }
    return -1;
}

void c_baseCache::access (s_cacheReq x_req) {
    c_bufferEnrty l_entry = new c_bufferEnrty<s_cacheReq>(m_nextReqId, x_req, simClock());
    m_nextReqId++;
}

void c_baseCache::addLine (uint64_t x_addr) {
    if (m_occupancy == m_array.size()) {
        int oldestLine = 0;
        uint64_t age = simClock() - m_array[0].m_lastUsed;
        for (int idx = 1; idx < m_array.size(); idx++) {
            if ((simClock() - m_array[idx].m_lastUsed) > age) oldestLine = idx;
        }
        m_array[oldestLine] = c_cacheLine(x_addr, simClock());
        return;
    }
    m_array[m_occupancy] = c_cacheLine(x_addr, simClock());
}

c_cacheLine c_baseCache::c_getLine (uint64_t x_addr) {
    for (int idx = 0; idx < m_occupancy; idx++) {
        if (m_array[idx].m_tag == x_addr) {
            m_array[idx].m_lastUsed = simClock();
            return m_array[idx];
        }
    }
    return c_cacheLine(0, 0, false);
}