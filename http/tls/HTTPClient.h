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

#ifndef TLS_HTTPCLIENT_H
#define TLS_HTTPCLIENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>
#include <netinet/in.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "socket/tls/SocketConnection.h"

namespace http {

    class ClientResponse;

    namespace tls {

        class HTTPClient {
        public:
            HTTPClient(const std::function<void(net::socket::tls::SocketConnection*)>& onConnect,
                       const std::function<void(ClientResponse& clientResponse)> onResponseReady,
                       const std::function<void(net::socket::tls::SocketConnection*)> onDisconnect, const std::string& caFile = "",
                       const std::string& caDir = "", bool useDefaultCADir = false);

            void connect(const std::string& server, in_port_t port, const std::function<void(int err)>& onError);

        protected:
            std::function<void(net::socket::tls::SocketConnection*)> onConnect;
            std::function<void(ClientResponse& clientResponse)> onResponseReady;
            std::function<void(net::socket::tls::SocketConnection*)> onDisconnect;

            std::string caFile;
            std::string caDir;
            bool useDefaultCADir;
        };

    } // namespace tls

} // namespace http

#endif // TLS_HTTPCLIENT_H