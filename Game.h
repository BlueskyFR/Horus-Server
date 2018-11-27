#include <SFML/System.hpp>
#include <thread>

#include "main.h"

#define M_PI 3.14159265358979323846

using namespace sf;





class Partie {

public:

	Partie()
		: localGameEngine(&Partie::gameEngineTheServerSide, this), isStarted(false)
	{

	}

	void setStartStatus(bool status);
	void setClientsIp(IpAddress ip, int internalClientId);
	void setClientsPort(unsigned short port, int internalClientId);
	void setAndSendAseroid(Asteroid asteroidToProcess, IpAddress sourceIP, unsigned short sourcePort, int id);
	void sendMissile(Missile missileToProcess, IpAddress sourceIP, unsigned short sourcePort);
	void sendSpaceShip(Spaceship spaceshipToSend, IpAddress sourceIP, unsigned short sourcePort, int id);
	IpAddress getClientIp(int clientId);
	bool getGameStatus();
	void setClientStatus(Client client, int id);

	Thread localGameEngine;

	int deadSpaceShips = 0; //évidemment à changer dès que chaque shapceship aura un numéro clairement implémenté et que donc je pourrais les identifier chacun et vérifier si ils sont morts ou non
	
private:

	
	Client clients[MAX_CLIENTS_PER_GAME];
	bool isStarted;

	Asteroid asteroids[ASTEROIDS_TABLE_LENGTH];

	void gameEngineTheServerSide();
	void asteroidsNextFrame(Asteroid *asteroids);
	Asteroid generateAsteroid(const int typeProbabilities[NB_TYPES]);
	void noAsteroidExist(Asteroid *asteroids);
	void initMissiles(Missile *missiles);
	void initSpaceship(int id, Spaceship &spaceship);
	bool asteroidManager(/*Asteroid *asteroids,*/ int &frames, float &secsInterval, bool &initGame, bool &reduce);
};