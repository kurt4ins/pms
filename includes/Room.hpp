#pragma once
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include "Guest.hpp"

struct Visit {
    time_t checkInDate;
    time_t checkOutDate;
    u_int64_t guestId;
    int roomNumber;
    std::vector<std::string> interactions;
    
    Visit(time_t checkIn, u_int64_t guest, int room) :
        checkInDate(checkIn), guestId(guest), roomNumber(room), checkOutDate(0) {}
};

class Room {
private:
    int roomNumber;
    std::string roomType;
    bool isOccupied;
    std::map<u_int64_t, Guest*> guests;
    std::vector<Visit> visitHistory;
    
public:
    Room();
    Room(int number, std::string type);
    ~Room();
    
    bool addGuest(Guest* guest);
    bool removeGuest(u_int64_t id);
    Guest* getGuest(u_int64_t id);
    
    bool checkIn(u_int64_t guestId, time_t checkInDate);
    bool checkOut(u_int64_t guestId, time_t checkOutDate);
    std::vector<Visit> getVisitHistory(u_int64_t guestId) const;
    
    bool addLoyaltyPoints(u_int64_t guestId, u_int points);
    u_int getLoyaltyPoints(u_int64_t guestId) const;
    
    bool isRoomOccupied() const;
    int getRoomNumber() const;
    std::string getRoomType() const;
};