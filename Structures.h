#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Constants.h"

using namespace sf;

typedef struct {
	IpAddress ip;
	bool isConnected;
	bool isInGame;
	float score;
	int matchMadeWithClient;
	int matchMakeScoreRange;
	unsigned short port;
	int gameID;
} Client;


typedef struct {
	float speed;
	float angle;
	float angularVelocity;
	bool exists;
	bool hasEnteredWindow;
	int type;
	ConvexShape shape;
} Asteroid;

typedef struct {
	//int damage;
	//int radius;
	char type;
} MissileType;

typedef struct {
	float speed;
	float angle;
	bool exists;
	int damage;
	int originSpaceship;
	MissileType type;
	ConvexShape shape;
} Missile;

typedef struct {
	int id;
	MissileType missile;
	Sprite sprite;
	Texture texture;
	Vector2f speed;
	//int weapon;
	bool isAlive;
	int shootDelay;
} Spaceship;

