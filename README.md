# ezECS
ezECS is an [entity component system](https://en.wikipedia.org/wiki/Entity_component_system). 

The goal of this project is to provide a light-weight and easy to use system, while keeping cache-friendliness in mind. Specifically, this means that components are contiguous in memory, ensuring fast iteration over components of a given type. This is achieved using a [sparse sets](https://programmingpraxis.com/2012/03/09/sparse-sets/) in conjuction with the actual component pool.

## Usage

### Creating Components
```c++
struct Position
{
	Position() : x(0.f), y(0.f) {}
	Position(float x, float y) : x(x), y(y) {}
	float x;
	float y;
};
```

### Creating Systems

```c++
struct MovementSystem : public System
{
	MovementSystem()
	{
		requireComponent<Position>();
	}

	virtual void update(EntityManager& entity_manager, float delta_seconds) override
	{
		for (auto& entity : getMatchingEntities(entity_manager))
		{
			Position& pos = entity_manager.getComponent<Position>(entity);
			pos.x += 1.f;
			pos.y += 1.f;
		}
	}
};
```

### Game Loop
```c++
EntityManager entity_manager;

Entity& entity = entity_manager.createEntity();
entity_manager.addComponent(entity, Position(0.f, 0.f));

MovementSystem movement_system;

float delta_time = 1.f;
for(int i = 0; i < 10; ++i)
{
  movement_system.update(entity_manager, delta_time);
}
```

## TODO
* ~~Rewrite component pool to use [sparse sets](https://programmingpraxis.com/2012/03/09/sparse-sets/)~~
* Give system base class a function to iterate over a component type directly
* Add event system
