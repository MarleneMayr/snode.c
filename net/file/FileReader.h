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

#ifndef FILEREADER_H
#define FILEREADER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>
#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "File.h"
#include "ReadEventReceiver.h"

class FileReader
    : public net::ReadEventReceiver
    , virtual public File {
protected:
    FileReader(int fd, const std::function<void(char* junk, int junkLen)>& junkRead, const std::function<void(int err)>& onError);

public:
    static FileReader* read(const std::string& path, const std::function<void(char* junk, int junkLen)>& junkRead,
                            const std::function<void(int err)>& onError);

    void disable() override;

    void readEvent() override;

private:
    void unobserved() override;

    std::function<void(char* data, int len)> junkRead;

    std::function<void(int errnum)> onError;

    bool stopped;
};

#endif // FILEREADER_H
