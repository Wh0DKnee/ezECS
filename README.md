# ezECS
ezECS is an [entity component system](https://en.wikipedia.org/wiki/Entity_component_system). 

The goal of this project is to provide a light-weight and easy to use system, while keeping cache-friendliness in mind. Specifically, this means that components are contiguous in memory, ensuring fast iteration over components of a given type.

## Usage

### Creating Components
```c++
struct Position
{
	Position(float x, float y) : x(x), y(y) {}
	float x = 0.f;
	float y = 0.f;
};
```

### Creating Systems

```c++
struct MovementSystem : public System
{
	MovementSystem()
	{
		requireComponent<Position>();
		requireComponent<Velocity>();
	}

	virtual void update(EntityManager& entity_manager, float delta_seconds) override
	{
		for (auto& entity : getMatchingEntities(entity_manager))
		{
			Position& pos = entity_manager.getComponent<Position>(entity);
			const Velocity& velocity = entity_manager.getComponent<Velocity>(entity);

			pos.x += velocity.x * delta_seconds;
			pos.y += velocity.y * delta_seconds;
		}
	}
};
```

### Game Loop
```c++
EntityManager entity_manager;
MovementSystem movement_system;
for(int i = 0; i < 10; ++i)
{
  movement_system.update(entity_manager, delta_seconds);
}
```
