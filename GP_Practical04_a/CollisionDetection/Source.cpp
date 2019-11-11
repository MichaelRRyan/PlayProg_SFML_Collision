/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 11/11/19
/// </summary>

#include <iostream>
#include <math.h>
#include <time.h>

class Point {
public:
	int x;
	int y;

	void print() {
		std::cout << "x:" << x << " y:" << y << std::endl;
	}
};

class Circle {
public:
	Point p;
	int r;

	void print() {
		std::cout << "x:" << p.x << " y:" << p.y << " r:" << r << std::endl;
	}
};

class Box {
public:
	Point p;
	int w;
	int h;

	void print()
	{ 
		std::cout << "x:" << p.x << " y:" << p.y << " w:" << w << " h:" << h << std::endl;
	}
};

int calculate_hypotenuse(Point a, Point b) {
	int result = 0;

	// Find the distance (Length) of X and Y  

	int x_Distance = b.x - a.x;
	int y_Distance = b.y - a.y;

	// Now that we have the distance (Length)
	// of x and y, use Pythagoras theorem 
	// to get the distance (length of hypotenuse) 
	// between Point a and Point b 
	result = sqrt(pow(x_Distance, 2) + pow(y_Distance, 2));

	return result;
}


int main() {

	srand(time(NULL));

	/// SET UP OBJECTS
	// Point Collision
	Point player_point;
	player_point.x = rand() % 10 + 1;
	player_point.y = rand() % 10 + 1;

	Point npc_point;
	npc_point.x = rand() % 10 + 1;
	npc_point.y = rand() % 10 + 1;

	// Circle Collision
	Circle player_circle;
	player_circle.p.x = rand() % 10 + 1;
	player_circle.p.y = rand() % 10 + 1;
	player_circle.r = rand() % 2 + 1;
	
	Circle npc_circle;
	npc_circle.p.x = rand() % 10 + 1;
	npc_circle.p.y = rand() % 10 + 1;
	npc_circle.r = rand() % 2 + 1;

	// Circle-Point Collision
	Circle cpcCircle; // Circle-point-collision circle
	cpcCircle.p.x = rand() % 10 + 1;
	cpcCircle.p.y = rand() % 10 + 1;
	cpcCircle.r = rand() % 2 + 1;

	Point cpcPoint; // Circle-point-collision Point
	cpcPoint.x = rand() % 10 + 1;
	cpcPoint.y = rand() % 10 + 1;

	// Box Collision
	Box playerBox;
	playerBox.p.x = rand() % 10 + 1;
	playerBox.p.y = rand() % 10 + 1;
	playerBox.w = rand() % 2 + 1;
	playerBox.h = rand() % 2 + 1;

	Box npcBox;
	npcBox.p.x = rand() % 10 + 1;
	npcBox.p.y = rand() % 10 + 1;
	npcBox.w = rand() % 2 + 1;
	npcBox.h = rand() % 2 + 1;

	/// CHECK COLLISIONS
	while (true) {

		// Point Collision check
		if (calculate_hypotenuse(player_point, npc_point) == 0)
		{
			std::cout << "Point Collision" << std::endl;
			player_point.print();
			npc_point.print();
		}
		else
		{
			std::cout << "No Collision" << std::endl;
			player_point.print();
			npc_point.print();
		}

		player_point.x = rand() % 10 + 1;
		player_point.y = rand() % 10 + 1;

		// Circle Collision check
		if (calculate_hypotenuse(player_circle.p, npc_circle.p) < (player_circle.r + npc_circle.r))
		{
			std::cout << "Circle Collision" << std::endl;
			player_circle.print();
			npc_circle.print();
		}
		else
		{
			std::cout << "No Collision" << std::endl;
			player_circle.print();
			npc_circle.print();
		}

		player_circle.p.x = rand() % 10 + 1;
		player_circle.p.y = rand() % 10 + 1;

		// Circle-point Collision
		if (calculate_hypotenuse(cpcCircle.p, cpcPoint) < cpcCircle.r)
		{
			std::cout << "Circle to Point Collision" << std::endl;
		}
		else
		{
			std::cout << "No Collision" << std::endl;
		}

		cpcCircle.print();
		cpcPoint.print();

		cpcCircle.p.x = rand() % 10 + 1;
		cpcCircle.p.y = rand() % 10 + 1;

		// Circle-point Collision
		if (playerBox.p.x + playerBox.w > npcBox.p.x && playerBox.p.x < npcBox.p.x + npcBox.w // Horizontal.
			&& playerBox.p.y + playerBox.h > npcBox.p.y && playerBox.p.y < npcBox.p.y + npcBox.h) // Vertical
		{
			std::cout << "Box Collision" << std::endl;
		}
		else
		{
			std::cout << "No Collision" << std::endl;
		}

		playerBox.print();
		npcBox.print();

		playerBox.p.x = rand() % 10 + 1;
		playerBox.p.y = rand() % 10 + 1;

		system("pause");
		system("cls");
	}

	return 0;
}