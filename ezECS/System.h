#pragma once

class ComponentManager;

class System
{
public:
	virtual void update(ComponentManager& component_manager, float delta_seconds) = 0;
};

class MovementSystem : public System
{
public:
	virtual void update(ComponentManager& component_manager, float delta_seconds) override;
};