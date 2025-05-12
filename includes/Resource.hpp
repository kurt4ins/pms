#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Resource {
protected:
    int resourceId;
    string name;
    string description;
    bool isAvailable;
    map<time_point<system_clock>, vector<pair<time_point<system_clock>, time_point<system_clock>>>> schedule;

public:
    Resource(int id, const string& name, const string& desc = "");
    virtual ~Resource() = default;

    virtual bool reserve(const time_point<system_clock>& start, const time_point<system_clock>& end, int guestId);
    virtual bool cancelReservation(const time_point<system_clock>& start);
    virtual bool checkAvailability(const time_point<system_clock>& start, const time_point<system_clock>& end) const;

    int getId() const;
    string getName() const;
    bool getAvailability() const;
};

#endif