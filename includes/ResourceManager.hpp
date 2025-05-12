#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <memory>
#include <unordered_map>
#include "Resource.hpp"

class ResourceManager {
private:
    std::unordered_map<int, std::shared_ptr<Resource>> resources;

public:
    void addResource(std::shared_ptr<Resource> resource);
    bool removeResource(int id);
    std::shared_ptr<Resource> getResource(int id) const;
    std::vector<std::shared_ptr<Resource>> getAvailableResources() const;
};

#endif 