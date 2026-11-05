#include "raycaster.hpp"
#include <iostream>
//implementacao de classe definida no header raycaster.hpp
//windown é um objeto a a qual estamos passando os parametros
RayCaster::RayCaster() : window( //função construtora
	sf::VideoMode({ 1280, 720 }),
	"RayCaster",
	sf::Style::Titlebar | sf::Style::Close
),
bg(bg_texture) {
	//inicializando alguns valores:
	player_x = player_y = { 8.f };
	player_angle = 0.f;
	fov = { 3.14159f / 4.f }; //45graus
	depth = { 16.f }; //tamanho do png da textura
	if(!wall_texture.loadFromFile("./resources/mossy.png")){
		throw std::runtime_error("Erro ao baixar mossy.png");
	};
	if (!bg_texture.loadFromFile("./resources/bg.jpg")) {
		throw std::runtime_error("Erro ao baixar bg.jpg");
	};
	map = {
		"################",
		"#..............#",
		"#...####.......#",
		"#.......####...#",
		"#..........#...#",
		"#..........#...#",
		"#..........#...#",
		"#....#######...#",
		"#....#.........#",
		"#....#.........#",
		"#....#.........#",
		"#....###########",
		"#..............#",
		"#..............#",
		"#..............#",
		"################"

	};
}

void RayCaster::events() {
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
	}
}

void RayCaster::draw() {
	window.clear();
	window.draw(bg);
	render();
	draw_minimap();
	rays();
	window.display();
}

void RayCaster::run() {
	while(window.isOpen()) {
		//loop do jogo
		events();
		keys();
		draw();
		//std::cout << "Fov: " << fov << "Player_angle: " << player_angle << " x:"  << player_x << " y:" << player_y << std::endl;

	}
}

