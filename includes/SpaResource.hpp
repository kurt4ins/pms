#ifndef SPA_RESOURCE_HPP
#define SPA_RESOURCE_HPP

#include "Resource.hpp"

class SpaResource : public Resource {
private:
    int maxSlots;
    float pricePerHour;

public:
    SpaResource(int id, const string& name, int maxSlots, float price, const string& desc = "");
    float calculateCost(const time_point<system_clock>& start, 
        const time_point<system_clock>& end) const;
};

#endif 