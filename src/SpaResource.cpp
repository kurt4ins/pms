#include "SpaResource.hpp"
#include <chrono>
#include <cmath>
#include <stdexcept>

using namespace std;
using namespace std::chrono;

SpaResource::SpaResource(int id, const string& name, int maxSlots, float price, const string& desc)
    : Resource(id, name, desc), maxSlots(maxSlots), pricePerHour(price) 
{
    if (maxSlots <= 0) {
        throw invalid_argument("Max slots must be positive");
    }
    if (price < 0.0f) {
        throw invalid_argument("Price per hour cannot be negative");
    }
}

float SpaResource::calculateCost(const time_point<system_clock>& start, 
                               const time_point<system_clock>& end) const 
{
    if (end <= start) {
        return 0.0f;
    }

    const auto time_diff = end - start;
    
    const auto duration_in_seconds = duration_cast<seconds>(time_diff);
    const double full_hours = ceil(duration_in_seconds.count() / 3600.0);
    
    if (full_hours > numeric_limits<float>::max() / pricePerHour) {
        throw overflow_error("Cost calculation overflow");
    }

    return static_cast<float>(full_hours) * pricePerHour;
}