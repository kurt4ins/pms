#include "../includes/Booking.hpp"

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
    this->status = RESERVED;
    
    return newBookingId;
}

bool Booking::cancelReservation(u_int64_t bookingId) {
    if (this->bookingId != bookingId) {
        return false;
    }
    
    if (status == CHECKED_IN) {
        return false;
    }
    
    status = CANCELLED;
    return true;
}

bool Booking::checkIn(u_int64_t bookingId) {
    if (this->bookingId != bookingId) {
        return false;
    }
    
    if (status != RESERVED) {
        return false;
    }
    
    if (rooms.find(roomNumber) == rooms.end()) {
        return false;
    }
    
    if (rooms[roomNumber]->checkIn(guestId, time(nullptr))) {
        status = CHECKED_IN;
        createBill();
        
        return true;
    }
    
    return false;
}

bool Booking::checkOut(u_int64_t bookingId) {
    if (this->bookingId != bookingId) {
        return false;
    }
    
    if (status != CHECKED_IN) {
        return false;
    }
    
    if (rooms.find(roomNumber) == rooms.end()) {
        return false;
    }
    
    for (const auto& billPair : bills) {
        if (billPair.second->getRemainingAmount() > 0) {
            return false;
        }
    }
    
    if (rooms[roomNumber]->checkOut(guestId, time(nullptr))) {
        status = CHECKED_OUT;
        
        for (auto& billPair : bills) {
            billPair.second->close();
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

    return CANCELLED;
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

u_int64_t Booking::createBill() {
    Bill* newBill = new Bill(bookingId, guestId);
    u_int64_t billId = newBill->getBillId();
    bills[billId] = newBill;
    
    int stayDuration = static_cast<int>((checkOutDate - checkInDate) / 86400);
    double roomRate = 5000.0; //впоследствии будет добавлена возможность изменения стоимости номера
    newBill->addItem("Проживание в номере " + std::to_string(roomNumber), roomRate * stayDuration);
    
    return billId;
}

Bill* Booking::getBill(u_int64_t billId) {
    if (bills.find(billId) != bills.end()) {
        return bills[billId];
    }
    return nullptr;
}

bool Booking::deleteBill(u_int64_t billId) {
    if (bills.find(billId) == bills.end()) {
        return false;
    }
    
    if (!bills[billId]->getIsClosed()) {
        delete bills[billId];
        bills.erase(billId);
        return true;
    }
    
    return false;
}

std::vector<Bill*> Booking::getAllBills() const {
    std::vector<Bill*> allBills;
    for (const auto& billPair : bills) {
        allBills.push_back(billPair.second);
    }
    return allBills;
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

u_int64_t Booking::bookResource(u_int64_t resourceId, time_t startTime, time_t endTime) {
    if (status != CHECKED_IN) {
        return 0;
    }
    
    auto it = resources.find(resourceId);
    if (it == resources.end()) {
        return 0;
    }
    
    return it->second->book(guestId, startTime, endTime);
}

bool Booking::addResourceToBill(u_int64_t billId, u_int64_t resourceId, u_int64_t bookingId) {
    Bill* bill = getBill(billId);
    if (bill == nullptr) {
        return false;
    }
    
    auto it = resources.find(resourceId);
    if (it == resources.end()) {
        return false;
    }
    
    Resource* resource = it->second;
    double charge = resource->calculateCharge(bookingId);
    
    if (charge <= 0.0) {
        return false;
    }
    
    bill->addItem("Услуга: " + resource->getName(), charge);
    return true;
}
