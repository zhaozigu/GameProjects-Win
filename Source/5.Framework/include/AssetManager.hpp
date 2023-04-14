#pragma once
#include "Asset.hpp"

using SharedResource = std::shared_ptr<Resource>;

class AssetManager
{
public:
    AssetManager();

    virtual ~AssetManager();

    virtual AssetType GetAssetType(const std::string &name);

    virtual SharedResource GetResource(const std::string &name);

    virtual bool AddResource(const std::string &label, SharedResource &&resource);

    virtual void DeleteResource(const std::string &name);

    virtual bool IsAssetExist(const std::string &name);

protected:
    std::unordered_map<std::string, SharedResource> assets;
};
