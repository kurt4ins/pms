#pragma once
#include <string>
#include <vector>
#include <ctime>

enum ResourceType {
    SPA,
    MASSAGE,
    GYM,
    OTHER
};

struct ResourceBooking {
    u_int64_t bookingId;
    u_int64_t guestId;
    time_t startTime;
    time_t endTime;
    
    ResourceBooking(u_int64_t bookingId, u_int64_t guestId, time_t start, time_t end) :
        bookingId(bookingId), guestId(guestId), startTime(start), endTime(end) {}
};

class Resource {
private:
    u_int64_t resourceId;
    std::string name;
    ResourceType type;
    double hourlyRate;
    bool available;
    static u_int64_t nextResourceId;
    std::vector<ResourceBooking> bookings;
    
public:
    Resource(const std::string& name, ResourceType type, double hourlyRate);
    
    u_int64_t getId() const;
    std::string getName() const;
    ResourceType getType() const;
    double getRate() const;
    bool isAvailable() const;
    
    void setAvailable(bool status);
    
    bool checkAvailability(time_t startTime, time_t endTime) const;
    u_int64_t book(u_int64_t guestId, time_t startTime, time_t endTime);
    double calculateCharge(u_int64_t bookingId) const;
}; 