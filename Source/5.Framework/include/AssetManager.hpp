#pragma once
#include "Asset.hpp"

using SharedResource = std::shared_ptr<class Resource>;

class AssetManager
{
public:
    AssetManager();

    virtual ~AssetManager();

    virtual ResourceType GetAssetType(const std::string &name);

    virtual SharedResource GetAsset(const std::string &name);

    virtual bool AddAsset(const std::string &label, SharedResource &&resource);

    virtual void DeleteResource(const std::string &name);

    virtual bool IsAssetExist(const std::string &name);

protected:
    std::unordered_map<std::string, SharedResource> assets;
};
