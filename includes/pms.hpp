// property management system
#pragma once
#include <string>

class Guest {
private: // модификатор доступа
    u_int64_t id;
    std::string firstName; // свойства/поле
    std::string secondName;
    std::string phone;
    std::string email;
    u_int64_t passport;
    std::string date;
    u_int loyalPoint;
    

public:
    Guest(
        u_int64_t id,
        std::string fname,
        std::string sname,
        std::string phone,
        std::string email,
        u_int64_t passport,
        std::string date,
        u_int loyalPoint
    ); //конструктор
    void setFirstName(std::string);
    void setSecondName(std::string);
    void setPhone(std::string);
    void setPassport(u_int64_t);

    std::string getFullName();
    void showInfo();
};

class DateTime {
private:
    u_int day;
    u_int month;
    u_int year;

    // u_int hour;
    // u_int minute;

public:
    void string_to_date(std::string);
    void date_to_string(DateTime);
};