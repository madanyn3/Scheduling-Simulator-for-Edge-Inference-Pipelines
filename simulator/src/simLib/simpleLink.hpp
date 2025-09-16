// simple link between two modules
// every link is a queue with FIFO policy
#ifndef SIMPLELINK_HPP
#define SIMPLELINK_HPP
#include<cstdint>

template <typename T>
class c_SimpleLink {
  public:
    c_SimpleLink(uint16_t x_size);
    ~c_SimpleLink();

    void canWrite();
    T read();
    void push_back(T x_packet);
    T pop_front();
    uint16_t size();
    bool isFull();

  private:
    uint16_t m_size;
    T* m_array;
    uint16_t m_head;
    uint16_t m_tail;
};

template <typename T>
c_SimpleLink<T>::c_SimpleLink(uint16_t x_size) {
    m_size = x_size;
    m_array = new T[m_size];
    m_head = 0;
    m_tail = 0;
}

template <typename T>
void
canWrite() {
    if (m_tail > m_tail) {
        return ((m_tail - m_head) < m_size);
    }
    else {
        if (m_head - m_tail == 1) return false;
        return true;
    }
}

template <typename T>
uint16_t
size() { return m_size; }

template <typename T>
bool
isFull() {return ((std::abs(m_head - m_tail) - m_size) == 0) }

template <typaname T>
T
read() {
    assert(m_array[m_head] != T{0});
    return m_array[m_head];
}

template <typename T>
T 
pop_front() {
    assert(m_array[m_head]);
    T l_packet = m_array[m_head];
    m_array[m_head] = T{0};
    m_head = (m_head + 1) % m_size;
    return l_packet;
}

template <typename T>
void
push_back(T x_packet) {
    assert(canWrite());
    assert(x_packet)
    m_array[m_tail] = x_packet;
    m_tail++;
}


#endif // SIMPLELINK_HPP