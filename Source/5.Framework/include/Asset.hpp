#pragma once
#include <unordered_map>
#include <string>
#include <memory>

enum class AssetType
{
    Unknown = 0,
    Texture = 1,
    Audio = 2,
    Font = 3,
};

template <AssetType AssetTypeID>
struct asset_traits
{
    virtual ~asset_traits() {}
};

class Resource
{
public:
    Resource(AssetType assetType = AssetType::Unknown) : type_(assetType){};
    virtual ~Resource(){};

    virtual AssetType GetAssetType() { return type_; }

    virtual void SetAssetType(AssetType assetType) { type_ = assetType; }

protected:
    AssetType type_;
};

template <>
struct asset_traits<AssetType::Unknown>
{
    using asset_type = Resource;
};

template <AssetType AssetTypeID>
class Asset : public Resource, asset_traits<AssetTypeID>
{
    using ResourceType = typename asset_traits<AssetTypeID>::asset_type;

public:
    Asset(AssetType assetType = AssetType::Unknown) : Resource(assetType), resource(nullptr){};

    virtual ~Asset(){};

    virtual bool Initialize() = 0;

    virtual ResourceType GetAsset() const { return resource; }

    virtual void SetAsset(ResourceType &asset) { resource = asset; }

protected:
    ResourceType resource;
};

namespace AssetCast
{
    template<typename T>
    inline T* Cast(Resource* res)
    {
        return dynamic_cast<T*>(res);
    }
}

