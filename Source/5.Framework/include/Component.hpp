#pragma once
#include <memory>

class Actor;

class Component : public std::enable_shared_from_this<Component>
{
public:
    /// \brief Constructor
    /// \param owner A weak_ptr to the owning Actor
    /// \param updateOrder The order in which the component will be updated (lower values update earlier)
    Component(std::weak_ptr<Actor> owner, int updateOrder = 100);

    /// \brief Destructor
    virtual ~Component(){};

    /// \brief Process input for the component
    /// \param keyState The current state of the keyboard
    virtual void ProcessInput(const uint8_t *keyState){};

    /// \brief Update the component with a given delta time
    /// \param deltaTime The time elapsed since the last update
    virtual void Update(float deltaTime){};

    /// \brief Add the component to its owner
    virtual void AddComponent();

    /// \brief Remove the component from its owner
    virtual void RemoveComponent();

    /// \brief Get the update order of the component
    /// \return The update order as an integer
    virtual int GetUpdateOrder() const { return mUpdateOrder; }

    /// \brief Get a shared_ptr to this component
    /// \return A shared_ptr to this component
    virtual std::shared_ptr<Component> GetSelf();

protected:
    /// \brief A weak_ptr to the owning Actor
    std::weak_ptr<Actor> mOwner;
    /// \brief The update order of the component
    int mUpdateOrder;
};
