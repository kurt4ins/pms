#pragma once
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include "Room.hpp"
#include "Guest.hpp"
#include "Bill.hpp"
#include "Resource.hpp"

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
    std::map<u_int64_t, Bill*> bills;
    std::map<u_int64_t, Resource*> resources;
    
public:
    Booking();
    ~Booking();
    
    u_int64_t createReservation(u_int64_t guestId, int roomNumber, time_t checkInDate, time_t checkOutDate);
    bool cancelReservation(u_int64_t bookingId);
    
    bool checkIn(u_int64_t bookingId);
    bool checkOut(u_int64_t bookingId);
    
    void addRoom(Room* room);
    void removeRoom(int roomNumber);
    Room* getRoom(int roomNumber);
    std::vector<Room*> getAvailableRooms(time_t checkInDate, time_t checkOutDate);
    
    BookingStatus getBookingStatus(u_int64_t bookingId) const;
    std::vector<u_int64_t> getGuestBookings(u_int64_t guestId) const;
    std::vector<u_int64_t> getRoomBookings(int roomNumber) const;
    
    std::vector<Room*> getOccupiedRooms() const;
    std::vector<Room*> getVacantRooms() const;

    u_int64_t createBill();
    Bill* getBill(u_int64_t billId);
    bool deleteBill(u_int64_t billId);
    std::vector<Bill*> getAllBills() const;

    bool addBillItem(u_int64_t billId, const std::string& description, double amount);
    bool removeBillItem(u_int64_t billId, int itemIndex);

    bool payBill(u_int64_t billId);
    bool payBillItem(u_int64_t billId, int itemIndex);
    bool payBillAmount(u_int64_t billId, double amount);

    double getTotalBillAmount(u_int64_t billId) const;
    double getRemainingBillAmount(u_int64_t billId) const;
    std::string generateBillSummary(u_int64_t billId) const;

    void addResource(Resource* resource);
    std::vector<Resource*> getAvailableResources(time_t startTime, time_t endTime) const;
    u_int64_t bookResource(u_int64_t resourceId, time_t startTime, time_t endTime);
    bool addResourceToBill(u_int64_t billId, u_int64_t resourceId, u_int64_t bookingId);
};