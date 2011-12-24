// Copyright (c) 2011, Noriyuki OHKAWA a.k.a. notogawa.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above
//   copyright notice, this list of conditions and the following
//   disclaimer in the documentation and/or other materials provided
//   with the distribution.
//
// * Neither the name of Noriyuki OHKAWA and notogawa nor the names of other
//   contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "Inotify.hpp"

#include <sys/inotify.h>
#include <sys/epoll.h>
#include <pficommon/concurrent/lock.h>

namespace {

struct runner
{
    runner(Inotify& obj) : obj(obj) {}
    void operator () () { obj.run(); }
    Inotify& obj;
};

} // anonymous namespace



Inotify::Inotify()
    : wd2path(), fd(inotify_init()), keep_running(true), thread(runner(*this))
{
    thread.start();
}

Inotify::~Inotify() throw ()
{
    keep_running = false;
    close(fd);
    thread.join();
}

int
Inotify::watch(const std::string& pathname, uint32_t mask)
{
    pfi::concurrent::scoped_lock lock(mutex);
    int wd = inotify_add_watch(fd, pathname.c_str(), mask);
    if (0 < wd) wd2path[wd] = pathname;
    return wd;
}

void
Inotify::ignore(int wd)
{
    pfi::concurrent::scoped_lock lock(mutex);
    if (wd2path.find(wd) != wd2path.end())
    {
        inotify_rm_watch(fd, wd);
        wd2path.erase(wd);
    }
}

void
Inotify::ignore_all()
{
    pfi::concurrent::scoped_lock lock(mutex);
    for (std::map< int, std::string >::iterator i = wd2path.begin();
         i != wd2path.end(); ++i)
        inotify_rm_watch(fd, i->first);
    wd2path.clear();
}

std::string
Inotify::get_path(int wd)
{
    std::string result("");
    pfi::concurrent::scoped_lock lock(mutex);
    if (wd2path.find(wd) != wd2path.end()) result = wd2path[wd];
    return result;
}

void
Inotify::run()
{
    struct epoll_event events[16];
    const size_t MAX_EVENTS = sizeof(events)/sizeof(events[0]);
    int epoll_fd = epoll_create(MAX_EVENTS);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLPRI;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    while (keep_running)
    {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, 1000);
        for (int i = 0; i < n; ++i)
        {
            struct inotify_event e;
            int result = 0;
            result = read(events[i].data.fd, &e, sizeof(e));
            Event event;
            event.wd = e.wd;
            event.mask = e.mask;
            event.cookie = e.cookie;
            event.pathname = std::string(e.name);
            handle(event);
        }
    }
}
