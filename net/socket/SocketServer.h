#ifndef SOCKETSERVERINTERFACE_H
#define SOCKETSERVERINTERFACE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>
#include <netinet/in.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


class SocketConnection;

class SocketServer {
public:
    SocketServer(const SocketServer&) = delete;
    SocketServer& operator=(const SocketServer&) = delete;

    virtual void listen(in_port_t port, int backlog, const std::function<void(int err)>& onError) = 0;

    virtual void disconnect(SocketConnection* cs) = 0;

    static void start();
    static void stop();

protected:
    SocketServer() = default;
};

#endif // SOCKETSERVERINTERFACE_H
