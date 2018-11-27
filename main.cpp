#include <SFML/Network.hpp>
#include <thread>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Game.h"


// onConnect:
/// client sends score to server # Implement DataType Score and senddata(data)
/// client sends server the missileType in use # Done each frame within spaceship # done
// inGame:
/// client sends coords (movement) # done
/// server callback with other player's coords
/// client creates missile then send it to server # done
// onCollideAsteroidWithMissileOrSpaceship
/// client tells server (collision or death) # do it at the end
// internal:
/// asteroid generation + management # done????

using namespace sf;
using namespace std;

// GLOBALS BEGIN
UdpSocket socket;

Partie games[MAX_CLIENT / MAX_CLIENTS_PER_GAME];

Client waitingClients[MAX_CLIENT];
/*int clientsCount = 0;*/

// GLOBALS END

int main(int argCount, char *args[]) {

	//Thread packetReceiver(listener);
	Client neutralClient;

	neutralClient.gameID = MAX_CLIENT + 1;
	neutralClient.ip = "0";
	neutralClient.isConnected = false;
	neutralClient.isInGame = false;
	neutralClient.matchMadeWithClient = MAX_CLIENT + 1;
	neutralClient.matchMakeScoreRange = 0;
	neutralClient.port = 0;
	neutralClient.score = 0;

	initClients(waitingClients);
	cout << "Starting Horus server 1.1..." << endl;

	unsigned short port = 1234;

	if (argCount == 2) {
		if (strcmp(args[1], "alt") == 0) {
			port = 25565;
		}
	}

	if (socket.bind(port) != Socket::Done)
		return 0;

	cout << "Horus Server started on ip [" << IpAddress().getPublicAddress(seconds(5)) << ":" <<
		port << "]! Waiting for clients..." << endl;

	initClients(waitingClients);
	std::thread packetReceiver(listener);

	while (true) 
	{
		games[0].setStartStatus(true);
		int k;
		for (int i = 0; i < MAX_CLIENT; i++)
		{
			/*sleep(milliseconds(10));
			if (i==0)
				cout << "client" << i << "is" << waitingClients[i].isConnected << "connected and " << waitingClients[i].isInGame << "in game" << endl;*/
			if (waitingClients[i].isConnected && !waitingClients[i].isInGame)
			{
				for (int j = 0; j < MAX_CLIENT; j++)
				{
					if (waitingClients[j].isConnected && !waitingClients[j].isInGame && i != j)
					{
						float scoreDifference = waitingClients[i].score - waitingClients[j].score;

						if (scoreDifference < waitingClients[i].matchMakeScoreRange && scoreDifference < waitingClients[j].matchMakeScoreRange)
						{
							waitingClients[i].matchMadeWithClient = j;
							waitingClients[j].matchMadeWithClient = i;

							k = 0;

							while (games[k].getGameStatus() == true)
							{
								k++;
							}
							games[k].setStartStatus(true);

							games[k].setClientStatus(waitingClients[i], 0);
							waitingClients[i].isInGame = true;
							waitingClients[i].gameID = k;

							games[k].setClientStatus(waitingClients[j], 1);
							waitingClients[j].isInGame = true;
							waitingClients[j].gameID = k;

							games[k].localGameEngine.launch();

							cout << "Game " << k << " started" << endl;
						}
					}
				}
			}

			if (waitingClients[i].isConnected && !waitingClients[i].isInGame)
			{
				waitingClients[i].matchMakeScoreRange += MATCHMAKING_SCORE_PRECISION_COEFFICIENT;
			}
		}


		for (int i = 0; i < MAX_CLIENT / MAX_CLIENTS_PER_GAME; i++)
		{
			for (int j = 0; j < MAX_CLIENT; j++)
			{
				if (!games[i].getGameStatus() && waitingClients[j].gameID == i)
				{
					cout << "MANGER\n" << endl;

					waitingClients[j].gameID = MAX_CLIENT + 1;
					waitingClients[j].isConnected = false;
					waitingClients[j].isInGame = false;
					waitingClients[j].port = 0 ;
					waitingClients[j].ip = "0"; 
					waitingClients[j].matchMakeScoreRange = 0;
					waitingClients[j].score = 1000;
					
					games[i].setClientStatus(neutralClient, 0);
					games[i].setClientStatus(neutralClient, 1);
				}
			}
		}

	}

}


