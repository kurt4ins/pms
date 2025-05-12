#include "../includes/Resource.hpp"

u_int64_t Resource::nextResourceId = 1;

Resource::Resource(const std::string& name, ResourceType type, double hourlyRate) :
    name(name),
    type(type),
    hourlyRate(hourlyRate),
    available(true) {
    resourceId = nextResourceId++;
}

u_int64_t Resource::getId() const {
    return resourceId;
}

std::string Resource::getName() const {
    return name;
}

ResourceType Resource::getType() const {
    return type;
}

double Resource::getRate() const {
    return hourlyRate;
}

bool Resource::isAvailable() const {
    return available;
}

void Resource::setAvailable(bool status) {
    available = status;
}

bool Resource::checkAvailability(time_t startTime, time_t endTime) const {
    if (!available) {
        return false;
    }
    
    for (const auto& booking : bookings) {
        if (!(endTime <= booking.startTime || startTime >= booking.endTime)) {
            return false;
        }
    }
    
    return true;
}

u_int64_t Resource::book(u_int64_t guestId, time_t startTime, time_t endTime) {
    if (!checkAvailability(startTime, endTime)) {
        return 0;
    }
    
    u_int64_t bookingId = static_cast<u_int64_t>(time(nullptr)) + guestId;
    bookings.emplace_back(bookingId, guestId, startTime, endTime);
    
    return bookingId;
}

double Resource::calculateCharge(u_int64_t bookingId) const {
    for (const auto& booking : bookings) {
        if (booking.bookingId == bookingId) {
            double hours = static_cast<double>(booking.endTime - booking.startTime) / 3600.0;
            return hours * hourlyRate;
        }
    }
    
    return 0.0;
} 