// implementation of cache modules
#include<cstdint>
#include"cache.hpp"

template<typename T>
c_baseCache::c_baseCache (std::size_t x_size, std::size_t x_latency, c_Bus<T, c_baseCache, Treq, c_cacheResp* x_bus): m_bus(x_bus) {
    m_size = x_size;
    m_latency = x_latency;
    m_occupancy = 0;

    int l_lineSize = 64;
    assert((m_size % 64) == 0);
    int l_numLines = (m_size / 64);

    m_array = new c_CacheLine[l_numLines];
}

template<typename T>
int8_t c_baseCache::getWay (uint64_t x_addr) {
    // returns 0 if line is present in the cache
    // returns -1 otherwise;
    for (int idx = 0; idx < m_array.size(); i++) {
        if (m_array[idx].m_tag == x_addr) return 0;
    }
    return -1;
}

template<typename T>
void c_baseCache::access (s_cacheReq x_req) {
    c_bufferEnrty l_entry = new c_bufferEnrty<s_cacheReq>(m_nextReqId, x_req, simClock());
    m_nextReqId++;
}

template<typename T>
void c_baseCache::addLine (uint64_t x_addr) {
    if (m_array.size() == m_size) {
        int oldestLine = 0;
        uint64_t age = simClock() - m_array[0].m_lastUsed;
        for (int idx = 1; idx < m_array.size(); idx++) {
            if ((simClock() - m_array[idx].m_lastUsed) > age) oldestLine = idx;
        }
        m_array[oldestLine] = c_cacheLine(x_addr, simClock());
        return;
    }
    m_array.push_back(c_cacheLine(x_addr, simClock()));
    return;
}

template<typename T>
c_cacheLine c_baseCache::read (uint64_t x_addr) {
    for (int idx = 0; idx < m_array.size(); idx++) {
        if (m_array[idx].m_tag == x_addr) {
            m_array[idx].m_lastUsed = simClock();
            return m_array[idx];
        }
    }
    return c_cacheLine(0, 0, false);
}

template<typename T>
void c_baseCache::write (uint64_t x_addr, c_cacheLine x_line) {
    for (int idx = 0; idx < m_array.size(); idx++) {
        if (m_array[idx].m_tag == x_addr) {
            m_array[idx].m_lastUsed = simClock();
            return;
        }
    }
    addLine(x_addr);
    return;
}

// a req will be picked up from the m_reqBuffer only after m_latency/2 of its arrival
template<typename T>
void c_baseCache::processReq () {
    if (m_reqBuffer.front().m_arrivalTime <= (simClock() - (m_latency/2))) {
        s_cacheReq l_req = m_array.front().m_req;
        int8_t l_way = getWay(l_req.addr);
        e_ReqType l_reqType = l_req.reqType;
        if (l_reqType == e_ReqType::READ) {
            if (l_way >= 0 ) {
                c_cacheLine l_line = read(l_req.addr);
                m_respBuffer.push_back(s_cacheResp(l_req.addr, 0, l_reqType, e_CacheLineStatus::HIT, l_line));
            }
            else {
                m_respBuffer.push_back(s_cacheReqp(l_req.addr, -1, l_reqType, e_CacheLineStatus::MISS, c_cacheLine(0, 0, false)));
            }
        }
        else if (l_reqType == e_ReqType::WRITE) {
            write(l_req.addr, l_req.line);
            if (l_way >= 0 ) {
                m_respBuffer.push_back(s_cacheResp(l_req.addr, 0, l_reqType, e_CacheLineStatus::HIT, l_req.line));
            }
            else {
                m_respBuffer.push_back(s_cacheResp(l_req.addr, -1, l_reqType, e_CacheLineStatus::MISS, l_req.line));   
            }
        }
    }
}

template<typename T>
void c_baseCache::readInputs () {
    int8_t l_picked = 0;
    while (m_bus->hasReq() && (l_picked < 2)) {
        s_cacheReq l_req = m_bus->getNextReq();
        m_reqBuffer.push_back(l_req);
        l_picked++;
    }
}

template<typename T>
void c_baseCache::writeOutputs () {
    int8_t l_picked = 0;
    while ((m_respBuffer.size() > 0) && (l_picked < 2)) {
        auto l_resp = m_respBuffer.front();
        if (l_resp.m_arrivalTime <= (simClock() - m_latency)) {
            m_bus->sendToSrc(l_resp);
            l_picked++;
        }
    }
}

// run funtion for every cycle operation
template<typename T>
void c_baseCache::run () {

    readInputs();
    processRes();
    writeOutputs();
    tick();
}