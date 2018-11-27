/*#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>

#include "Horus Client.h"

using namespace sf;
using namespace std;

#define MAX_CLIENT 10

int amain()
{
	runClient(1234);
	return 0;
}

UdpSocket socket;

void getMessages() {
	Packet packet;

	while (true) {
		IpAddress senderIP;
		unsigned short senderPort;
		socket.receive(packet, senderIP, senderPort);
		Uint32 integer;
		double doubleValue;
		packet >> doubleValue >> integer;
		cout << "OMG JUST RECEIVED THAT -> " << integer << " <- and -> " << doubleValue << " <- !!! " << endl;
	}
}

void runClient(unsigned int port) {
	system("color d");

	IpAddress server;
	string username;
	do {
		cout << "Addresse du serveur auquel se connecter : ";
		cin >> server;
	} while (server == IpAddress::None);

	cout << "Connecting to " << server << ":" << port << "..." << endl;

	Thread thread(getMessages);
	thread.launch();

	Packet packet;
	bool isNewConnection = true;
	packet << "HELLO! I'm a client :)";

	// Wait here for server callback
	while (true) {
		socket.send(packet, server, port);
		system("pause");
		packet.clear();
	}
}*/