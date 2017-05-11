#include <cstring>
#include <iostream>
#include <time.h>

#include "Game.h"

#define SpriteVelocityBot 0.05
#define SpriteVelocityTop 0.05
#define SpriteVelocityBlock 0.07

#define DISTANCIA_BODY 70
#define DISTANCIA_ATTACK 150

// 1_i_x_y // WELCOME_id_x // Welcome al client, amb la seva id i posicions
// 2_i_x_y // POSITION_id_x // Posicio del jugador id
// 3 // Ping
// Protocol: https://docs.google.com/spreadsheets/d/152EPpd8-f7fTDkZwQlh1OCY5kjCTxg6-iZ2piXvEgeg/edit?usp=sharing

int FoggOffset = 0;
int DireccioAtacJugador1 = 0; // 0=Idle 1=Top 2=Mid 3=Bot
int DireccioAtacJugador2 = 0; // 0=Idle 1=Top 2=Mid 3=Bot

bool distancia=false;
bool distanciaAtac=false;

enum State {
	connect,	// Per conectarse al servidor
	send,		// enviar paraula nova y que comenci partida
	play,		// mentres els jugadors estan escribint. comproba si sacaba el temps i si algú ha encertat la partida
	points,		// Envia les puntuacions als jugadors y actualitza els seus logs
	win			// el joc sacaba
};