void RayCaster::draw_minimap() {
	//desenhar o mapa
	int minimapsize = window.getSize().y - 500;
	int cellsize = minimapsize / map[0].size();
	sf::RectangleShape cell(sf::Vector2f(cellsize, cellsize));
	//size_t é um tipo de dado que armazena o maior tam possivel, a função size retorna um tipo size_t
	for (size_t y = 0; y < map.size(); y++) {
		for (size_t x = 0; x < map[y].size(); ++x) {
			if (map[y][x] == '#') {
				cell.setFillColor(sf::Color::White);
			}
			else {
				cell.setFillColor(sf::Color(50, 50, 50));
			}
			//convertendo o size_t para float
			cell.setPosition({ static_cast<float>(10 + x * cellsize),static_cast<float>(10 + y * cellsize) });
			window.draw(cell);
		}
	}

	//desenhando o jogador:
	sf::CircleShape playerDot(cellsize / 3.f);
	playerDot.setFillColor(sf::Color::Red);
	playerDot.setPosition(
		{ 10 + player_x * cellsize - playerDot.getRadius(),
		10 + player_y * cellsize - playerDot.getRadius() }
	);
	window.draw(playerDot);

	//desenhando linha da visao
	sf::Vertex line[] = {
		//começo, pos do jogador
		sf::Vertex({10 + player_x * cellsize, 10 + player_y * cellsize}, sf::Color::Red),
		//fim
		//esse 1.5 é oq define o tamanho do raio
		sf::Vertex({10 + (player_x + std::cos(player_angle) * 1.5f) * cellsize, //fim
			10 + (player_y + std::sin(player_angle) * 1.5f) * cellsize}, sf::Color::Red)
	};

	window.draw(line, 2, sf::PrimitiveType::Lines);
}
void RayCaster::keys() {
	/*quanto maior o fps do loop, menor vai ser esse clock, pq menor vai ser o tempo em relação a 1 segundo,
	,portanto mulitplicar pelo timer adapta a movimentacao dependendo de cada pc*/
	float timer = clock.restart().asSeconds();
	//direita  e esquerda altera angulo
	//cima e baixo altera pos de acordo o angulo

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) { //left
		player_angle -= 1.5f * timer;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) { //right
		player_angle += 1.5f * timer;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { //up
		player_x += std::cos(player_angle) * 5.f * timer;
		player_y += std::sin(player_angle) * 5.f * timer;
		if (map[(int)player_y][(int)player_x] == '#') {
			player_x -= std::cos(player_angle) * 5.f * timer;
			player_y -= std::sin(player_angle) * 5.f * timer;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) { //down
		player_x -= std::cos(player_angle) * 5.f * timer;
		player_y -= std::sin(player_angle) * 5.f * timer;
		if (map[(int)player_y][(int)player_x] == '#') {
			player_x += std::cos(player_angle) * 5.f * timer;
			player_y += std::sin(player_angle) * 5.f * timer;
		}
	}
}

void RayCaster::rays() {
	//repetindo calculo do cellsize:
	int cellsize = (window.getSize().y - 500)/ map[0].size();
	//numero de raios
	int numRays = 3;
	//o seno e cos de 45 graus aprox:
	float spread = 0.785f;

	for (int i = 0; i < numRays; i++) {
		/*ofsset é o espaço entre os raios
		CALC: i/numRays -1 --> calcula a porcentagem , ex: 2(3 contando 0) vai ser 1, o maximo
		-0,5 --> joga esse valor metade para tras (para espalhar dos dois lados as retas)
		*2.0f --> para que o valor seja de -1 a 1, e não de -0,5 a 0,5
		*spread --> para que o valor seja de -0,785 a 0,785, ou seja, -45 a 45 graus
		*/
		float ofsset = ((i / (float)(numRays - 1)) - 0.5f) * spread * 2.0f;
		float rayAngle = player_angle + ofsset;
		float rayX = player_x;
		float rayY = player_y;
		// o 0.01f é a velocidade do raio
		while (map[(int)rayY][(int)rayX] != '#') {
			rayX += std::cos(rayAngle) * 0.01f;
			rayY += std::sin(rayAngle) * 0.01f;

		}
		sf::Vertex line[] = {
			//começo, pos do jogador
			sf::Vertex({10 + player_x * cellsize, 10 + player_y * cellsize}, sf::Color::Yellow),
			//fim - esse 1.5 é oq define o tamanho do raio
			sf::Vertex({10 + rayX * cellsize,
				10 + rayY * cellsize}, sf::Color::Yellow)
		};

		window.draw(line, 2, sf::PrimitiveType::Lines);
	}
}

void RayCaster::render() {
	//percorre x do tam inteiro da tela:
	for (size_t x = 0; x < window.getSize().x; ++x) {
		/*Calculo para percorrer raio dentro do campo de visão de forma proporcional ao tamanho horizontal da tela(window.getSize().x)
		* Aplicação da fórmula de interpolação linear:
		* x0,x os valores inicias
		* y0,y o final que quero encontrar
		* fórmula > y = y0 + (y1 - y0) * ((x - x0) / (x1 - x0))
		* y = ray_angle,
		* y0 = player_angle - fov / 2.f --ponto inicial
		* (y1-y0) = fov --ponto final - ponto inicial
		* (x - x0) / (x1 - x0) = (x / window.getSize().x) --proporção de 0.0 a 1.0 do x na tela
		*/

		float ray_angle = (player_angle - fov / 2.f) //reta extrema esquerda do campo de visão
			+ ((float)x / (float)window.getSize().x) * fov; //prop. de 0.0 a 1.0 do x na tela * fov ( result vai de 0 a 45 graus em radianos)
		
		float dwall = 0.f; //distancia do raio até a parede
		bool hit_wall = false;
		bool is_vertical_hit = false;

		float eye_x = std::cos(ray_angle);
		float eye_y = std::sin(ray_angle);

		//loop verificação impacto:
		while (!hit_wall && dwall < depth) {
			dwall += 0.01f; //velocidade do raio
			int check_x = (int)(player_x + eye_x * dwall);
			int check_y = (int)(player_y + eye_y * dwall);
			//se atravessou mapa
			if (check_x < 0 || check_x >= map_widht || check_y < 0 || check_y >= map_height) {
				hit_wall = true;
				dwall = depth;//tam máximo é tam da imagem
			}else { //se colidiu:
				if(map[check_y][check_x] == '#') {
					hit_wall = true;
					//posicao exata do raio:
					float wall_x = player_x + eye_x * dwall;
					float wall_y = player_y + eye_y * dwall;
					/*
					* Se bateu na vertical -> wall_x é inteiro, pois as paredes são sempres posições inteiras da matriz
					* Se bateu na horizontal -> wall_y é inteiro
					*/
					is_vertical_hit = (std::fabs(wall_x - std::round(wall_x))) < (std::fabs(wall_y - std::round(wall_y)));
				}
			}
		}
		// corrige a distância quando o raio está inclinado, obtendo a distância perpendicular à direção da câmera
		float correct = dwall * std::cos(ray_angle - player_angle);
		//teto (ínicio parede) --> menor quanto maior o correct, correct pequeno = parede próxima = teto pequeno:
		int ceiling = (float)(window.getSize().y / 2.f) - window.getSize().y / correct;
		//chão (final parede):
		int floor = window.getSize().y - ceiling;
		//pos exata que o raio bateu:
		float hit_x = player_x + eye_x * dwall;
		float hit_y = player_y + eye_y * dwall;
		//trecho da textura que vai ser oega
		int tex_x = 0;

		if (is_vertical_hit) {
			// Se bateu em uma parede vertical, usamos a posição Y.
			// esse hit_y - std::floor(hit_y) pega a parte decimal que define a "porcentagem" que estamos olhando da textura
			tex_x = static_cast<int>((hit_y - std::floor(hit_y)) * wall_texture.getSize().x);
		}else {
			tex_x = static_cast<int>((hit_x - std::floor(hit_x)) * wall_texture.getSize().x);
		}

		// TriangleStrip usa 4 pontos para formar um retângulo através de dois triângulos.
		sf::VertexArray wall_strip(sf::PrimitiveType::TriangleStrip, 4);

		// x e x + 1 fazem com que essa parede tenha aproximadamente 1 pixel de largura
		wall_strip[0].position = sf::Vector2f(x, ceiling);
		wall_strip[1].position = sf::Vector2f(x + 1, ceiling);
		wall_strip[2].position = sf::Vector2f(x + 1, floor);
		wall_strip[3].position = sf::Vector2f(x, floor);

		// Define qual "trecho" da textura vai ser usada
		wall_strip[0].texCoords = sf::Vector2f(tex_x, 0);
		wall_strip[1].texCoords = sf::Vector2f(tex_x + 1, 0);
		wall_strip[2].texCoords = sf::Vector2f(tex_x + 1, wall_texture.getSize().y);
		wall_strip[3].texCoords = sf::Vector2f(tex_x, wall_texture.getSize().y);

		//fator de iluminação:
		float light_factor = std::max(0.2f, 1.0f - (dwall / depth));

		sf::Color wall_color(255 * light_factor, 255 * light_factor, 255 * light_factor);

		wall_strip[0].color = wall_color;
		wall_strip[1].color = wall_color;
		wall_strip[2].color = wall_color;
		wall_strip[3].color = wall_color;
		//indica qual textura usar:
		sf::RenderStates states;
		states.texture = &wall_texture;
		window.draw(wall_strip, states);
	
	}

	
}