void initClients(Client *clients)
{
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		clients[i].isConnected = 0;
		clients[i].isInGame = 0;
		clients[i].matchMakeScoreRange = 0;
		clients[i].score = 1000;
	}
}

void listener() {

	Packet packet;
	IpAddress senderIp;
	unsigned short senderPort;

	while (true) {
		//printf("I'm gonna wait for some data\n");
		packet.clear();

		socket.receive(packet, senderIp, senderPort);

		//printf("Received some data \n");

		if (!packet.endOfPacket()) {

			Uint32 type;
			packet >> type;
			DataType dataType = DataTypeOfIndex(type);

			bool isNewConnection = true;
			for (int i = 0; i < MAX_CLIENT; i++) {
				if (senderIp == waitingClients[i].ip && senderPort == waitingClients[i].port && waitingClients[i].isConnected) {
					isNewConnection = false;
					break;
				}
			}

			if (isNewConnection) {
				for (int i = 0; i < MAX_CLIENT; i++) {
					if (!waitingClients[i].isConnected) {
						waitingClients[i].ip = senderIp;
						waitingClients[i].port = senderPort;
						waitingClients[i].isConnected = true;
						waitingClients[i].isInGame = false;
						//cout << "client" << i << "is" << waitingClients[i].isConnected << "connected and " << waitingClients[i].isInGame << "in game" << endl;
						//TODO: Score
						break;
					}
				}
				cout << "A new connection has been detected from " << senderIp << ":" << senderPort << "!" << endl;
			}
			else {
				//TODO: recevoir les déplacements du spaceship d'un client et l'envoyer à l'autre client # TODO IN LISTENER
				//TODO: recevoir la potentielle destruction d'un astéroïde et l'appliquer dans asteroids[i].exists # TODO IN LISTENER
				//TODO: recevoir la potentielle mort d'un spaceship, l'appliquer dans spaceship
				//		(1 ou 2, attention ici c'est 1 ou 2 alors que pour tout le reste le i c'est 0 ou 1)
				//		et l'envoyer à l'autre client # TODO IN LISTENER
				//TODO: recevoir les potentiels missiles d'un client et les envoyer à l'autre client #Todo

				getData(packet, dataType, senderIp, senderPort);
			}
		}
	}
}


DataType DataTypeOfIndex(int i) {
	return static_cast<DataType>(i);
}

int indexOfDataType(DataType dataType) {
	return static_cast<int>(dataType);
}

// Data types implementation

void _sendPacket(Packet packet, IpAddress originIp, unsigned short originPort, bool ignoreOrigin, int clientsCount, Client *clients) {
	//cout << "Processing..." << endl;
	for (int i = 0; i < clientsCount; i++) {
		if (ignoreOrigin && originIp == clients[i].ip && originPort == clients[i].port)
			continue;
		socket.send(packet, clients[i].ip, clients[i].port);
		/*cout << "Packet sent to " << clients[i].ip << ":" <<
			clients[i].port << "." << endl;*/
	}
}

void _convexShapeToPacket(ConvexShape convexShape, Packet &packet) {
	// Points Count
	int count = convexShape.getPointCount();
	packet << Uint32(count);

	// Points
	/// Float
	for (int i = 0; i < count; i++) {
		packet << convexShape.getPoint(i).x << convexShape.getPoint(i).y;
	}

	// Color
	/// R, G, B = Uint8
	packet << convexShape.getFillColor().r << convexShape.getFillColor().g <<
		convexShape.getFillColor().b;

	// Position
	/// Float
	packet << convexShape.getPosition().x << convexShape.getPosition().y;
}

