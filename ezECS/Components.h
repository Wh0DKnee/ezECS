#pragma once
#include <vector>

struct Position
{
	float x;
	float y;
};

struct Velocity
{
	float x;
	float y;
};

struct Components
{
public:
	Components();

	std::vector<Position> positions;
	std::vector<Velocity> velocities;
};

