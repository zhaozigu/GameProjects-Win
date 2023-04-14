#include "AssetManager.hpp"

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

AssetType AssetManager::GetAssetType(const std::string &name)
{
    auto it = assets.find(name);
    if (it != assets.end())
    {
        return it->second->GetAssetType();
    }

    return AssetType::Unknown;
}

SharedResource AssetManager::GetResource(const std::string &name)
{
    auto it = assets.find(name);
    if (it != assets.end())
    {
        return it->second;
    }
    return std::make_shared<Resource>();
}

bool AssetManager::AddResource(const std::string &label, SharedResource &&resource)
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
