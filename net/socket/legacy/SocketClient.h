#ifndef LEGACY_SOCKETCLIENT_H
#define LEGACY_SOCKETCLIENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "socket/SocketClient.h"
#include "socket/legacy/SocketConnection.h"

namespace legacy {

    class SocketClient : public ::SocketClient<legacy::SocketConnection> {
    public:
        using ::SocketClient<legacy::SocketConnection>::SocketClient;
    };

} // namespace legacy

#endif // LEGACY_SOCKETCLIENT_H