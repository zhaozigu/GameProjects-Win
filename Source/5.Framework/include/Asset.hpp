#pragma once
#include <unordered_map>
#include <string>
#include <memory>

enum class ResourceType
{
    Unknown = 0,
    Texture = 1,
    Audio = 2,
    Font = 3,
};

class Asset
{
public:
    Asset() {}

    virtual ~Asset() {}
    virtual bool Initialize() = 0;
};

class Resource
{
public:
    Resource(ResourceType assetType = ResourceType::Unknown) : type_(assetType) {}

    Resource(ResourceType assetType, std::shared_ptr<Asset> &&asset) : type_(assetType), resource_(asset) {}

    virtual bool Initialize() { return false; }

    virtual ~Resource() {}

    virtual ResourceType GetResourceType() const { return type_; }

    virtual void SetResourceType(ResourceType resourceType) { type_ = resourceType; }

    virtual const std::shared_ptr<Asset> &GetResource() const { return resource_; }

    virtual void SetResource(std::shared_ptr<Asset> &resource) { resource_ = resource; }

protected:
    ResourceType type_;
    std::shared_ptr<Asset> resource_;
};

class ITextureAsset : public Asset
{
public:
    ITextureAsset() {}

    virtual ~ITextureAsset() {}

    virtual int GetTexWidth() const { return texWidth_; }

    virtual int GetTexHeight() const { return texHeight_; }

    virtual void SetTexHeight(int height) { texHeight_ = height; };

    virtual void SetTexWidth(int width) { texWidth_ = width; };

protected:
    int texWidth_ = 0;
    int texHeight_ = 0;
};
