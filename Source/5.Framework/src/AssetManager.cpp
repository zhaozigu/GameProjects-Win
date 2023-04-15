#include "AssetManager.hpp"

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

ResourceType AssetManager::GetAssetType(const std::string &name)
{
    auto it = assets.find(name);
    if (it != assets.end())
    {
        return it->second->GetResourceType();
    }

    return ResourceType::Unknown;
}

SharedResource AssetManager::GetAsset(const std::string &name)
{
    auto it = assets.find(name);
    if (it != assets.end())
    {
        return it->second;
    }
    return std::make_shared<Resource>();
}

bool AssetManager::AddAsset(const std::string &label, SharedResource &&resource)
{
    if (!IsAssetExist(label))
    {
        assets.insert({label, resource});
    }
    return false;
}

void AssetManager::DeleteResource(const std::string &label)
{
    auto it = assets.find(label);
    if (it != assets.end())
    {
        assets.erase(it);
    }
}

bool AssetManager::IsAssetExist(const std::string &label)
{
    return assets.find(label) != assets.end();
}
