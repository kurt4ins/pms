#pragma once
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include "Room.hpp"
#include "Guest.hpp"

enum BookingStatus {
    RESERVED,
    CHECKED_IN,
    CHECKED_OUT,
    CANCELLED
};

class Booking {
private:
    u_int64_t bookingId;
    u_int64_t guestId;
    int roomNumber;
    time_t checkInDate;
    time_t checkOutDate;
    BookingStatus status;
    static u_int64_t nextBookingId;
    
    std::map<int, Room*> rooms;
    
public:
    Booking();
    ~Booking();
    
    u_int64_t createReservation(u_int64_t guestId, int roomNumber, time_t checkInDate, time_t checkOutDate);
    bool cancelReservation(u_int64_t bookingId);
    bool modifyReservation(u_int64_t bookingId, time_t newCheckInDate, time_t newCheckOutDate);
    
    bool checkIn(u_int64_t bookingId);
    bool checkOut(u_int64_t bookingId);
    
    void addRoom(Room* room);
    void removeRoom(int roomNumber);
    Room* getRoom(int roomNumber);
    std::vector<Room*> getAvailableRooms(time_t checkInDate, time_t checkOutDate);
    
    BookingStatus getBookingStatus(u_int64_t bookingId) const;
    std::vector<u_int64_t> getGuestBookings(u_int64_t guestId) const;
    std::vector<u_int64_t> getRoomBookings(int roomNumber) const;
    
    int getOccupancyRate(time_t date) const;
    std::vector<Room*> getOccupiedRooms() const;
    std::vector<Room*> getVacantRooms() const;
};