#include "../includes/Bill.hpp"
#include <sstream>

u_int64_t Bill::nextBillId = 1;

Bill::Bill(u_int64_t bookingId, u_int64_t guestId) : 
    bookingId(bookingId),
    guestId(guestId),
    totalAmount(0.0),
    paidAmount(0.0),
    isClosed(false) {
    billId = nextBillId++;
    createdDate = time(nullptr);
    lastUpdatedDate = createdDate;
}

Bill::~Bill() {}

void Bill::addItem(const std::string& description, double amount) {
    if (isClosed) {
        return;
    }
    
    time_t now = time(nullptr);
    items.emplace_back(description, amount, now);
    totalAmount += amount;
    lastUpdatedDate = now;
}

bool Bill::removeItem(int itemIndex) {
    if (isClosed || itemIndex < 0 || itemIndex >= items.size()) {
        return false;
    }
    
    if (items[itemIndex].isPaid) {
        paidAmount -= items[itemIndex].amount;
    }
    
    totalAmount -= items[itemIndex].amount;
    items.erase(items.begin() + itemIndex);
    lastUpdatedDate = time(nullptr);
    
    return true;
}

std::vector<BillItem> Bill::getItems() const {
    return items;
}

bool Bill::payItem(int itemIndex) {
    if (isClosed || itemIndex < 0 || itemIndex >= items.size() || items[itemIndex].isPaid) {
        return false;
    }
    
    items[itemIndex].isPaid = true;
    paidAmount += items[itemIndex].amount;
    lastUpdatedDate = time(nullptr);
    
    return true;
}

bool Bill::payAll() {
    if (isClosed) {
        return false;
    }
    
    for (auto& item : items) {
        if (!item.isPaid) {
            item.isPaid = true;
            paidAmount += item.amount;
        }
    }
    
    lastUpdatedDate = time(nullptr);
    
    if (paidAmount >= totalAmount) {
        isClosed = true;
    }
    
    return true;
}

bool Bill::payAmount(double amount) {
    if (isClosed || amount <= 0) {
        return false;
    }
    
    double remainingPayment = amount;
    
    for (auto& item : items) {
        if (!item.isPaid && remainingPayment > 0) {
            if (remainingPayment >= item.amount) {
                item.isPaid = true;
                paidAmount += item.amount;
                remainingPayment -= item.amount;
            } else {
                continue;
            }
        }
    }
    
    lastUpdatedDate = time(nullptr);
    
    if (paidAmount >= totalAmount) {
        isClosed = true;
    }
    
    return true;
}

u_int64_t Bill::getBillId() const {
    return billId;
}

u_int64_t Bill::getBookingId() const {
    return bookingId;
}

u_int64_t Bill::getGuestId() const {
    return guestId;
}

double Bill::getTotalAmount() const {
    return totalAmount;
}

double Bill::getPaidAmount() const {
    return paidAmount;
}

double Bill::getRemainingAmount() const {
    return totalAmount - paidAmount;
}

time_t Bill::getCreatedDate() const {
    return createdDate;
}

time_t Bill::getLastUpdatedDate() const {
    return lastUpdatedDate;
}

bool Bill::getIsClosed() const {
    return isClosed;
}

void Bill::close() {
    isClosed = true;
    lastUpdatedDate = time(nullptr);
}

std::string Bill::generateBillSummary() const {
    std::stringstream ss;
    
    ss << "Счет #" << billId << " (";
    ss << (isClosed ? "Закрыт" : "Открыт") << ")\n";
    ss << "Бронирование: " << bookingId << ", Гость: " << guestId << "\n";
    ss << "Дата: " << DateTime(createdDate).dateToStr() << "\n\n";
    
    for (const auto& item : items) {
        ss << (item.isPaid ? "✓ " : "✗ ") << item.description << " — "
           << item.amount << " руб." << "\n";
           
    }
    
    ss << "\n";
    ss << "ИТОГО: " << totalAmount << " руб.\n";
    ss << "ОПЛАЧЕНО: " << paidAmount << " руб.\n";
    ss << "К ОПЛАТЕ: " << (totalAmount - paidAmount) << " руб.";
    
    return ss.str();
}
