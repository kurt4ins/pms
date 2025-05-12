#pragma once
#include <string>

class Guest {
private:
    u_int64_t id;
    std::string firstName;
    std::string lastName;
    std::string phone;
    std::string email;
    u_int64_t passport;
    std::string date;
    u_int loyalPoint;
    

public:
    Guest(
        u_int64_t id,
        std::string fname,
        std::string lname,
        std::string phone,
        std::string email,
        u_int64_t passport,
        std::string date,
        u_int loyalPoint
    );
    void setFirstName(std::string);
    void setLastName(std::string);
    void setPhone(std::string);
    void setPassport(u_int64_t);

    std::string getFullName();
    u_int64_t getId();
    u_int getLoyalPoints();

    void addLoyaltyPoints(u_int points);
    void showInfo();  
};

