#include "../includes/Booking.hpp"
#include <iostream>

u_int64_t Booking::nextBookingId = 1;

Booking::Booking() {}

Booking::~Booking() {
    for (auto& pair : bills) {
        delete pair.second;
    }
    bills.clear();
}

u_int64_t Booking::createReservation(u_int64_t guestId, int roomNumber, time_t checkInDate, time_t checkOutDate) {
    if (rooms.find(roomNumber) == rooms.end()) {
        return 0;
    }
    
    for (const auto& booking : bookings) {
        if (booking.second.roomNumber == roomNumber && booking.second.status != CANCELLED && booking.second.status != CHECKED_OUT) {
            if (!(checkOutDate <= booking.second.checkInDate || checkInDate >= booking.second.checkOutDate)) {
                return 0;
            }
        }
    }
    
    u_int64_t newBookingId = nextBookingId++;
    BookingInfo newBooking(newBookingId, guestId, roomNumber, checkInDate, checkOutDate);
    bookings.insert(std::make_pair(newBookingId, newBooking));
    
    return newBookingId;
}

bool Booking::cancelReservation(u_int64_t bookingId) {
    auto it = bookings.find(bookingId);
    if (it == bookings.end()) {
        return false;
    }
    
    if (it->second.status == CHECKED_IN) {
        return false;
    }
    
    it->second.status = CANCELLED;
    return true;
}

bool Booking::modifyReservation(u_int64_t bookingId, time_t newCheckInDate, time_t newCheckOutDate) {
    auto it = bookings.find(bookingId);
    if (it == bookings.end()) {
        return false;
    }
    
    if (it->second.status != RESERVED) {
        return false;
    }
    
    int roomNumber = it->second.roomNumber;
    for (const auto& booking : bookings) {
        if (booking.first != bookingId && booking.second.roomNumber == roomNumber && 
            booking.second.status != CANCELLED && booking.second.status != CHECKED_OUT) {
            if (!(newCheckOutDate <= booking.second.checkInDate || newCheckInDate >= booking.second.checkOutDate)) {
                return false;
            }
        }
    }
    
    it->second.checkInDate = newCheckInDate;
    it->second.checkOutDate = newCheckOutDate;
    
    return true;
}

bool Booking::checkIn(u_int64_t bookingId) {
    auto it = bookings.find(bookingId);
    if (it == bookings.end()) {
        return false;
    }
    
    BookingInfo& booking = it->second;
    
    if (booking.status != RESERVED) {
        return false;
    }
    
    if (rooms.find(booking.roomNumber) == rooms.end()) {
        return false;
    }
    
    if (rooms[booking.roomNumber]->checkIn(booking.guestId, time(nullptr))) {
        booking.status = CHECKED_IN;
        u_int64_t billId = createBill(bookingId);
        booking.associatedBillIds.push_back(billId);
        
        return true;
    }
    
    return false;
}

