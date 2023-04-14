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
    virtual ~Resource() {};

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
    Asset(): resource(nullptr) {};

    virtual ~Asset(){};

    virtual bool Initialize() = 0;

    virtual ResourceType GetAsset() const { return resource; }

    virtual void SetAsset(ResourceType &asset) { resource = asset; }

protected:
    ResourceType resource;
};

class AssetManager
{
public:
    AssetManager();

    virtual ~AssetManager();

    virtual AssetType GetAssetType(const std::string &name);

    struct AssetData
    {
        AssetType type;
        std::shared_ptr<Resource> resource;
    };

    virtual AssetData GetResource(const std::string &name);

    virtual bool AddResource(const std::string &label, AssetData &&resource);

    virtual void DeleteResource(const std::string &name);

    virtual bool IsAssetExist(const std::string &name);

protected:
    std::unordered_map<std::string, AssetData> assets;
};
