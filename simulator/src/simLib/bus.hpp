// template class for a two-way bus

template <typename Ts, typename Td, typename Treq, typename Trsp>
class c_Bus {
    public:
        void connect(Ts* src, Td* dest) {
            m_src = src;
            m_dest = dest;
        }

        void sendToDst(const Treq& req) {
            reqQ.push(req);
        }

        bool hasReq() const { return !reqQ.empty(); }

        Treq getNextReq() {
            auto r = reqQ.front();
            reqQ.pop();
            return r;
        }

        void sendToSrc(const Trsp& resp) {
            respQ.push(resp);
        }
        bool hasResp() const { return !respQ.empty(); }

        Trsp getNextResp() {
            auto r = respQ.front();
            respQ.pop();
            return r;
        }

    private:
        Ts* src = nullptr;
        Td* dst = nullptr;
        std::queue<Treq> reqQ;
        std::queue<Trsp> rspQ;

}