#include "../includes/Booking.hpp"
#include <algorithm>

u_int64_t Booking::nextBookingId = 1;

Booking::Booking() {}

Booking::~Booking() {}

u_int64_t Booking::createReservation(u_int64_t guestId, int roomNumber, time_t checkInDate, time_t checkOutDate) {
    if (rooms.find(roomNumber) == rooms.end()) {
        return 0;
    }
    
    for (auto it = rooms.begin(); it != rooms.end(); ++it) {
        if (it->first == roomNumber) {
            if (it->second->isRoomOccupied()) {
                std::vector<Visit> visits = it->second->getVisitHistory(guestId);
                for (const auto& visit : visits) {
                    if (!(checkOutDate <= visit.checkInDate || checkInDate >= visit.checkOutDate)) {
                        return 0;
                    }
                }
            }
        }
    }
    
    u_int64_t newBookingId = nextBookingId++;
    this->bookingId = newBookingId;
    this->guestId = guestId;
    this->roomNumber = roomNumber;
    this->checkInDate = checkInDate;
    this->checkOutDate = checkOutDate;
    this->status = BookingStatus::RESERVED;
    
    return newBookingId;
}

bool Booking::cancelReservation(u_int64_t bookingId) {
    if (this->bookingId != bookingId) {
        return false;
    }
    
    if (status == BookingStatus::CHECKED_IN) {
        return false;
    }
    
    status = BookingStatus::CANCELLED;
    return true;
}

bool Booking::modifyReservation(u_int64_t bookingId, time_t newCheckInDate, time_t newCheckOutDate) {
    if (this->bookingId != bookingId) {
        return false;
    }
    
    if (status != BookingStatus::RESERVED) {
        return false;
    }
    
    this->checkInDate = newCheckInDate;
    this->checkOutDate = newCheckOutDate;
    
    return true;
}

bool Booking::checkIn(u_int64_t bookingId) {
    if (this->bookingId != bookingId) {
        return false;
    }
    
    if (status != BookingStatus::RESERVED) {
        return false;
    }
    
    if (rooms.find(roomNumber) == rooms.end()) {
        return false;
    }
    
    if (rooms[roomNumber]->checkIn(guestId, time(nullptr))) {
        status = BookingStatus::CHECKED_IN;
        return true;
    }
    
    return false;
}

bool Booking::checkOut(u_int64_t bookingId) {
    if (this->bookingId != bookingId) {
        return false;
    }
    
    if (status != BookingStatus::CHECKED_IN) {
        return false;
    }
    
    if (rooms.find(roomNumber) == rooms.end()) {
        return false;
    }
    
    if (rooms[roomNumber]->checkOut(guestId, time(nullptr))) {
        status = BookingStatus::CHECKED_OUT;
        return true;
    }
    
    return false;
}

void Booking::addRoom(Room* room) {
    if (room != nullptr) {
        rooms[room->getRoomNumber()] = room;
    }
}

void Booking::removeRoom(int roomNumber) {
    rooms.erase(roomNumber);
}

Room* Booking::getRoom(int roomNumber) {
    if (rooms.find(roomNumber) != rooms.end()) {
        return rooms[roomNumber];
    }
    return nullptr;
}

std::vector<Room*> Booking::getAvailableRooms(time_t checkInDate, time_t checkOutDate) {
    std::vector<Room*> availableRooms;
    
    for (auto it = rooms.begin(); it != rooms.end(); ++it) {
        if (!it->second->isRoomOccupied()) {
            availableRooms.push_back(it->second);
        }
    }
    
    return availableRooms;
}

BookingStatus Booking::getBookingStatus(u_int64_t bookingId) const {
    if (this->bookingId == bookingId) {
        return status;
    }

    return BookingStatus::CANCELLED;
}

std::vector<u_int64_t> Booking::getGuestBookings(u_int64_t guestId) const {
    std::vector<u_int64_t> bookings;
    if (this->guestId == guestId) {
        bookings.push_back(bookingId);
    }
    return bookings;
}

std::vector<u_int64_t> Booking::getRoomBookings(int roomNumber) const {
    std::vector<u_int64_t> bookings;
    if (this->roomNumber == roomNumber) {
        bookings.push_back(bookingId);
    }
    return bookings;
}

int Booking::getOccupancyRate(time_t date) const {
    int totalRooms = rooms.size();
    if (totalRooms == 0) {
        return 0;
    }
    
    int occupiedRooms = 0;
    for (const auto& room : rooms) {
        if (room.second->isRoomOccupied()) {
            occupiedRooms++;
        }
    }
    
    return (occupiedRooms * 100) / totalRooms;
}

std::vector<Room*> Booking::getOccupiedRooms() const {
    std::vector<Room*> occupiedRooms;
    
    for (const auto& room : rooms) {
        if (room.second->isRoomOccupied()) {
            occupiedRooms.push_back(room.second);
        }
    }
    
    return occupiedRooms;
}

std::vector<Room*> Booking::getVacantRooms() const {
    std::vector<Room*> vacantRooms;
    
    for (const auto& room : rooms) {
        if (!room.second->isRoomOccupied()) {
            vacantRooms.push_back(room.second);
        }
    }
    
    return vacantRooms;
}
