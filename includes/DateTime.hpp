#pragma once
#include <string>

class DateTime {
private:
    u_int64_t sec;

public:
    DateTime(u_int64_t sec = 0);
    DateTime(int dd, int mm, int yy, int hour = 0, int min = 0, int sec = 0);

    DateTime strToDate();
    std::string dateToStr();
};