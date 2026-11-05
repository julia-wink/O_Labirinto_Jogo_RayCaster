#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <array>
#include <cmath>

class RayCaster {
	//o contextpr é usado para assegurar constantes ou realizar calculos já na compilação
	static constexpr int map_widht = 16, map_height = 16;
	//matriz 16x1
	std::array<std::string, map_height> map;
	float player_x, player_y, player_angle, 
		fov, depth;

	sf::RenderWindow window;
	sf::Clock clock;
	sf::Texture wall_texture, bg_texture;
	sf::Sprite bg;
	void events(), draw(), draw_minimap(),
		keys(), rays(), render();
	public:
		RayCaster();
		void run();
 
	

};