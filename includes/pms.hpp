// property management system
#pragma once
#include <string>

typedef unsigned long long int64;

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
    int64 sec;

public:
    DateTime(int64 sec = 0);
    DateTime(int dd, int mm, int yy, int hour = 0, int min = 0, int sec = 0);

    void str_to_date(std::string);
    std::string date_to_str(DateTime);
};
