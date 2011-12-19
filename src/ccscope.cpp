#include <iostream>
#include <fstream>
#include <sstream>
#include <csignal>
#include <error.h>
#include <sys/inotify.h>
#include <pficommon/data/digest/md5.h>
#include <pficommon/concurrent/mvar.h>
#include "Inotify.hpp"

namespace {

volatile sig_atomic_t keep_running = 1;

void
signal_handler(int)
{
    keep_running = 0;
}

class TargetWatcher
    : public Inotify
{
public:
    TargetWatcher()
        : path2md5(), update(false) {}
    virtual ~TargetWatcher() throw () {}

    virtual int
    watch(const std::string& pathname, uint32_t mask) {
        std::string md5(to_md5(pathname));
        if ("" == md5) return -1;
        path2md5[pathname] = md5;
        return Inotify::watch(pathname, mask);
    }

    bool
    detect_update() {
        bool result = update;
        update = false;
        return result;
    }

protected:

    virtual void
    handle(const Event& event) {
        switch (event.mask &
                (IN_ALL_EVENTS | IN_UNMOUNT | IN_Q_OVERFLOW | IN_IGNORED))
        {
        case IN_MODIFY: {} break;
        case IN_MOVE_SELF: {} break;
        default:{ return; } break;
        }
        std::string pathname(get_path(event.wd));
        std::string md5(to_md5(pathname));
        if ("" == md5) return;
        if (path2md5[pathname] != md5)
        {
            path2md5[pathname] = md5;
            update = true;
        }
    }

private:

    std::string
    to_md5(const std::string& pathname) {
        std::ifstream ifs(pathname.c_str(), std::ios::binary);
        if (!ifs) return "";
        std::ostringstream oss;
        oss << pfi::data::digest::md5sum(ifs);
        ifs.close();
        return oss.str();
    }

    std::map< std::string, std::string > path2md5;
    bool update;
};

class ListWatcher
    : public Inotify
{
public:
    ListWatcher() : update(false) {}
    virtual ~ListWatcher() throw () {}

    bool
    detect_update() {
        bool result = update;
        update = false;
        return result;
    }

protected:
    virtual void
    handle(const Event& event) {
        switch (event.mask &
                (IN_ALL_EVENTS | IN_UNMOUNT | IN_Q_OVERFLOW | IN_IGNORED))
        {
        case IN_MODIFY: { update = true; } break;
        case IN_MOVE_SELF: { update = true; } break;
        }
    }
private:
    bool update;
};

void
load(TargetWatcher& tw, std::string listfile)
{
    std::ifstream ifs(listfile.c_str());
    while (ifs)
    {
        std::string target;
        if (!std::getline(ifs, target)) break;
        tw.watch(target, IN_ALL_EVENTS);
    }
    ifs.close();
}

void
reload(TargetWatcher& tw, std::string listfile)
{
    tw.ignore_all();
    load(tw, listfile);
}

} // anonymous namespace

#define IGNORE_RESULT(X) if (X) {}

int
main(int argc, char* argv[])
{
    if (daemon(1, 0) < 0)
        error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
                      "failed daemonize");

    IGNORE_RESULT(nice(19));

    if (SIG_IGN == signal(SIGINT, signal_handler))
        signal(SIGINT, SIG_IGN);

    std::string listfile("./cscope.files");
    std::string buildfile("./cscope.sh");

    TargetWatcher tw;
    load(tw, listfile);
    ListWatcher lw;
    lw.watch(listfile, IN_ALL_EVENTS);

    while (keep_running)
    {
        if (lw.detect_update())
        {
            reload(tw, listfile);
            IGNORE_RESULT(system(buildfile.c_str()));
        }
        if (tw.detect_update())
        {
            IGNORE_RESULT(system(buildfile.c_str()));
        }
        sleep(1);
    }

    return 0;
}