bool Booking::checkOut(u_int64_t bookingId) {
    auto it = bookings.find(bookingId);
    if (it == bookings.end()) {
        return false;
    }
    
    BookingInfo& booking = it->second;
    
    if (booking.status != CHECKED_IN) {
        return false;
    }
    
    if (rooms.find(booking.roomNumber) == rooms.end()) {
        return false;
    }
    
    for (u_int64_t billId : booking.associatedBillIds) {
        auto billIt = bills.find(billId);
        if (billIt != bills.end() && billIt->second->getRemainingAmount() > 0) {
            return false;
        }
    }
    
    if (rooms[booking.roomNumber]->checkOut(booking.guestId, time(nullptr))) {
        booking.status = CHECKED_OUT;
        
        for (u_int64_t billId : booking.associatedBillIds) {
            auto billIt = bills.find(billId);
            if (billIt != bills.end()) {
                billIt->second->close();
            }
        }
        
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
    
    std::vector<int> occupiedRoomNumbers;
    for (const auto& booking : bookings) {
        if (booking.second.status != CANCELLED && booking.second.status != CHECKED_OUT) {
            if (!(checkOutDate <= booking.second.checkInDate || checkInDate >= booking.second.checkOutDate)) {
                occupiedRoomNumbers.push_back(booking.second.roomNumber);
            }
        }
    }
    
    for (const auto& room : rooms) {
        if (std::find(occupiedRoomNumbers.begin(), occupiedRoomNumbers.end(), room.first) == occupiedRoomNumbers.end() && 
            !room.second->isRoomOccupied()) {
            availableRooms.push_back(room.second);
        }
    }
    
    return availableRooms;
}

BookingStatus Booking::getBookingStatus(u_int64_t bookingId) const {
    auto it = bookings.find(bookingId);
    if (it != bookings.end()) {
        return it->second.status;
    }
    
    return CANCELLED;
}

std::vector<u_int64_t> Booking::getGuestBookings(u_int64_t guestId) const {
    std::vector<u_int64_t> result;
    
    for (const auto& booking : bookings) {
        if (booking.second.guestId == guestId) {
            result.push_back(booking.first);
        }
    }
    
    return result;
}

std::vector<u_int64_t> Booking::getRoomBookings(int roomNumber) const {
    std::vector<u_int64_t> result;
    
    for (const auto& booking : bookings) {
        if (booking.second.roomNumber == roomNumber) {
            result.push_back(booking.first);
        }
    }
    
    return result;
}

std::vector<u_int64_t> Booking::getAllBookings() const {
    std::vector<u_int64_t> result;
    
    for (const auto& booking : bookings) {
        result.push_back(booking.first);
    }
    
    return result;
}

BookingInfo* Booking::getBookingInfo(u_int64_t bookingId) {
    auto it = bookings.find(bookingId);
    if (it != bookings.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<Room*> Booking::getOccupiedRooms() const {
    std::vector<Room*> result;
    
    for (const auto& room : rooms) {
        if (room.second->isRoomOccupied()) {
            result.push_back(room.second);
        }
    }
    
    return result;
}

std::vector<Room*> Booking::getVacantRooms() const {
    std::vector<Room*> result;
    
    for (const auto& room : rooms) {
        if (!room.second->isRoomOccupied()) {
            result.push_back(room.second);
        }
    }
    
    return result;
}

u_int64_t Booking::createBill(u_int64_t bookingId) {
    auto bookingIt = bookings.find(bookingId);
    if (bookingIt == bookings.end()) {
        return 0;
    }
    
    const BookingInfo& booking = bookingIt->second;
    
    Bill* newBill = new Bill(bookingId, booking.guestId);
    u_int64_t billId = newBill->getBillId();
    bills[billId] = newBill;
    
    int stayDuration = static_cast<int>((booking.checkOutDate - booking.checkInDate) / 86400);
    double roomRate = 5000.0;
    newBill->addItem("Проживание в номере " + std::to_string(booking.roomNumber), roomRate * stayDuration);
    
    return billId;
}

Bill* Booking::getBill(u_int64_t billId) {
    auto it = bills.find(billId);
    if (it != bills.end()) {
        return it->second;
    }
    return nullptr;
}

bool Booking::deleteBill(u_int64_t billId) {
    auto it = bills.find(billId);
    if (it == bills.end()) {
        return false;
    }
    
    if (!it->second->getIsClosed()) {
        for (auto& booking : bookings) {
            auto& billIds = booking.second.associatedBillIds;
            billIds.erase(std::remove(billIds.begin(), billIds.end(), billId), billIds.end());
        }
        
        delete it->second;
        bills.erase(it);
        return true;
    }
    
    return false;
}

std::vector<Bill*> Booking::getAllBills() const {
    std::vector<Bill*> result;
    
    for (const auto& pair : bills) {
        result.push_back(pair.second);
    }
    
    return result;
}

std::vector<Bill*> Booking::getBookingBills(u_int64_t bookingId) const {
    std::vector<Bill*> result;
    
    auto bookingIt = bookings.find(bookingId);
    if (bookingIt != bookings.end()) {
        for (u_int64_t billId : bookingIt->second.associatedBillIds) {
            auto billIt = bills.find(billId);
            if (billIt != bills.end()) {
                result.push_back(billIt->second);
            }
        }
    }
    
    return result;
}

bool Booking::addBillItem(u_int64_t billId, const std::string& description, double amount) {
    Bill* bill = getBill(billId);
    if (bill == nullptr) {
        return false;
    }
    
    bill->addItem(description, amount);
    return true;
}

bool Booking::removeBillItem(u_int64_t billId, int itemIndex) {
    Bill* bill = getBill(billId);
    if (bill == nullptr) {
        return false;
    }
    
    return bill->removeItem(itemIndex);
}

bool Booking::payBill(u_int64_t billId) {
    Bill* bill = getBill(billId);
    if (bill == nullptr) {
        return false;
    }
    
    return bill->payAll();
}

bool Booking::payBillItem(u_int64_t billId, int itemIndex) {
    Bill* bill = getBill(billId);
    if (bill == nullptr) {
        return false;
    }
    
    return bill->payItem(itemIndex);
}

bool Booking::payBillAmount(u_int64_t billId, double amount) {
    Bill* bill = getBill(billId);
    if (bill == nullptr) {
        return false;
    }
    
    return bill->payAmount(amount);
}

double Booking::getTotalBillAmount(u_int64_t billId) const {
    auto it = bills.find(billId);
    if (it == bills.end()) {
        return 0.0;
    }
    
    return it->second->getTotalAmount();
}

double Booking::getRemainingBillAmount(u_int64_t billId) const {
    auto it = bills.find(billId);
    if (it == bills.end()) {
        return 0.0;
    }
    
    return it->second->getRemainingAmount();
}

std::string Booking::generateBillSummary(u_int64_t billId) const {
    auto it = bills.find(billId);
    if (it == bills.end()) {
        return "Счет не найден";
    }
    
    return it->second->generateBillSummary();
}

void Booking::addResource(Resource* resource) {
    if (resource != nullptr) {
        resources[resource->getId()] = resource;
    }
}

std::vector<Resource*> Booking::getAvailableResources(time_t startTime, time_t endTime) const {
    std::vector<Resource*> available;
    
    for (const auto& pair : resources) {
        if (pair.second->isAvailable() && pair.second->checkAvailability(startTime, endTime)) {
            available.push_back(pair.second);
        }
    }
    
    return available;
}

u_int64_t Booking::bookResource(u_int64_t bookingId, u_int64_t resourceId, time_t startTime, time_t endTime) {
    auto bookingIt = bookings.find(bookingId);
    if (bookingIt == bookings.end() || bookingIt->second.status != CHECKED_IN) {
        return 0;
    }
    
    auto resourceIt = resources.find(resourceId);
    if (resourceIt == resources.end()) {
        return 0;
    }
    
    return resourceIt->second->book(bookingIt->second.guestId, startTime, endTime);
}

bool Booking::addResourceToBill(u_int64_t billId, u_int64_t resourceId, u_int64_t resourceBookingId) {
    Bill* bill = getBill(billId);
    if (bill == nullptr) {
        return false;
    }
    
    auto it = resources.find(resourceId);
    if (it == resources.end()) {
        return false;
    }
    
    Resource* resource = it->second;
    double charge = resource->calculateCharge(resourceBookingId);
    
    if (charge <= 0.0) {
        return false;
    }
    
    bill->addItem("Услуга: " + resource->getName(), charge);
    return true;
}

