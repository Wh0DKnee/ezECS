#pragma once

class System
{
public:
	virtual void update(float delta_seconds) = 0;
};

class MovementSystem : public System
{
	virtual void update(float delta_seconds) override;
};