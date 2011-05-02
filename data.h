#pragma once

#include "types.h"
#include <map>
#include <vector>

namespace ext {

struct Host;

struct Job {
    Host* host;
    u16 port;
    u32 timeout;
};

struct Port {
    u16 number;
    u32 timeout;
    Port() : number(0), timeout(0) {}
};

struct Host {
    string address;
    i32 type;
    u8* resolved;
    bool range_setted;
    u16 range_start;
    u16 range_end;
    u32 timeout;
    std::vector<Port> port_list;
    Host() : type(0), resolved(NULL), range_setted(false), range_start(0), range_end(0), timeout(0) {}
    ~Host() {
        if (resolved != NULL)
            delete[] resolved;
    }
};

struct HostList : public Host {
    std::map<string, Host> hosts;
    HostList() : Host() {}
};

} // namespace ext
