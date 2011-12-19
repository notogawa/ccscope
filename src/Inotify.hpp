#ifndef INOTIFY_HPP
#define INOTIFY_HPP

#include <string>
#include <map>
#include <pficommon/concurrent/thread.h>
#include <pficommon/concurrent/mutex.h>

class Inotify
{
public:
    Inotify();
    virtual ~Inotify() throw ();

    virtual int watch(const std::string& pathname, uint32_t mask);
    virtual void ignore(int wd);
    virtual void ignore_all();

    virtual std::string get_path(int wd);

    class Event
    {
    public:
        int wd;
        uint32_t mask;
        uint32_t cookie;
        std::string pathname;
    };

    void run();

protected:
    virtual void handle(const Event& event) = 0;

    std::map< int, std::string > wd2path;
    int fd;
    bool keep_running;

    pfi::concurrent::r_mutex mutex;
    pfi::concurrent::thread thread;

};

#endif
