#include "../includes/DateTime.hpp"

DateTime::DateTime(u_int64_t sec): sec(sec) {};

DateTime::DateTime(int dd, int mm, int yy, int hour, int min, int sec):
    sec((yy - 1970) * 31536000 + (mm - 1) * 2592000 + (dd - 1) * 86400 + hour * 3600 + min * 60 + sec) {};

DateTime DateTime::strToDate() {
    return DateTime();
}

std::string DateTime::dateToStr() {
    u_int64_t tmp = sec;
    int yy = 1970 + tmp / 31536000;
    tmp %= 31536000;
    
    int mm = 1 + tmp / 2592000;
    tmp %= 2592000;
    
    int dd = 1 + tmp / 86400;
    tmp %= 86400;
    
    int hour = tmp / 3600;
    tmp %= 3600;

    int min = tmp / 60;

    tmp %= 60;
    int sec = tmp;
    
    char str[32];
    snprintf(str, sizeof(str), "%02d-%02d-%04d %02d:%02d:%02d", dd, mm, yy, hour, min, sec);
    
    return std::string(str);
}
