#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>

#include "timing.h"

using namespace sf;
using namespace std;


void initClients(Client *clients);

void listener();

DataType DataTypeOfIndex(int i);

int indexOfDataType(DataType dataType);

void _sendPacket(Packet packet, IpAddress originIp, unsigned short originPort, bool ignoreOrigin, int clientsCount, Client *clients);

void _convexShapeToPacket(ConvexShape convexShape, Packet &packet);

void sendGameLaunchCallback(Client *clients, int id);

void sendData(Asteroid asteroid, int id, IpAddress originIp, unsigned short originPort, bool ignoreOrigin, Client *clients);

//void sendData(MissileType missileType, IpAddress originIp, unsigned short originPort, bool ignoreOrigin);

void sendData(Missile missile, IpAddress originIp, unsigned short originPort, bool ignoreOrigin, Client *clients);

void sendData(Spaceship spaceship, IpAddress originIp, unsigned short originPort, bool ignoreOrigin, int id, Client *clients);

void getData(Packet packet, DataType dataType, IpAddress originIp, unsigned short originPort);

int getGameID(IpAddress clientIP, unsigned short port, Client *clientsTableToSearchIn);
