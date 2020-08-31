/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020  Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstdlib>
#include <easylogging++.h>
#include <functional>
#include <unistd.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "AcceptEventReceiver.h"
#include "Logger.h"
#include "ReadEventReceiver.h"
#include "Socket.h"

namespace net::socket {

    template <typename SocketConnection>
    class SocketServer
        : public AcceptEventReceiver
        , public Socket {
    public:
        void* operator new(size_t size) {
            SocketServer* serverSocket = reinterpret_cast<SocketServer*>(malloc(size));
            serverSocket->isDynamic = true;

            return serverSocket;
        }

        void operator delete(void* serverSocket_v) {
            SocketServer* serverSocket = reinterpret_cast<SocketServer*>(serverSocket_v);
            if (serverSocket->isDynamic) {
                free(serverSocket_v);
            }
        }

        SocketServer(const std::function<void(SocketConnection* socketConnection)>& onConnect,
                     const std::function<void(SocketConnection* socketConnection)>& onDisconnect,
                     const std::function<void(SocketConnection* socketConnection, const char* junk, ssize_t junkLen)>& onRead,
                     const std::function<void(SocketConnection* socketConnection, int errnum)>& onReadError,
                     const std::function<void(SocketConnection* socketConnection, int errnum)>& onWriteError)
            : AcceptEventReceiver()
            , Socket()
            , onConnect(onConnect)
            , onDisconnect(onDisconnect)
            , onRead(onRead)
            , onReadError(onReadError)
            , onWriteError(onWriteError) {
        }

        SocketServer() = delete;
        SocketServer& operator=(const SocketServer&) = delete;

        ~SocketServer() = default;

        void listen(const InetAddress& localAddress, int backlog, const std::function<void(int err)>& onError) {
            open([this, &localAddress, &backlog, &onError](int errnum) -> void {
                if (errnum > 0) {
                    onError(errnum);
                } else {
                    reuseAddress([this, &localAddress, &backlog, &onError](int errnum) -> void {
                        if (errnum != 0) {
                            onError(errnum);
                        } else {
                            bind(localAddress, [this, &backlog, &onError](int errnum) -> void {
                                if (errnum > 0) {
                                    onError(errnum);
                                } else {
                                    listen(backlog, [this, &onError](int errnum) -> void {
                                        if (errnum == 0) {
                                            AcceptEventReceiver::enable();
                                        }
                                        onError(errnum);
                                    });
                                }
                            });
                        }
                    });
                }
            });
        }

        void listen(in_port_t port, int backlog, const std::function<void(int err)>& onError) {
            listen(InetAddress(port), backlog, onError);
        }

        void listen(const std::string& ipOrHostname, uint16_t port, int backlog, const std::function<void(int err)>& onError) {
            listen(InetAddress(ipOrHostname, port), backlog, onError);
        }

        void acceptEvent() override {
            errno = 0;

            struct sockaddr_in remoteAddress {};
            socklen_t addrlen = sizeof(remoteAddress);

            int scFd = -1;

            scFd = ::accept4(getFd(), reinterpret_cast<struct sockaddr*>(&remoteAddress), &addrlen, SOCK_NONBLOCK);

            if (scFd >= 0) {
                struct sockaddr_in localAddress {};
                socklen_t addressLength = sizeof(localAddress);

                if (getsockname(scFd, reinterpret_cast<sockaddr*>(&localAddress), &addressLength) == 0) {
                    SocketConnection* socketConnection = SocketConnection::create(onRead, onReadError, onWriteError, onDisconnect);

                    socketConnection->open(scFd);

                    socketConnection->setRemoteAddress(InetAddress(remoteAddress));
                    socketConnection->setLocalAddress(InetAddress(localAddress));

                    socketConnection->ReadEventReceiver::enable();

                    onConnect(socketConnection);
                } else {
                    PLOG(ERROR) << "getsockname";
                    shutdown(scFd, SHUT_RDWR);
                    ::close(scFd);
                }
            } else if (errno != EINTR) {
                PLOG(ERROR) << "accept";
            }
        }

        void end() {
            AcceptEventReceiver::disable();
        }

    protected:
        void listen(int backlog, const std::function<void(int errnum)>& onError) {
            int ret = ::listen(getFd(), backlog);

            if (ret < 0) {
                onError(errno);
            } else {
                onError(0);
            }
        }

    private:
        void reuseAddress(const std::function<void(int errnum)>& onError) {
            int sockopt = 1;

            if (setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) < 0) {
                onError(errno);
            } else {
                onError(0);
            }
        }

        void unobserved() override {
            if (isDynamic) {
                delete this;
            }
        }

        std::function<void(SocketConnection* socketConnection)> onConnect;
        std::function<void(SocketConnection* socketConnection)> onDisconnect;
        std::function<void(SocketConnection* socketConnection, const char* junk, ssize_t junkLen)> onRead;
        std::function<void(SocketConnection* socketConnection, int errnum)> onReadError;
        std::function<void(SocketConnection* socketConnection, int errnum)> onWriteError;

        bool isDynamic;

    public:
        //    using SocketConnectionType = SocketConnectionImpl;
    };

} // namespace net::socket

#endif // SOCKETSERVER_H
