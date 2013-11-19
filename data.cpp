#include "data.h"
#include <iostream>
#include <fstream>
#include "socket.h"

#ifdef max
    #undef max
#endif
#ifdef min
    #undef min
#endif

namespace ext {

bool HostList::Parse(const string& filename) {
    std::fstream file;
    file.open(filename.c_str(), std::ios::in );
    if (!file.is_open()) {
        std::cout << "file " << filename << " is not existed" << std::endl;
        return false;
    }
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(file, root)) {
        std::cout << reader.getFormatedErrorMessages() << std::endl;
        return false;
    }
    file.close();
    return Parse(root);
}

bool HostList::Parse(Json::Value& node) {
    if (!Host::Parse(node))
        return false;
    Json::Value hosts_node = node.get("hosts", Json::Value::null);
    if (hosts_node == Json::Value::null) {
        std::cout << "\"hosts\" node is not found" << std::endl;
        return false;
    }
    if (!hosts_node.isObject()) {
        std::cout << "\"hosts\" node should be an object" << std::endl;
        return false;
    }
    if (hosts_node.size() == 0) {
        std::cout << "\"hosts\" object is empty" << std::endl;
    }
    Json::Value::Members members(hosts_node.getMemberNames());   
    for (Json::Value::Members::iterator it = members.begin();    
            it != members.end();    
            ++it )   
    {   
        const std::string &name = *it;   
        Host h;
        h.address = name;
        
        if (!SocketConnector::resolve(h)) {
            std::cout << "Can't resolve " << name << "hostname" << std::endl;
            return false;
        }
        Json::Value hostNode = hosts_node[name];
        if (hostNode == Json::Value::null) {
            std::cout << "Host node " << h.address << " is empty" << std::endl;
            return false;
        }
        if (!h.Parse(hostNode)) {
            return false;
        }
        hosts[h.address] = h;
    }
    return true;
}


bool Host::Parse(Json::Value& node) {
    timeout = node.get("timeout", 0).asUInt();
    Json::Value rangeNode = node.get("port_to_scan", Json::Value::null);
    if (rangeNode == Json::Value::null)
    {
        range_setted = false;
    }
    else
    {
        range_setted = true;
        i32 from = rangeNode.get("from", 0).asUInt();
        i32 to = rangeNode.get("to", 65535).asUInt();
        if (from > to || from > 65535 || to > 65535) {
            std::cout << "incorrect range" << std::endl;
            return false;
        }
        range_start = from;
        range_end = to;
    }

    Json::Value pt = node.get("port_timeout", Json::Value::null);
    if (pt == Json::Value::null) {
        return true;
    }
    if (!pt.isArray()) {
        std::cout << "\"port_timeout\" node should be an array" << std::endl;
        return false;
    }
    for(u32 i=0; i<pt.size(); i++) {
        Json::Value portTimeout = pt[i];
        u32 port = portTimeout.get("port", 0).asUInt();
        if (port > 65535) {
            std::cout << "incorrect port value" << std::endl;
            return false;
        }
        u32 tout = portTimeout.get("timeout", 0).asUInt();
        port_timeouts[(u16)port] = tout;
    }

    return true;
}

JobList HostList::GenJobs() {
    JobList jobs;

    for (std::map<string, Host>::iterator it = hosts.begin();
        it != hosts.end();
        ++it) {
        Host& h = it->second;
        if (h.range_setted && range_setted)
        {
            if (range_start < h.range_start)
            {
                if (range_end < h.range_start)
                {
                    // no intersection
                    AddRange(jobs, h, range_start, range_end);
                    AddRange(jobs, h, h.range_start, h.range_end);
                }
                else
                {
                    // intersection or inclusion
                    AddRange(jobs, h, range_start, std::max(h.range_end, range_end));
                }
            }
            else
            {
                if (range_start < h.range_end)
                {
                    AddRange(jobs, h, h.range_start, std::max(h.range_end, range_end));
                }
                else
                {
                    // no intersection
                    AddRange(jobs, h, range_start, range_end);
                    AddRange(jobs, h, h.range_start, h.range_end);
                }
            }
        }
        else if (h.range_setted)
        {
            AddRange(jobs, h, h.range_start, h.range_end);
        }
        else if (range_setted)
        {
            AddRange(jobs, h, range_start, range_end);
        }
    }
    return jobs;
}

void HostList::AddRange(JobList& jobs, Host& h, u16 start, u16 end) {
    for(u16 port=start; port<end; port++) {
        std::map<u16, u32>::iterator itr = h.port_timeouts.find(port);
        Job toAdd;
        toAdd.host = &h;
        toAdd.port = port;
        if (itr != h.port_timeouts.end()) {
            toAdd.timeout = itr->second;
            jobs.push(toAdd);
            continue;
        }
        itr = port_timeouts.find(port);
        if (itr != port_timeouts.end()) {
            toAdd.timeout = itr->second;
            jobs.push(toAdd);
            continue;
        }
        if (h.timeout > 0) {
            toAdd.timeout = h.timeout;
            jobs.push(toAdd);
            continue;
        }
        if (timeout > 0) {
            toAdd.timeout = timeout;
            jobs.push(toAdd);
            continue;
        }
    }
}

} // namespace ext
