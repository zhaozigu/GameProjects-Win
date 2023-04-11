#pragma once
#include <memory>

class Actor;

class Component : public std::enable_shared_from_this<Component>
{
public:
    // 构造函数
    // (值越低的更新顺序，则组件越早更新)
    Component(std::weak_ptr<Actor> owner, int updateOrder = 100);
    // 析构函数
    virtual ~Component();
    // 通过增量时间更新组件
    virtual void Update(float deltaTime);

    virtual void AddComponent();

    virtual void RemoveComponent();

    int GetUpdateOrder() const { return mUpdateOrder; }

    virtual std::shared_ptr<Component> GetSelf()
    {
        return shared_from_this();
    }

protected:
    // 所属的角色
    std::weak_ptr<Actor> mOwner;
    // 组件的更新顺序
    int mUpdateOrder;
};
