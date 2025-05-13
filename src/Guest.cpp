#include <iostream>
#include <sstream>
#include "../includes/Guest.hpp"

void Guest::setFirstName(std::string name) {
    this->firstName = name; // this == self
}

void Guest::setLastName(std::string name) {
    this->lastName = name;
}

void Guest::setPhone(std::string phoneNum) {
    this->phone = phoneNum;
}

void Guest::setPassport(u_int64_t passportNum) {
    this->passport = passportNum;
}

void Guest::addLoyaltyPoints(u_int points) {
    loyalPoint += points;
}

std::string Guest::getFullName() {
    return this->firstName + " " + this->lastName;
}

u_int64_t Guest::getId() {
    return this->id;
}

u_int Guest::getLoyalPoints() {
    return this->loyalPoint; 
}

void Guest::showInfo() {
    std::cout << "ID: " << this->id << std::endl;
    std::cout << "Имя: " << this->firstName << std::endl;
    std::cout << "Фамилия: " << this->lastName << std::endl;
    std::cout << "Номер телефона: " << this->phone << std::endl;
    std::cout << "Email: " << this->email << std::endl;
    std::cout << "Номер паспорта: " << this->passport << std::endl;
    std::cout << "Дата рождения: " << this->date << std::endl;
    std::cout << "Бонусы: " << this->loyalPoint << std::endl;
    std::cout << std::endl;
}

Guest::Guest(
        u_int64_t id,
        std::string fname,
        std::string lname,
        std::string phone,
        std::string email,
        u_int64_t passport,
        std::string date,
        u_int loyalPoint
    ) {
    this->id = id;
    this->firstName = fname;
    this->lastName = lname;
    this->phone = phone;
    this->email = email;
    this->passport = passport;
    this->date = date;
    this->loyalPoint = loyalPoint;
}