#include <iostream>
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include <SFML/Graphics.hpp>

class MovementSystem : public System
{
public:
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

class RenderSystem : public System
{
public:
	RenderSystem(sf::RenderTarget& render_target) : render_target(render_target)
	{
		requireComponent<Position>();
		requireComponent<Circle>();
	}

	virtual void update(EntityManager& entity_manager, float delta_seconds) override
	{
		for (auto& entity : getMatchingEntities(entity_manager))
		{
			const Position& pos = entity_manager.getComponent<Position>(entity);
			const Circle& circle = entity_manager.getComponent<Circle>(entity);
			
			sf::CircleShape shape;
			shape.setRadius(circle.radius);
			shape.setFillColor(sf::Color::Green);
			shape.setPosition(sf::Vector2f(pos.x, pos.y));
			render_target.draw(shape);
		}
	}

	sf::RenderTarget& render_target;
};

int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "circles", sf::Style::Fullscreen);

	EntityManager entity_manager;
	Entity& entity = entity_manager.createEntity();
	entity_manager.addComponent(entity, Position(0, 0));
	entity_manager.addComponent(entity, Velocity(100, 100));
	entity_manager.addComponent(entity, Circle(20));
	MovementSystem movement_system;
	RenderSystem render_system(window);

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) 
		{
			switch (event.type) 
			{
			case sf::Event::Closed:
			case sf::Event::KeyPressed:
				window.close();
				break;
			default:
				break;
			}
		}
		window.clear();
		sf::Time delta_time = clock.restart();
		float delta_seconds = delta_time.asSeconds();

		movement_system.update(entity_manager, delta_seconds);
		render_system.update(entity_manager, delta_seconds);
		window.display();
	}

	for (size_t i = 0; i < 10; ++i)
	{
		movement_system.update(entity_manager, 1.f);
		Position p = entity_manager.getComponent<Position>(entity);
		std::cout << p.x << " " << p.y << std::endl;
	}

	std::cout << ComponentMaskGetter<Position>::getComponentMask() << std::endl;
	std::cout << ComponentMaskGetter<Velocity>::getComponentMask() << std::endl;
	
}