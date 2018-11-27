#pragma once

#define MAX_CLIENT 20
#define MATCHMAKING_SCORE_PRECISION_COEFFICIENT 10
#define FPS 60.0
#define ASTEROIDS_TABLE_LENGTH 50
#define MISSILE_TABLE_LENGTH 50
#define INITIAL_SPAWN_INTERVAL 5.0
#define SPACESHIP_SIZE 64
#define INITIAL_ASTEROID_COUNT 4
#define NB_TYPES 3
#define SPAWN_MULTIPLICATOR_FACTOR 0.95
#define MIN_SPAWN_INTEVAL 1.0
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1200
#define SPAWN_MARGIN 50
#define _USE_MATH_DEFINES
#define MAX_SPEED 3
#define FLOAT_PRECISION 100.0
#define MAX_ANGULAR_VELOCITY 1
#define NB_POINTS 6
#define MIN_ASTEROID_SPECIAL_RADIUS 5
#define MIN_ASTEROID_RADIUS 30
#define ASTEROID_SPECIAL_RANDOMNESS_RANGE 2
#define ASTEROID_RANDOMNESS_RANGE 20
#define MAX_CLIENTS_PER_GAME 2

const int PROBABILITIES[NB_TYPES] = { 12, 8, 1 };

enum struct DataType {
	Connection,

	// Native
	Int, // Uint32
	Float,
	Double,
	Bool,
	String,

	// Converted
	Asteroid,
	MissileType,
	Missile,
	Spaceship
};