// Native types

/*void sendData(int data) {
	Packet packet;
	DataType dataType = DataType::Int;

	packet << Uint32(dataType) << data;
	_sendPacket(packet);
}

void sendData(float data) {
	Packet packet;
	DataType dataType = DataType::Float;

	packet << Uint32(dataType) << data;
	_sendPacket(packet);
}

void sendData(double data) {
	Packet packet;
	DataType dataType = DataType::Double;

	packet << Uint32(dataType) << data;
	_sendPacket(packet);
}

void sendData(bool data) {
	Packet packet;
	DataType dataType = DataType::Bool;

	packet << Uint32(dataType) << data;
	_sendPacket(packet);
}

void sendData(string data) {
	Packet packet;
	DataType dataType = DataType::String;

	packet << Uint32(dataType) << data;
	_sendPacket(packet);
}*/

// Converted types

void sendGameLaunchCallback(Client *clients, int id) {
	Packet packet;
	DataType dataType = DataType::Connection;
	packet << Uint32(indexOfDataType(dataType));

	_sendPacket(packet, "0", 0, false, MAX_CLIENTS_PER_GAME, clients);
}

/// Ignore Origin = Don't send to origin
void sendData(Asteroid asteroid, int id, IpAddress originIp, unsigned short originPort, bool ignoreOrigin, Client *clients) {
	Packet packet;
	DataType dataType = DataType::Asteroid;
	packet << Uint32(indexOfDataType(dataType));

	packet << Uint32(id);

	// Native properties
	packet << asteroid.speed << asteroid.angle << asteroid.angularVelocity <<
		asteroid.exists << asteroid.hasEnteredWindow << Uint32(asteroid.type);

	_convexShapeToPacket(asteroid.shape, packet);

	_sendPacket(packet, originIp, originPort, ignoreOrigin, MAX_CLIENTS_PER_GAME, clients);
}

/*void sendData(MissileType missileType, IpAddress originIp, unsigned short originPort, bool ignoreOrigin) {
	Packet packet;
	DataType dataType = DataType::MissileType;
	packet << Uint32(indexOfDataType(dataType));

	packet << Uint8(missileType.type);

	_sendPacket(packet, originIp, originPort, ignoreOrigin);
}*/

void sendData(Missile missile, IpAddress originIp, unsigned short originPort, bool ignoreOrigin, Client *clients) {
	Packet packet;
	DataType dataType = DataType::Missile;
	packet << Uint32(indexOfDataType(dataType));

	packet << missile.speed << missile.angle << missile.damage;
	packet << Uint32(missile.damage) << Uint32(missile.originSpaceship);

	packet << Uint8(missile.type.type);
	// Position
	/// Float
	packet << missile.shape.getPosition().x << missile.shape.getPosition().y;

	_sendPacket(packet, originIp, originPort, ignoreOrigin, MAX_CLIENTS_PER_GAME, clients);
}

void sendData(Spaceship spaceship, IpAddress originIp, unsigned short originPort, bool ignoreOrigin, int id, Client *clients) {
	Packet packet;
	DataType dataType = DataType::Spaceship;
	packet << Uint32(indexOfDataType(dataType));

	// Id
	packet << Uint32(spaceship.id);

	// Missile (MissileType)
	packet << Uint8(spaceship.missile.type);

	// Speed
	packet << spaceship.speed.x << spaceship.speed.y;

	packet << spaceship.isAlive << Uint32(spaceship.shootDelay);

	packet << spaceship.sprite.getPosition().x << spaceship.sprite.getPosition().y << spaceship.sprite.getRotation();

	_sendPacket(packet, originIp, originPort, ignoreOrigin, MAX_CLIENTS_PER_GAME, clients);
}

