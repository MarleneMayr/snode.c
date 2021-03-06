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

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <easylogging++.h>
#include <map>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "HTTPClientContext.h"
#include "socket/SocketConnectionBase.h"

namespace http {

    HTTPClientContext::HTTPClientContext(net::socket::SocketConnectionBase* socketConnection,
                                         const std::function<void(ClientResponse&)>& onResponse,
                                         const std::function<void(int status, const std::string& reason)>& onError)
        : socketConnection(socketConnection)
        , parser(
              [this](const std::string& httpVersion, const std::string& statusCode, const std::string& reason) -> void {
                  clientResponse.httpVersion = httpVersion;
                  clientResponse.statusCode = statusCode;
                  clientResponse.reason = reason;
              },
              [this](const std::map<std::string, std::string>& headers,
                     const std::map<std::string, http::ResponseCookie>& cookies) -> void {
                  clientResponse.headers = &headers;
                  clientResponse.cookies = &cookies;
              },
              [this](char* content, size_t contentLength) -> void {
                  clientResponse.body = content;
                  clientResponse.contentLength = contentLength;
              },
              [this, onResponse](http::HTTPResponseParser& parser) -> void {
                  onResponse(clientResponse);
                  parser.reset();
              },
              [onError](int status, const std::string& reason) -> void {
                  onError(status, reason);
              }) {
    }

    void HTTPClientContext::receiveResponseData(const char* junk, size_t junkLen) {
        parser.parse(junk, junkLen);
    }

} // namespace http
