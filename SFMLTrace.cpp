//q|DANIEL-_-CORCORAN|p
//2D Raycasting implementation in C++ using the SFML openGL library

#include "pch.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <math.h>
#define PI 3.14159265
class gameInstance {
private:
	//std::vector<sf::RectangleShape> ray; //This will have the amount of elements as the window resolution
	int draw = -20;
	int view;
public:
	gameInstance(int draw_distance, int view_angle) // Constructor
	{
		draw = draw_distance;
		view = view_angle;
	}
	void setDraw(int d) {
		draw = d;
	}
	int getDraw() {
		return draw;
	}
	void setView(int v) {
		view = v;
	}
	int getView() {
		return view;

	}
	
};
class map {
private:
	std::vector<std::vector<int>> vector;
	sf::Vector2i VectorSize;
public:
	map(int mapx, int mapy) { //default constructor
		std::vector<std::vector<int>> vector(mapx, std::vector<int>(mapy));
		VectorSize.x = mapx; VectorSize.y = mapy;
		std::cout << "Creating Walls...\n";
		std::cout << "x: " << mapx << " y: " << mapy << std::endl;
		for (int i = 0; i < mapx; i++) {
			vector[i][0] = 1;
			vector[i][mapy - 1] = 3;
		}
		for (int i = 0; i < mapy; i++) {
			vector[0][i] = 4;
			vector[mapx - 1][i] = 2;
		}
		for (int i = 0; i < mapy; i++) {
			for (int j = 0; j < mapx; j++) {
				std::cout << vector[j][i] << " ";
			}
			std::cout << std::endl;
		}
		this->vector = vector;
	}
	void place(int x, int y, int color) {
		vector[x][y] = color;
	}
	int at(int x, int y) {
		if ((x >= 0) && (x < VectorSize.x) && (y >= 0) && (y < VectorSize.y)) { //verify that the input is valid, otherwise we just say it's "empty space"
			return vector[x][y]; //This isn't a boolean because different numbers can represent different colors. 
		}
		return 0; //0 indicates empty space
	}
	sf::Vector2i size() {
		return VectorSize;
	}
};
class camera {
private:
float angle;
sf::Vector2f position;
sf::RectangleShape icon;
public:
	void setAngle(float a){
		angle = a;
	}
	float getAngle() {
		return angle;
	}
	void rotate(double a) {
		angle = angle + a;
		if (angle == 360) {
			angle = 0;
		}
		if (angle < 0 ) {
			angle = 360 + angle;
		}
	}
	void move(double x, double y) {
		position.x = position.x + x;
		position.y = position.y + y;
	}
	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}
	sf::Vector2f getPosition() {
		return position;
	}
	camera(float x, float y, float a) {
		angle = a;
		position.x = x;
position.y = y;
icon.setSize(sf::Vector2f(5, 5));
icon.setFillColor(sf::Color::Magenta);
icon.setPosition(1900 - 5 * position.x, 950 - 5 * position.y);
	}
	sf::RectangleShape getIcon(map world) {
		int iconx = 1900 - 5 * (world.size().x - position.x);
		int icony = 950 - 5 * (world.size().y - position.y);
		icon.setPosition(iconx, icony);
		icon.setOrigin(sf::Vector2f(2.5, 2.5));
		icon.setRotation(angle);
		return icon;
	}
};
//Color codes
// 1 red
// 2 blue
// 3 green
// 4 yellow
void ADD(map &world) {
	world.place(10, 10, 1);
	world.place(10, 11, 2);
	world.place(11, 10, 3);
	world.place(11, 11, 4);

	world.place(4, 4, 4);
	world.place(4, 5, 3);
	world.place(5, 4, 2);
	world.place(5, 5, 1);
	world.place(5, 6, 1);

}
std::vector<sf::RectangleShape> CreateMiniMap(gameInstance game, camera player, map world) {
	//Generate a 4*4 rectangle for every unit on the map. 
	sf::RectangleShape icon(sf::Vector2f(5, 5));
	std::vector<sf::RectangleShape> map;
	int iconx; int icony;
	for (int i = world.size().y - 1; i >= 0; i--) { //Construct from the botton corner
		for (int j = world.size().x - 1; j >= 0; j--) { //X axis...
			iconx = 1900 - 5 * (world.size().x - j);
			icony = 950 - 5 * (world.size().y - i);
			icon.setPosition(iconx, icony);
			switch (world.at(j, i)) {
			case 0:
				icon.setFillColor(sf::Color(66, 66, 66, 255));
				break;
			case 1:
				icon.setFillColor(sf::Color::Red);
				break;
			case 2:
				icon.setFillColor(sf::Color::Green);
				break;
			case 3:
				icon.setFillColor(sf::Color::Blue);
				break;
			case 4:
				icon.setFillColor(sf::Color::Yellow);
				break;
			}


			map.push_back(icon);
		}
	}






	return map;
}
void draw(std::vector<sf::RectangleShape> obj, sf::RenderWindow &window) {
	for (int i = 0; i < obj.size(); i++) {
		window.draw(obj[i]);
	}
}
std::vector<sf::RectangleShape> scan(camera player, gameInstance game, map world)
{
	std::vector<sf::RectangleShape> ray;
	sf::RectangleShape raySprite;
	int index = 0;
	double rangeDistance;
	for (float angle = player.getAngle() - (game.getView() / 2); angle <= player.getAngle() + (game.getView() / 2); angle = angle + static_cast<float>(game.getView()) / 384) {
		for (float distance = 0.5; distance < game.getDraw(); distance = distance + 0.01) {
			if (world.at(player.getPosition().x + cos(((PI / 180) * angle)) * distance, player.getPosition().y + sin(((PI / 180) * angle)) * distance) > 0) {
				ray.push_back(raySprite);
				rangeDistance = distance * cos((PI / 180) * (player.getAngle() - angle));
				ray[index].setSize(sf::Vector2f(5, 900.f * (1 / rangeDistance)));
				ray[index].setOrigin(2.5, ray[index].getSize().y / 2);
				ray[index].setPosition(sf::Vector2f(index * 5, 1080 / 2));
				float darkness = pow(0.955, rangeDistance); //This is variable, but i think o.955 works well for the intents and purposes of this project. 
				//SHADOW TEXTURE EVALUATION. Global shadow coefficient is a function of the distance of the ray impact to camera. 
				if (world.at(player.getPosition().x + cos(((PI / 180) * angle)) * distance - 0.01, player.getPosition().y + sin(((PI / 180) * angle)) * distance) == 0){
					darkness = darkness * 0.8;
				}
				if (world.at(player.getPosition().x + cos(((PI / 180) * angle)) * distance + 0.01, player.getPosition().y + sin(((PI / 180) * angle)) * distance) == 0) {
					darkness = darkness * 0.7;
				}
				if (world.at(player.getPosition().x + cos(((PI / 180) * angle)) * distance, player.getPosition().y + sin(((PI / 180) * angle)) * distance + 0.01) == 0) {
					darkness = darkness * 0.6;
				}

				switch (world.at(player.getPosition().x + cos(((PI / 180) * angle)) * distance, player.getPosition().y + sin(((PI / 180) * angle)) * distance)) {
				
				
				case 1: //FIXME
					ray[index].setFillColor(sf::Color(255 * darkness, 0, 0, 255));
					break;
				case 2:
					ray[index].setFillColor(sf::Color(0, 255 * darkness, 0, 255));
					break;
				case 3:
					ray[index].setFillColor(sf::Color(0, 0, 255 * darkness, 255));
					break;
				case 4:
					ray[index].setFillColor(sf::Color(255 * darkness, 255 * darkness, 0, 255));
					break;
				}
				distance = game.getDraw() + 1;
				index++;
			}
		}
	}
	return ray;

}
void process(camera player, gameInstance game, map world) {
constexpr auto MOVEDISTANCE = 0.5;
sf::Texture Texture_1;
Texture_1.loadFromFile("asset/1.png");
sf::IntRect rect_texture1(0, 0, 5, 100);

	std::vector<sf::RectangleShape> ray;
	std::vector<sf::RectangleShape> miniMap = CreateMiniMap(game, player, world);
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "D3DENGINE Copyright (c) 2019 Daniel P. Corcoran");
	window.setFramerateLimit(60);
	sf::Texture skyTexture; 
	if (!skyTexture.loadFromFile("sky.png")) {
		std::cout << "Error loading bg file..\n";
	}
		

	sf::RectangleShape ground(sf::Vector2f(1920, 540));	ground.setFillColor(sf::Color(86, 86, 86, 255));
	ground.setPosition(0, 540);
	sf::RectangleShape sky1(sf::Vector2f(1920, 540));// sky.setFillColor(sf::Color(12, 213, 205, 255));


	sky1.setTexture(&skyTexture);
	
	while (window.isOpen())
	{
		// Event processing
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Request for closing the window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up))||(sf::Keyboard::isKeyPressed(sf::Keyboard::W))){
				if (world.at(player.getPosition().x + 2.5*MOVEDISTANCE * cos(PI / 180 * player.getAngle()), player.getPosition().y + 2.5*MOVEDISTANCE * sin(PI / 180 * player.getAngle())) == 0) {
					player.move(MOVEDISTANCE * cos(PI / 180 * player.getAngle()),MOVEDISTANCE * sin(PI / 180 * player.getAngle()));
				}
				
			}
				//Move N
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left))||(sf::Keyboard::isKeyPressed(sf::Keyboard::A))){
				player.rotate(-5);
				
				}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down))||(sf::Keyboard::isKeyPressed(sf::Keyboard::S))){
				if (world.at(player.getPosition().x - 2.5*MOVEDISTANCE * cos(PI / 180 * player.getAngle()), player.getPosition().y - 2.5*MOVEDISTANCE * sin(PI / 180 * player.getAngle())) == 0) {
					player.move(MOVEDISTANCE * -1 * cos(PI / 180 * player.getAngle()),MOVEDISTANCE * -1 * sin(PI / 180 * player.getAngle()));
				}
			}
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right))||(sf::Keyboard::isKeyPressed(sf::Keyboard::D))){
				player.rotate(5);	
				
			}
			std::cout << "Player Position: (" << player.getPosition().x << ", " << player.getPosition().y << ")\n";
			std::cout << "Player Orientation : " << player.getAngle() << std::endl;
		}
		ray = scan(player, game, world);





		// Clear the whole window before rendering a new frame
		window.clear();
		//Draw the ground and sky background
		window.draw(ground);
		window.draw(sky1);
		
		draw(ray, window);
		draw(miniMap, window);
		window.draw(player.getIcon(world));
		// End the current frame and display its contents on screen
		window.display();




	}
}
int main()
{	
	map world(20, 20); //Gen a 30 30 map
	ADD(world);
	camera player(8, 8, 0); //X= 14 Y= 14 Angle = 45deg
	gameInstance game(world.size().x * 1.5, 80); // FOV of 100
	process(player, game, world);
	return 0;

}