void getData(Packet packet, DataType dataType, IpAddress originIp, unsigned short originPort) {

	/*Uint32 data;

	packet >> data;
	DataType dataType = DataTypeOfIndex(data);*/

	if (dataType == DataType::Asteroid) {

		Asteroid asteroid;

		Uint32 nb;
		packet >> nb;
		int id = nb;

		cout << "Asteroid " << id << " has been destroyed by " << originIp << endl ;

		packet >> asteroid.speed >> asteroid.angle >> asteroid.angularVelocity >>
			asteroid.exists >> asteroid.hasEnteredWindow;

		// ConvexShape

		Uint32 type;
		packet >> type;
		asteroid.type = type;

		Uint32 count;
		packet >> count;

		ConvexShape convexShape;
		convexShape.setPointCount(count);

		// Points
		/// Float
		float x, y;
		for (int i = 0; i < count; i++) {
			packet >> x >> y;
			convexShape.setPoint(i, Vector2f(x, y));
		}

		// Color
		/// R, G, B = Uint8
		Uint8 r, g, b;
		packet >> r >> g >> b;
		convexShape.setFillColor(Color::Color(r, g, b));

		// Position
		/// Float
		packet >> x >> y;
		convexShape.setPosition(x, y);

		asteroid.shape = convexShape;

		cout << getGameID(originIp, originPort, waitingClients) << endl;

		games[getGameID(originIp, originPort, waitingClients)].setAndSendAseroid(asteroid, originIp, originPort, id);

		// DO HERE WHAT YOU HAVE TO DO WITH IT HUGO
	}
	else if (dataType == DataType::MissileType) {
		MissileType missileType;

		Uint8 type;
		packet >> type;
		missileType.type = type;
	}
	else if (dataType == DataType::Missile) {
		Missile missile;

		float speed, angle;
		packet >> speed >> angle;
		missile.speed = speed;
		missile.angle = angle;

		bool exists;
		packet >> exists;
		missile.exists = exists;

		Uint32 damage, originSpaceship;
		packet >> damage >> originSpaceship;
		missile.damage = damage;
		missile.originSpaceship = originSpaceship;
		// ConvexShape

		Uint8 type;
		packet >> type;
		missile.type.type = type;

		// Position
		/// Float
		float x, y;
		packet >> x >> y;
		missile.shape.setPosition(x, y);
		
		games[getGameID(originIp, originPort, waitingClients)].sendMissile(missile, originIp, originPort);

		// DO HERE WHAT YOU HAVE TO DO WITH IT HUGO 2

	}
	else if (dataType == DataType::Spaceship) {
		Spaceship spaceship;

		// Id
		Uint32 id;
		packet >> id;
		spaceship.id = id;	

		// Missile (MissileType)
		Uint8 type;
		packet >> type;
		spaceship.missile.type = type;

		// Speed
		float speedX, speedY;
		packet >> speedX >> speedY;
		spaceship.speed.x = speedX;
		spaceship.speed.y = speedY;

		bool isAlive;
		packet >> isAlive;
		spaceship.isAlive = isAlive;

		Uint32 shootDelay;
		packet >> shootDelay;
		spaceship.shootDelay = shootDelay;

		float x, y, r;

		packet >> x >> y >> r;
		spaceship.sprite.setPosition(x, y);
		spaceship.sprite.setRotation(r);

		games[getGameID(originIp, originPort, waitingClients)].sendSpaceShip(spaceship, originIp, originPort, 0); // le 0 correspond à l'id du spaceship (numéro de joueur), il ne sert à rien et n'est pas utilisé pour le moment, mais on sait jamais...
	}
	else {
		cout << "DATATYPE ERROR NOT HANDLED! DataType ID : " << indexOfDataType(dataType) << " from " << originIp << endl;
	}
}

int getGameID(IpAddress clientIP, unsigned short port, Client *clientsTableToSearchIn)
{
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		if (clientsTableToSearchIn[i].ip == clientIP && clientsTableToSearchIn[i].port == port)
			return clientsTableToSearchIn[i].gameID;
	}
	return 0;
}