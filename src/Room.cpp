#include "../includes/Room.hpp"

Room::Room() : roomNumber(0), roomType("Standard"), isOccupied(false) {}
Room::Room(int number, std::string type) : roomNumber(number), roomType(type), isOccupied(false) {}

Room::~Room() {
    guests.clear();
}

bool Room::addGuest(Guest* guest) {
    if (guest == nullptr) {
        return false;
    }
    
    if (guests.find(guest->getId()) != guests.end()) {
        return false;
    }
    
    guests[guest->getId()] = guest;
    return true;
}

bool Room::removeGuest(u_int64_t id) {
    if (guests.find(id) == guests.end()) {
        return false;
    }
    
    guests.erase(id);
    return true;
}

Guest* Room::getGuest(u_int64_t id) {
    if (guests.find(id) == guests.end()) {
        return nullptr;
    }
    
    return guests[id];
}

bool Room::checkIn(u_int64_t guestId, time_t checkInDate) {
    if (guests.find(guestId) == guests.end()) {
        return false;
    }
    
    if (isOccupied) {
        return false;
    }
    
    Visit newVisit(checkInDate, guestId, roomNumber);
    visitHistory.push_back(newVisit);
    
    isOccupied = true;
    
    return true;
}

bool Room::checkOut(u_int64_t guestId, time_t checkOutDate) {
    if (!isOccupied) {
        return false;
    }
    
    for (auto it = visitHistory.rbegin(); it != visitHistory.rend(); ++it) {
        if (it->guestId == guestId && it->checkOutDate == 0) {
            it->checkOutDate = checkOutDate;
            
            int stayDuration = static_cast<int>((checkOutDate - it->checkInDate) / 86400);
            addLoyaltyPoints(guestId, stayDuration * 10);
            
            isOccupied = false;
            
            return true;
        }
    }
    
    return false;
}

std::vector<Visit> Room::getVisitHistory(u_int64_t guestId) const {
    std::vector<Visit> guestVisits;
    
    for (const auto& visit : visitHistory) {
        if (visit.guestId == guestId) {
            guestVisits.push_back(visit);
        }
    }
    
    return guestVisits;
}

bool Room::addLoyaltyPoints(u_int64_t guestId, u_int points) {
    auto it = guests.find(guestId);
    if (it == guests.end()) {
        return false;
    }
    
    it->second->addLoyaltyPoints(points);
    return true;
}

u_int Room::getLoyaltyPoints(u_int64_t guestId) const {
    auto it = guests.find(guestId);
    if (it == guests.end()) {
        return 0;
    }
    
    return it->second->getLoyalPoints();
}

bool Room::addInteraction(u_int64_t guestId, const std::string& interaction) {
    for (auto it = visitHistory.rbegin(); it != visitHistory.rend(); ++it) {
        if (it->guestId == guestId && it->checkOutDate == 0) {
            it->interactions.push_back(interaction);
            return true;
        }
    }
    
    return false;
}

std::vector<std::string> Room::getInteractions(u_int64_t guestId) const {
    std::vector<std::string> interactions;
    
    for (const auto& visit : visitHistory) {
        if (visit.guestId == guestId) {
            interactions.insert(interactions.end(), visit.interactions.begin(), visit.interactions.end());
        }
    }
    
    return interactions;
}

bool Room::isRoomOccupied() const {
    return isOccupied;
}

int Room::getRoomNumber() const {
    return roomNumber;
}

std::string Room::getRoomType() const {
    return roomType;
}

