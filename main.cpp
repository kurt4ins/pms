#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "src/DateTime.cpp"
#include "src/Guest.cpp"

std::vector<Guest> loadGuests() {
    std::vector<Guest> guests;
    std::ifstream file("guests.txt");
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);

        char delim;
        u_int64_t id;
        std::string fname;
        std::string sname;
        std::string phone;
        std::string email;
        u_int64_t passport;
        std::string date;
        u_int loyalPoint;

        ss >> id >> delim;
        std::getline(ss, fname, ',');
        std::getline(ss, sname, ',');
        std::getline(ss, phone, ',');
        std::getline(ss, email, ',');
        ss >> passport >> delim;
        std::getline(ss, date, ',');
        ss >> loyalPoint;

        Guest guest(id, fname, sname, phone, email, passport, date, loyalPoint);
        guests.push_back(guest);
        
    }

    return guests;           

}

int main() {
    auto guests = loadGuests();
    for (auto guest : guests) {
        guest.showInfo();
    }
    return 0;
}