int main()
{
	//-- UDP --//

	//sf::IpAddress ip = sf::IpAddress::IpAddress("127.0.0.1"); //sf::IpAddress::getLocalAddress();
	//unsigned short serverPort = 5000;
	//sf::UdpSocket socket;
	//std::queue<sf::IpAddress> ipQueue;							// On es guarden les ips no asignades dels nous jugadors
	//std::queue<unsigned short> portQueue;						// On es guarden els ports no asigntas dels nous jugadors
	//std::queue<std::string> serverCommands;						// Misatges del servidor per anar executant
	//sf::Mutex mutex;											// Per evitar varis accesos a les cues
	//std::string command;										// el misatge que envia als clients
	//Send sender;												// Sender per enviar misatges
	//ClientReceive receiver;										// Receiver per rebre constanment misatges
	//sf::Thread thread(&ClientReceive::ReceiveCommands, &receiver);	// Thread per el receiver
	//std::vector<Player> player;	// Vector de jugadors

	//sender.command = &command;
	//sender.socket = &socket;

	//receiver.commands = &serverCommands;
	//receiver.socket = &socket;
	//receiver.mutex = &mutex;
	//receiver.players = &player;

	//std::cout << "Port: ";										// Demanem port al client
	//unsigned short port;
	//std::cin >> port;											// Guardem el port del client
	//sf::Socket::Status status = socket.bind(port);				// Bind al port del client
	//if (status != sf::Socket::Done) {							// Si falla el bind, acaba el programa
	//	std::cout << "Error al intent de bind" << std::endl;
	//	return -1;
	//}

	//-- CLIENT --//

	MessageManager protocol;	// Per llegir els segons el protocol
	Timer timerConnect;			// timer per intentar conectarse a servidor	
	State state = play;		// Comencem en connect per que es conecti al server
	Player playertmp;			// Amb el tmp es guardara a ell mateix i als altres en el vector player
	Player playerEnemy; //El player enemic

	playertmp.x = 270;
	playertmp.y = 150;

	playerEnemy.x = 800;
	playerEnemy.y = 150;

	//player.push_back(playertmp);
	//player.push_back(playertmp);

	//-- GAME --//
	

	//carreguem imatges
	//fons
	sf::Texture texture;
	if (!texture.loadFromFile("../Resources/Fons.png")) {
		std::cout << "Can't load the image file" << std::endl;
		return -1;
	}
	sf::Sprite fons; // fons
	fons.setTexture(texture);
	//gespa
	sf::Texture gespaText;
	if (!gespaText.loadFromFile("../Resources/front.png")) { //FALLA AQUI
		std::cout << "Can't load the image file" << std::endl;
		return -1;
	}
	sf::Sprite herba; // fons
	herba.setTexture(gespaText);
	herba.setPosition(0, 600);
	
	//boira




	sf::Texture BoiraText;
	if (!BoiraText.loadFromFile("../Resources/moviment.png")) {
		std::cout << "Can't load the image file" << std::endl;
		return -1;
	}
	sf::Sprite Boira; // fons
	Boira.setTexture(BoiraText);
	sf::Sprite Boira2; // fons
	Boira2.setTexture(BoiraText);



	//Jugador 1 
	//TOP

	sf::Texture p1TextTop;
	if (!p1TextTop.loadFromFile("../Resources/SpriteEsquerrav1.png")) {
		std::cout << "Can't load the image file" << std::endl;
		return -1;
	}
	//Animation idle
	Animation idleAnimation1T;
	idleAnimation1T.setSpriteSheet(p1TextTop);
	idleAnimation1T.addFrame(sf::IntRect(0,0,650,650));

	//Animacio attack top player 1
	Animation attackAnimationTop1T;
	attackAnimationTop1T.setSpriteSheet(p1TextTop);
	attackAnimationTop1T.addFrame(sf::IntRect(652*1, 0, 650, 650));
	attackAnimationTop1T.addFrame(sf::IntRect(652*2, 0, 650, 650));
	attackAnimationTop1T.addFrame(sf::IntRect(652*3, 0, 650, 650));
	attackAnimationTop1T.addFrame(sf::IntRect(652*4, 0, 650, 650));
	attackAnimationTop1T.addFrame(sf::IntRect(652*5, 0, 650, 650));//5
	attackAnimationTop1T.addFrame(sf::IntRect(652*6, 0, 650, 650));
	attackAnimationTop1T.addFrame(sf::IntRect(652*7, 0, 650, 650));
	attackAnimationTop1T.addFrame(sf::IntRect(652*8, 0, 650, 650));
	attackAnimationTop1T.addFrame(sf::IntRect(652*9, 0, 650, 650));
	attackAnimationTop1T.addFrame(sf::IntRect(652*0,652, 650, 650));//10
	attackAnimationTop1T.addFrame(sf::IntRect(652*1, 652, 650, 650));
	attackAnimationTop1T.addFrame(sf::IntRect(652*2, 652, 650, 650));
	attackAnimationTop1T.addFrame(sf::IntRect(652*3, 652, 650, 650));

	//Animacio attack mid player 1
	Animation attackAnimationMid1T;
	attackAnimationMid1T.setSpriteSheet(p1TextTop);
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 4, 652, 650, 650));
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 5, 652, 650, 650));
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 6, 652, 650, 650));
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 7, 652, 650, 650));
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 8, 652, 650, 650));//5
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 9, 652, 650, 650));
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 0, 652*2, 650, 650));
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 1, 652*2, 650, 650));
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 2, 652*2, 650, 650));
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 3, 652*2, 650, 650));//10
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 4, 652*2, 650, 650));
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 5, 652*2, 650, 650));
	attackAnimationMid1T.addFrame(sf::IntRect(652 * 6, 652*2, 650, 650));

	//Animacio attack bot player 1
	Animation attackAnimationBot1T;
	attackAnimationBot1T.setSpriteSheet(p1TextTop);
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 7, 652 * 2, 650, 650));
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 8, 652 * 2, 650, 650));
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 9, 652 * 2, 650, 650));
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 0, 652 * 3, 650, 650));
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 1, 652 * 3, 650, 650));//5
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 2, 652 * 3, 650, 650));
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 3, 652 * 3, 650, 650));
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 4, 652 * 3, 650, 650));
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 5, 652 * 3, 650, 650));
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 6, 652 * 3, 650, 650));//10
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 7, 652 * 3, 650, 650));
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 8, 652 * 3, 650, 650));
	attackAnimationBot1T.addFrame(sf::IntRect(652 * 9, 652 * 3, 650, 650));

	//Animacio Bloqueig
	Animation BlockAnimation1T;
	BlockAnimation1T.setSpriteSheet(p1TextTop);
	BlockAnimation1T.addFrame(sf::IntRect(652 * 2, 0, 650, 650));
	BlockAnimation1T.addFrame(sf::IntRect(652 * 3, 0, 650, 650));
	BlockAnimation1T.addFrame(sf::IntRect(652 * 4, 0, 650, 650));
	BlockAnimation1T.addFrame(sf::IntRect(652 * 5, 0, 650, 650));
	BlockAnimation1T.addFrame(sf::IntRect(652 * 6, 0, 650, 650));//5
	BlockAnimation1T.addFrame(sf::IntRect(652 * 7, 0, 650, 650));
	BlockAnimation1T.addFrame(sf::IntRect(652 * 8, 0, 650, 650));


	//Animation* currentAnimation1T = &idleAnimation1T;
	

	// set up AnimatedSprite
	AnimatedSprite p1Top(sf::seconds(SpriteVelocityTop), true, false); //(sf::Time frameTime, bool paused, bool looped)
	p1Top.setPosition(sf::Vector2f(playertmp.x-325,playertmp.y));
	p1Top.play(idleAnimation1T);

	//Bot
	sf::Texture p1TextBot;
	if (!p1TextBot.loadFromFile("../Resources/PassosEsq.png")) {
		std::cout << "Can't load the image file" << std::endl;
		return -1;
	}
	//Animation idle
	Animation idleAnimation1B;
	idleAnimation1B.setSpriteSheet(p1TextBot);
	idleAnimation1B.addFrame(sf::IntRect(0, 0, 500, 380));

	//Animation Pas Ofensiu
	Animation pas1B;
	pas1B.setSpriteSheet(p1TextBot);
	pas1B.addFrame(sf::IntRect(502 * 1, 382 * 0, 500, 380));
	pas1B.addFrame(sf::IntRect(502 * 0, 382 * 1, 500, 380));
	pas1B.addFrame(sf::IntRect(502 * 1, 382 * 1, 500, 380));
	pas1B.addFrame(sf::IntRect(502 * 0, 382 * 2, 500, 380));
	pas1B.addFrame(sf::IntRect(502 * 1, 382 * 2, 500, 380));//-·--·--·--
	pas1B.addFrame(sf::IntRect(502 * 0, 382 * 2, 500, 380));
	pas1B.addFrame(sf::IntRect(502 * 1, 382 * 1, 500, 380));
	pas1B.addFrame(sf::IntRect(502 * 0, 382 * 1, 500, 380));
	pas1B.addFrame(sf::IntRect(502 * 1, 382 * 0, 500, 380));

	//Animation* currentAnimation1B = &idleAnimation1B;


	// set up AnimatedSprite
	AnimatedSprite p1Bot(sf::seconds(SpriteVelocityBot), true, false); //(sf::Time frameTime, bool paused, bool looped)
	p1Bot.setPosition(sf::Vector2f(playertmp.x - 325, playertmp.y+275)); //275 d'alçada per compensar amb la cintura
	p1Bot.play(idleAnimation1B);

	//Jugador 2 
	//TOP

	sf::Texture p2TextTop;
	if (!p2TextTop.loadFromFile("../Resources/SpriteJugadorDreta.png")) {
		std::cout << "Can't load the image file" << std::endl;
		return -1;
	}
	//Animation idle
	Animation idleAnimation2T;
	idleAnimation2T.setSpriteSheet(p2TextTop);
	idleAnimation2T.addFrame(sf::IntRect(0, 0, 650, 650));

	//Animacio attack top player 1
	Animation attackAnimationTop2T;
	attackAnimationTop2T.setSpriteSheet(p2TextTop);
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 1, 0, 650, 650));
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 2, 0, 650, 650));
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 3, 0, 650, 650));
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 4, 0, 650, 650));
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 5, 0, 650, 650));//5
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 6, 0, 650, 650));
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 7, 0, 650, 650));
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 8, 0, 650, 650));
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 9, 0, 650, 650));
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 0, 652, 650, 650));//10
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 1, 652, 650, 650));
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 2, 652, 650, 650));
	attackAnimationTop2T.addFrame(sf::IntRect(652 * 3, 652, 650, 650));

	//Animacio attack mid player 1
	Animation attackAnimationMid2T;
	attackAnimationMid2T.setSpriteSheet(p2TextTop);
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 4, 652, 650, 650));
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 5, 652, 650, 650));
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 6, 652, 650, 650));
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 7, 652, 650, 650));
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 8, 652, 650, 650));//5
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 9, 652, 650, 650));
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 0, 652 * 2, 650, 650));
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 1, 652 * 2, 650, 650));
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 2, 652 * 2, 650, 650));
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 3, 652 * 2, 650, 650));//10
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 4, 652 * 2, 650, 650));
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 5, 652 * 2, 650, 650));
	attackAnimationMid2T.addFrame(sf::IntRect(652 * 6, 652 * 2, 650, 650));

	//Animacio attack bot player 1
	Animation attackAnimationBot2T;
	attackAnimationBot2T.setSpriteSheet(p2TextTop);
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 7, 652 * 2, 650, 650));
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 8, 652 * 2, 650, 650));
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 9, 652 * 2, 650, 650));
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 0, 652 * 3, 650, 650));
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 1, 652 * 3, 650, 650));//5
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 2, 652 * 3, 650, 650));
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 3, 652 * 3, 650, 650));
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 4, 652 * 3, 650, 650));
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 5, 652 * 3, 650, 650));
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 6, 652 * 3, 650, 650));//10
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 7, 652 * 3, 650, 650));
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 8, 652 * 3, 650, 650));
	attackAnimationBot2T.addFrame(sf::IntRect(652 * 9, 652 * 3, 650, 650));

	//Animacio Bloqueig
	Animation BlockAnimation2T;
	BlockAnimation2T.setSpriteSheet(p2TextTop);
	BlockAnimation2T.addFrame(sf::IntRect(652 * 2, 0, 650, 650));
	BlockAnimation2T.addFrame(sf::IntRect(652 * 3, 0, 650, 650));
	BlockAnimation2T.addFrame(sf::IntRect(652 * 4, 0, 650, 650));
	BlockAnimation2T.addFrame(sf::IntRect(652 * 5, 0, 650, 650));
	BlockAnimation2T.addFrame(sf::IntRect(652 * 6, 0, 650, 650));//5
	BlockAnimation2T.addFrame(sf::IntRect(652 * 7, 0, 650, 650));
	BlockAnimation2T.addFrame(sf::IntRect(652 * 8, 0, 650, 650));


	//Animation* currentAnimation1T = &idleAnimation2T;


	// set up AnimatedSprite
	AnimatedSprite p2Top(sf::seconds(SpriteVelocityTop), true, false); //(sf::Time frameTime, bool paused, bool looped)
	p2Top.setPosition(sf::Vector2f(playerEnemy.x - 325, playerEnemy.y));
	p2Top.play(idleAnimation2T);

	//Bot
	sf::Texture p2TextBot;
	if (!p2TextBot.loadFromFile("../Resources/PassosDreta.png")) {
		std::cout << "Can't load the image file" << std::endl;
		return -1;
	}
	//Animation idle
	Animation idleAnimation2B;
	idleAnimation2B.setSpriteSheet(p2TextBot);
	idleAnimation2B.addFrame(sf::IntRect(0, 0, 500, 380));

	//Animation Pas Ofensiu
	Animation pas2B;
	pas2B.setSpriteSheet(p2TextBot);
	pas2B.addFrame(sf::IntRect(502 * 1, 382 * 0, 500, 380));
	pas2B.addFrame(sf::IntRect(502 * 0, 382 * 1, 500, 380));
	pas2B.addFrame(sf::IntRect(502 * 1, 382 * 1, 500, 380));
	pas2B.addFrame(sf::IntRect(502 * 0, 382 * 2, 500, 380));
	pas2B.addFrame(sf::IntRect(502 * 1, 382 * 2, 500, 380));//-·--·--·--
	pas2B.addFrame(sf::IntRect(502 * 0, 382 * 2, 500, 380));
	pas2B.addFrame(sf::IntRect(502 * 1, 382 * 1, 500, 380));
	pas2B.addFrame(sf::IntRect(502 * 0, 382 * 1, 500, 380));
	pas2B.addFrame(sf::IntRect(502 * 1, 382 * 0, 500, 380));

	//Animation* currentAnimation1B = &idleAnimation1B;


	// set up AnimatedSprite
	AnimatedSprite p2Bot(sf::seconds(SpriteVelocityBot), true, false); //(sf::Time frameTime, bool paused, bool looped)
	p2Bot.setPosition(sf::Vector2f(playerEnemy.x - 325, playerEnemy.y + 275)); //275 d'alçada per compensar amb la cintura
	p2Bot.play(idleAnimation2B);



	sf::Clock frameClock;//Preparem el temps



	/*sf::Sprite blue; // jugador
	blue.setTexture(texture2);
	blue.setOrigin(20, 20);*/
	sf::Texture texture3;
	if (!texture3.loadFromFile("../Resources/Fucsia.png")) {
		std::cout << "Can't load the image file" << std::endl;
		return -1;
	}
	sf::Sprite fucsia; // enemic
	fucsia.setTexture(texture3);
	fucsia.setOrigin(20, 20);

	/*sf::Font font;
	if (!font.loadFromFile("cour.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}*/

	sf::Vector2i screenDimensions(1600, 900);											// Dimensions pantalles
	sf::RenderWindow window;															// Creem la finestra del joc
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Aoi Samurai");	// Obrim la finestra del joc
	window.setFramerateLimit(60); //FrameRate

	//thread.launch();																	// Comencem thread receive

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		sf::Time frameTime = frameClock.restart();
		switch (state) {
		case connect:

			//if (timerConnect.Check()) {
			//	command = protocol.CreateMessage(1, 0, 0, 0);
			//	sender.SendMessages(ip, serverPort);
			//	timerConnect.Start(5000);
			//	timerConnect.Stop();
			//}
			//if (!serverCommands.empty()) {
			//	switch (protocol.GetType(serverCommands.front())) {

			//	case 1:
			//		player[0].id = protocol.GetSubType(serverCommands.front());
			//		player[0].x = protocol.GetPosition(serverCommands.front());
			//		serverCommands.pop();
			//		break;

			//	case 2:
			//		player[1].id = protocol.GetSubType(serverCommands.front());
			//		player[1].x = protocol.GetPosition(serverCommands.front());
			//		serverCommands.pop();

			//		command = protocol.CreateMessage(2, player[0].id, 0, 0);
			//		sender.SendMessages(ip, serverPort);
			//		state = play;
			//		break;

			//	default:
			//		break;

			//	}
		/*	}		
			break;*/

		case send:
			break;
		case play:
		/*	std::cout << "Hit!!" << std::endl;
			std::cout << "Hit!!" << std::endl;
			std::cout << "Hit!!" << std::endl;*/


			sf::Keyboard key;
			if (key.isKeyPressed(sf::Keyboard::Right)) {
				if(!(playerEnemy.x - (playertmp.x+2)<DISTANCIA_BODY)){
					playertmp.x += 2;
					p1Bot.play(pas1B);
				}
			}
			if (key.isKeyPressed(sf::Keyboard::Left)) {
				if (!(playerEnemy.x - (playertmp.x - 2)<DISTANCIA_BODY)) {
					playertmp.x -= 2;
					p1Bot.play(pas1B);

				}
			
				
			}

			if (!p1Top.isPlaying()) { //Per no cancelar las animacions Cooldown animacions
				DireccioAtacJugador1 = 0;
				p1Top.setFrameTime(sf::seconds(SpriteVelocityTop));

				if (key.isKeyPressed(sf::Keyboard::Q)) {
					DireccioAtacJugador1 = 1;
					p1Top.play(attackAnimationTop1T);

				}	if (key.isKeyPressed(sf::Keyboard::W)) {
					DireccioAtacJugador1 = 2;
					p1Top.play(attackAnimationMid1T);

				}	if (key.isKeyPressed(sf::Keyboard::E)) {
					DireccioAtacJugador1 = 3;
					p1Top.play(attackAnimationBot1T);
				}
			}

			if (!p2Top.isPlaying()) { //Per no cancelar las animacions Cooldown animacions
				DireccioAtacJugador2 = 0;
				p2Top.setFrameTime(sf::seconds(SpriteVelocityTop));

				if (key.isKeyPressed(sf::Keyboard::A)) {
					DireccioAtacJugador2 = 1;
					p2Top.play(attackAnimationTop2T);

				}	if (key.isKeyPressed(sf::Keyboard::S)) {
					DireccioAtacJugador2 = 2;
					p2Top.play(attackAnimationMid2T);

				}	if (key.isKeyPressed(sf::Keyboard::D)) {
					DireccioAtacJugador2 = 3;
					p2Top.play(attackAnimationBot2T);
				}
			}


			//if (!serverCommands.empty()) {
			//	switch (protocol.GetType(serverCommands.front())) {

			//	case 1: // NO TINDRIA QUE REBRE 1
			//		break;

			//	case 2: 		
			//		command = protocol.CreateMessage(2, player[0].id, 0, 0);
			//		sender.SendMessages(ip, serverPort);
			//		serverCommands.pop();
			//		break;

			//	case 3:
			//		command = "3" + std::to_string(player[0].id);
			//		sender.SendMessages(ip, serverPort);
			//		serverCommands.pop();
			//	default:
			//		break;

			//	}
			//}

			//Check de distancies

			if (playerEnemy.x - playertmp.x<DISTANCIA_BODY) {
				distancia = true;
			}
			else {
				distancia = false;
			}

			if (playerEnemy.x - playertmp.x<DISTANCIA_ATTACK) {
				distanciaAtac = true;
			}
			else {
				distanciaAtac = false;
			}
			// MOMENT DE L'ATAC
			if ((p1Top.m_currentFrame == 12 || p2Top.m_currentFrame == 12) && distanciaAtac) { //Xequejem colliders al atacar
				if (p1Top.m_currentFrame == 12) { //Jugador inicia el atac //Activar animacio de Bloqueig canviar timing per el jugador que bolequeja
					if (DireccioAtacJugador2 == DireccioAtacJugador1) {
						p1Top.setFrameTime(sf::seconds (SpriteVelocityBlock));
						p1Top.play(BlockAnimation1T);
						p2Top.play(BlockAnimation2T);
						std::cout << "Jugador 2 bloqueja" << std::endl;
							
					}
					else { //JUGADOR 1 Guanya
						p1Top.pause();
						state = points;
						std::cout << "guanya jugador 1" << std::endl;
					}
				}
				else { // Jugador dos inicia el atac
					if (DireccioAtacJugador2 == DireccioAtacJugador1) {
						p2Top.setFrameTime(sf::seconds(SpriteVelocityBlock));
						p1Top.play(BlockAnimation1T);
						p2Top.play(BlockAnimation2T);
						std::cout << "Jugador 1 bloqueja" << std::endl;

					}
					else { //JUGADOR 1 Guanya
						p2Top.pause();
						state = points;
						std::cout << "guanya jugador 2" << std::endl;
					}
				}
				

			}

			break;

		case points:
			break;
		case win:
			break;
		}

		window.draw(fons);	// Pintem el fons
		FoggOffset++;
		if (FoggOffset * 0.3 >= 1600) {
			FoggOffset = 0;
		}
		else {
			Boira.setPosition(1600 - FoggOffset * 0.3, 0);
		}
		Boira2.setPosition(-FoggOffset * 0.3, 0);

		window.draw(Boira);
		window.draw(Boira2);
		//if (player.size() > 0) { 
			//p1Bot.play(*currentAnimation1B);
			
			//
			p1Bot.update(frameTime);
			p1Bot.setPosition(playertmp.x, 150 + 275);
			window.draw(p1Bot);
			//p1Top.play(*currentAnimation1T);
			p1Top.update(frameTime);
			p1Top.setPosition(playertmp.x, 150);
			window.draw(p1Top);
			
			//P2
			p2Bot.update(frameTime);
			p2Bot.setPosition(playerEnemy.x + 150, playerEnemy.y + 275); //Aquests 150 en x son la desviació del sprite de les cames
			window.draw(p2Bot);

			p2Top.update(frameTime);
			p2Top.setPosition(playerEnemy.x, playerEnemy.y);
			window.draw(p2Top);// pintem el jugador
			//if (player.size() > 1) {
				/*for (int i = 1; i < player.size(); i++)
				{
					fucsia.setPosition(player[i].x, player[i].y);
					window.draw(fucsia); // pintem el enemic
				}*/
			//}
	//	}//Niebla


		window.draw(herba);
		
		window.display();		// Mostrem per la finestra
		window.clear();			// Netejem finestra
	}
	//receiver.stopReceive = false;
	//thread.terminate();
	return 0;
}

