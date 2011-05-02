#pragma once

#include "types.h"
#include <map>
#include <vector>
#include <queue>
#include "json/json.h"

namespace ext {

struct Host;

//! Tuple for worker class (final values to timeout, calculated by defaults, and ranges)
struct Job {
    Host* host;     //!< Pointer to host structure
    u16 port;       //!< Port to connect
    u32 timeout;    //!< Connection timeout
};
typedef std::queue<Job> JobList;

//! Host class
struct Host {
    string address;     //!< Host address
    i32 type;           //!< Address type, ipv4 or ipv6
    u8 resolved[16];    //!< Resolved address
    bool range_setted;  //!< Is range of port to scan setted
    u16 range_start;    //!< Start of range
    u16 range_end;      //!< End of range
    u32 timeout;        //!< Default timeout for this host
    std::map<u16, u32> port_timeouts;   //!< List of port-timeout, if port is in list, then we use this timeout value, and default timeout overwise
    Host() 
        : type(0)
        , range_setted(false)
        , range_start(0)
        , range_end(0)
        , timeout(0) {}
    //! Parses object from json node
    virtual bool Parse(Json::Value& node);
};

//! List of host, note - it contains default values of Host class, it becomes global defaults
struct HostList : public Host {
    std::map<string, Host> hosts;       //!< Named list of hosts

    HostList() : Host() {}
    
    //! Reads contant of file, and parse it to hosts structure
    bool Parse(const string& filename);
    //! Parses object from json node
    bool Parse(Json::Value& node);
    //! Generate list of tasks for worker (final tuples host-port-timeout)
    JobList GenJobs();
private:
    void AddRange(JobList& jobs, Host& h, u16 start, u16 end);
};

} // namespace ext
