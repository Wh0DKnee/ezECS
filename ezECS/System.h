#pragma once

class EntityManager;

class System
{
public:
	virtual void update(EntityManager& component_manager, float delta_seconds) = 0;
};

class MovementSystem : public System
{
public:
	virtual void update(EntityManager& component_manager, float delta_seconds) override;
};