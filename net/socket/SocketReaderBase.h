#ifndef SOCKETREADERBASE_H
#define SOCKETREADERBASE_H

#include "Reader.h"


class SocketConnection;

class SocketReaderBase : public Reader {
public:
    void readEvent();

protected:
    SocketReaderBase()
        : readProcessor(0) {
    }

    SocketReaderBase(const std::function<void(::SocketConnection* cs, const char* junk, ssize_t n)>& readProcessor,
                     const std::function<void(int errnum)>& onError)
        : Reader(onError)
        , readProcessor(readProcessor) {
    }

    virtual ssize_t recv(char* junk, const ssize_t& junkSize) = 0;

    std::function<void(::SocketConnection* cs, const char* junk, ssize_t n)> readProcessor;
};

#endif // SOCKETREADERBASE_H