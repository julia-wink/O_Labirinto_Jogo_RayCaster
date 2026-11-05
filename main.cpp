#include "raycaster.hpp"
#include <cstdlib>

void draw_rect() {
	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML window");
	sf::RectangleShape rect(sf::Vector2f(30, 30));
	rect.setFillColor(sf::Color::Red);
	rect.setPosition({ 30, 30 });
	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			auto* keyPress = event->getIf<sf::Event::KeyPressed>();
			if (keyPress != nullptr) {
				sf::Vector2f tam = rect.getSize();
				float tam_x = tam.x;
				float tam_y = tam.y;

				if (keyPress->code == sf::Keyboard::Key::Right) {
					rect.setSize({ tam_x * 2.f, tam_y * 2.f });
				}
				if (keyPress->code == sf::Keyboard::Key::Left) {
					rect.setSize({ tam_x / 2.f, tam_y / 2.f });
				}

			}
		
		}
		window.clear();
		window.draw(rect);
		window.display();
	
	}
}

int main() {
	//ponteiro inteligente, use --> para acessar ponteiros em vez de objetos direto
	auto ray = std::make_unique<RayCaster>();
	ray->run();
	//draw_rect();
	return EXIT_SUCCESS;

}

