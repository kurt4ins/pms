#include <iostream>
#include <sstream>
#include "includes/pms.hpp"

void Guest::setFirstName(std::string name) {
    // тут типо проверки
    this->firstName = name; // this == self
}

void Guest::setSecondName(std::string name) {
    this->secondName = name;
}

void Guest::setPhone(std::string phoneNum) {
    this->phone = phoneNum;
}

void Guest::setPassport(u_int64_t passportNum) {
    this->passport = passportNum;
}

std::string Guest::getFullName(){
    return this->firstName + this->secondName;
}

void Guest::showInfo() {
    std::cout << "ID: " << this->id << std::endl;
    std::cout << "Имя: " << this->firstName << std::endl;
    std::cout << "Фамилия: " << this->secondName << std::endl;
    std::cout << "Номер телефона: " << this->phone << std::endl;
    std::cout << "Email: " << this->email << std::endl;
    std::cout << "Номер паспорта: " << this->passport << std::endl;
    std::cout << "Дата рождения: " << this->date << std::endl;
    std::cout << "Бонусы: " << this->loyalPoint << std::endl;
    std::cout << std::endl;
}

// Реализация конструктора
Guest::Guest(
        u_int64_t id,
        std::string fname,
        std::string sname,
        std::string phone,
        std::string email,
        u_int64_t passport,
        std::string date,
        u_int loyalPoint
    ) {
    this->id = id;
    this->firstName = fname;
    this->secondName = sname;
    this->phone = phone;
    this->email = email;
    this->passport = passport;
    this->date = date;
    this->loyalPoint = loyalPoint;
}

DateTime::DateTime(int64 sec = 0): sec(sec) {};

DateTime::DateTime(int dd, int mm, int yy, int hour, int min, int sec):
    sec((yy - 1970) * 31536000 + (mm - 1) * 2592000 + (dd - 1) * 86400 + hour * 3600 + min * 60 + sec) {};

void DateTime::str_to_date(std::string str) {

}

std::string DateTime::date_to_str(DateTime) {
    return ("%02d-%02d-%04d %02d:%02d");
}