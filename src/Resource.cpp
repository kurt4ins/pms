#include "Resource.hpp"

Resource::Resource(int id, const string& name, const string& desc) 
    : resourceId(id), name(name), description(desc), isAvailable(true) {}

bool Resource::reserve(const time_point<system_clock>& start, const time_point<system_clock>& end, int guestId) {
    if (!checkAvailability(start, end)) {
        return false;
    }
    schedule[start].emplace_back(start, end);
    return true;
}

bool Resource::cancelReservation(const time_point<system_clock>& start) {
    if (schedule.find(start) == schedule.end()) {
        return false;
    }
    schedule.erase(start);
    return true;
}

bool Resource::checkAvailability(const time_point<system_clock>& start, const time_point<system_clock>& end) const {
    for (const auto& [date, slots] : schedule) {
        for (const auto& [slotStart, slotEnd] : slots) {
            if (!(end <= slotStart || start >= slotEnd)) {
                return false;  // Найдено пересечение
            }
        }
    }
    return true;
}

int Resource::getId() const { return resourceId; }
string Resource::getName() const { return name; }
bool Resource::getAvailability() const { return isAvailable; }