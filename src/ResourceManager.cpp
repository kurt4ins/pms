#include "ResourceManager.hpp"

void ResourceManager::addResource(std::shared_ptr<Resource> resource) {
    resources[resource->getId()] = resource;
}

bool ResourceManager::removeResource(int id) {
    return resources.erase(id) > 0;
}

std::shared_ptr<Resource> ResourceManager::getResource(int id) const {
    auto it = resources.find(id);
    return (it != resources.end()) ? it->second : nullptr;
}

std::vector<std::shared_ptr<Resource>> ResourceManager::getAvailableResources() const {
    std::vector<std::shared_ptr<Resource>> available;
    for (const auto& [id, res] : resources) {
        if (res->getAvailability()) {
            available.push_back(res);
        }
    }
    return available;
}