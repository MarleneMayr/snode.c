#ifndef SSLCONNECTEDSOCKET_H
#define SSLCONNECTEDSOCKET_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "socket/SocketConnectionBase.h"
#include "socket/tls/SocketReader.h"
#include "socket/tls/SocketWriter.h"


namespace tls {

    class SocketConnection : public SocketConnectionBase<tls::SocketReader, tls::SocketWriter> {
    public:
        SocketConnection(int csFd, ::SocketServer* ss,
                         const std::function<void(::SocketConnection* cs, const char* junk, ssize_t n)>& readProcessor,
                         const std::function<void(::SocketConnection* cs, int errnum)>& onReadError,
                         const std::function<void(::SocketConnection* cs, int errnum)>& onWriteError);
    };

}; // namespace tls

#endif // SSLCONNECTEDSOCKET_H