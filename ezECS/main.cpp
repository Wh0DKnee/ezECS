#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include <SFML/Graphics.hpp>

struct Position
{
	Position() = default;
	Position(float x, float y) : x(x), y(y) {}
	float x;
	float y;
};

struct Velocity
{
	Velocity() = default;
	Velocity(float x, float y) : x(x), y(y) {}
	float x;
	float y;
};

struct Circle
{
	Circle() = default;
	Circle(float radius) : radius(radius) {}

	float radius;
};

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

struct RenderSystem : public System
{
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
			shape.setOrigin(circle.radius, circle.radius);
			shape.setPosition(sf::Vector2f(pos.x, pos.y));
			render_target.draw(shape);
		}
	}

	sf::RenderTarget& render_target;
};

struct BounceSystem : public System
{
	BounceSystem(sf::RenderTarget& render_target) : render_target(render_target)
	{
		requireComponent<Position>();
		requireComponent<Velocity>();
		requireComponent<Circle>();
	}

	virtual void update(EntityManager& entity_manager, float delta_seconds) override
	{
		for (auto& entity : getMatchingEntities(entity_manager))
		{
			const Position& pos = entity_manager.getComponent<Position>(entity);
			Velocity& velocity = entity_manager.getComponent<Velocity>(entity);
			const Circle& circle = entity_manager.getComponent<Circle>(entity);

			const sf::View& window = render_target.getView();
			sf::Vector2f window_size = window.getSize();
			if ((pos.x <= circle.radius && velocity.x < 0.f) 
				|| (pos.x + circle.radius >= window_size.x && velocity.x > 0.f))
			{
				velocity.x *= -1.f;
			}
			if ((pos.y <= circle.radius && velocity.y < 0.f) 
				|| (pos.y + circle.radius >= window_size.y && velocity.y > 0.f))
			{
				velocity.y *= -1.f;
			}
		}
	}

	const sf::RenderTarget& render_target;
};

int main()
{
	const unsigned int window_width = 800;
	const unsigned int window_height = 600;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Bouncing Circles");

	EntityManager entity_manager;
	entity_manager.registerComponent<Position>();
	entity_manager.registerComponent<Velocity>();
	entity_manager.registerComponent<Circle>();

	const int num_circles = 100;
	const int min_radius = 3;
	const int max_radius = 50;
	const int min_velocity_x = 100;
	const int max_velocity_x = 1000;
	const int min_velocity_y = 100;
	const int max_velocity_y = 1000;

	srand(time(nullptr));
	for (int i = 0; i < num_circles; ++i)
	{
		Entity& entity = entity_manager.createEntity();
		entity_manager.addComponent(entity, Position(rand() % window_width, rand() % window_height));
		entity_manager.addComponent(entity, Velocity(rand() % max_velocity_x + min_velocity_x, rand() % max_velocity_y + min_velocity_y));
		entity_manager.addComponent(entity, Circle(rand() % max_radius + min_radius));
	}

	MovementSystem movement_system;
	RenderSystem render_system(window);
	BounceSystem bounce_system(window);

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
		std::cout << delta_seconds << std::endl;

		movement_system.update(entity_manager, delta_seconds);
		render_system.update(entity_manager, delta_seconds);
		bounce_system.update(entity_manager, delta_seconds);
		window.display();
	}
}