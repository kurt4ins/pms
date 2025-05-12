#pragma once
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include "Guest.hpp"
#include "DateTime.hpp"

struct BillItem {
    std::string description;
    double amount;
    time_t date;
    bool isPaid;
    
    BillItem(const std::string& desc, double amt, time_t dt) : 
        description(desc), amount(amt), date(dt), isPaid(false) {}
};

class Bill {
private:
    u_int64_t billId;
    u_int64_t bookingId;
    u_int64_t guestId;
    std::vector<BillItem> items;
    double totalAmount;
    double paidAmount;
    time_t createdDate;
    time_t lastUpdatedDate;
    bool isClosed;
    static u_int64_t nextBillId;
    
public:
    Bill(u_int64_t bookingId, u_int64_t guestId);
    
    ~Bill();
    
    void addItem(const std::string& description, double amount);
    bool removeItem(int itemIndex);
    std::vector<BillItem> getItems() const;

    bool payItem(int itemIndex);
    bool payAll();
    bool payAmount(double amount);
    
    u_int64_t getBillId() const;
    u_int64_t getBookingId() const;
    u_int64_t getGuestId() const;
    double getTotalAmount() const;
    double getPaidAmount() const;
    double getRemainingAmount() const;
    time_t getCreatedDate() const;
    time_t getLastUpdatedDate() const;
    bool getIsClosed() const;
    
    void close();
    
    std::string generateBillSummary() const;
};
