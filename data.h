#pragma once

#include "types.h"
#include <map>
#include <vector>
#include <queue>
#include "json/json.h"

namespace ext {

struct Host;

struct Job {
    Host* host;
    u16 port;
    u32 timeout;
};
typedef std::queue<Job> JobList;

struct Host {
    string address;
    i32 type;
    u8 resolved[16];
    bool range_setted;
    u16 range_start;
    u16 range_end;
    u32 timeout;
    std::map<u16, u32> port_timeouts;
    Host() : type(0), range_setted(false), range_start(0), range_end(0), timeout(0) {}
    virtual bool Parse(Json::Value& node);
};

struct HostList : public Host {
    std::map<string, Host> hosts;
    HostList() : Host() {}
    bool Parse(const string& text);
    bool Parse(Json::Value& node);;
    JobList GenJobs();
    void AddRange(JobList& jobs, Host& h, u16 start, u16 end);
};

} // namespace ext
