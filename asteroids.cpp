#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <utility>
#include <string>

// Compilation: g++ ./asteroids.cpp -lsfml-window -lsfml-system -lsfml-graphics -fcompare-debug-second

enum WinState {
	Loss,
	Playing	
};

WinState GameState = WinState::Playing;

enum Turn {
	Static,
	Glide
};

enum AsteroidType {
	Small,
	Medium,
	Large
};

enum Status {
	Remove,
	Keep
};

const int WINDOW_LENGTH = 800;
const int WINDOW_HEIGHT = 600;

class RocketClass {
	private:
		double MOVE_SPD=500;
		double ROTATION_RAD;
		double OUTLINE_THICKNESS=1.5;
		float SIZE=3;
	public:
		sf::CircleShape Rocket;
		RocketClass(sf::Vector2f StartPos, double Rotation){
			Rocket.setRadius(SIZE);
			Rocket.setFillColor(sf::Color(0, 0, 0, 0));
			Rocket.setPosition(StartPos);
			Rocket.setRotation(Rotation);
			Rocket.setOutlineThickness(OUTLINE_THICKNESS);
			ROTATION_RAD = Rotation*3.14159265/180;
			Rocket.setOrigin(sf::Vector2f(SIZE, SIZE));
		}
		Status Update(double TimeFrame){
			Rocket.move(MOVE_SPD*TimeFrame*cos(ROTATION_RAD), MOVE_SPD*TimeFrame*sin(ROTATION_RAD));
			if (Rocket.getPosition().y > WINDOW_HEIGHT){
				return Status::Remove;
			}
			if (Rocket.getPosition().y < 0){
				return Status::Remove;
			}
			if (Rocket.getPosition().x > WINDOW_LENGTH){
				return Status::Remove;
			}
			if (Rocket.getPosition().x <0){
				return Status::Remove;
			}
			return Status::Keep;
		}
};

std::vector<RocketClass> GlobalRockets = {};

class AsteroidClass {
	private:
		double ROTATE_SPD =0.8;
		double VELOCITY =70;
		double DIRECTION=0;
		double SIZE_MULTIP=0.25;
		double OUTLINE_THICKNESS = 1;
	public:
		AsteroidType Type = AsteroidType::Large;
		sf::ConvexShape Asteroid;
		AsteroidClass(AsteroidType ConstType, sf::Vector2f Pos){
			DIRECTION = rand()%361;
			Type = ConstType;
			Asteroid.setPosition(sf::Vector2f(Pos));
			switch(Type){
				case AsteroidType::Small:
					SIZE_MULTIP =3.75; 
					break;
				case AsteroidType::Medium:
					SIZE_MULTIP= 6.25;
					break;
				case AsteroidType::Large:
					SIZE_MULTIP=8.75;
					break;
				}
			switch(rand()%3+1){
				case 1:	
					Asteroid.setPointCount(16);
					Asteroid.setPoint(0, sf::Vector2f(-2*SIZE_MULTIP, -5*SIZE_MULTIP));
					Asteroid.setPoint(1, sf::Vector2f(0*SIZE_MULTIP, -5*SIZE_MULTIP));
					Asteroid.setPoint(2, sf::Vector2f(0*SIZE_MULTIP, -5*SIZE_MULTIP));
					Asteroid.setPoint(3, sf::Vector2f(2*SIZE_MULTIP, -4*SIZE_MULTIP));	
					Asteroid.setPoint(4, sf::Vector2f(4*SIZE_MULTIP, -2*SIZE_MULTIP));
					Asteroid.setPoint(5, sf::Vector2f(4*SIZE_MULTIP, 2*SIZE_MULTIP));
					Asteroid.setPoint(6, sf::Vector2f(2*SIZE_MULTIP, 4*SIZE_MULTIP));
					Asteroid.setPoint(7, sf::Vector2f(1*SIZE_MULTIP, 4*SIZE_MULTIP));
					Asteroid.setPoint(8, sf::Vector2f(0*SIZE_MULTIP, 3*SIZE_MULTIP));
					Asteroid.setPoint(9, sf::Vector2f(-3*SIZE_MULTIP, 3*SIZE_MULTIP));
					Asteroid.setPoint(10, sf::Vector2f(-2*SIZE_MULTIP, 2*SIZE_MULTIP));
					Asteroid.setPoint(11, sf::Vector2f(-2*SIZE_MULTIP, 1*SIZE_MULTIP));
					Asteroid.setPoint(12, sf::Vector2f(-4*SIZE_MULTIP, 0*SIZE_MULTIP));
					Asteroid.setPoint(13, sf::Vector2f(-4*SIZE_MULTIP, -4*SIZE_MULTIP));
					Asteroid.setPoint(14, sf::Vector2f(-3*SIZE_MULTIP, -4*SIZE_MULTIP));
					Asteroid.setPoint(15, sf::Vector2f(-2*SIZE_MULTIP, -5*SIZE_MULTIP));
					Asteroid.setOutlineThickness(OUTLINE_THICKNESS);
					Asteroid.setFillColor(sf::Color(0, 0, 0, 0));
					break;
				
				case 2:
					Asteroid.setPointCount(17);
					Asteroid.setPoint(0, sf::Vector2f(-3*SIZE_MULTIP, -4*SIZE_MULTIP));
					Asteroid.setPoint(1, sf::Vector2f(2*SIZE_MULTIP, -4*SIZE_MULTIP));
					Asteroid.setPoint(2, sf::Vector2f(2*SIZE_MULTIP, -3*SIZE_MULTIP));
					Asteroid.setPoint(3, sf::Vector2f(4*SIZE_MULTIP, -1*SIZE_MULTIP));
					Asteroid.setPoint(4, sf::Vector2f(4*SIZE_MULTIP, 2*SIZE_MULTIP));
					Asteroid.setPoint(5, sf::Vector2f(3*SIZE_MULTIP, 3*SIZE_MULTIP));
					Asteroid.setPoint(6, sf::Vector2f(2*SIZE_MULTIP, 3*SIZE_MULTIP));
					Asteroid.setPoint(7, sf::Vector2f(1*SIZE_MULTIP, 4*SIZE_MULTIP));
					Asteroid.setPoint(8, sf::Vector2f(-2*SIZE_MULTIP, 4*SIZE_MULTIP));
					Asteroid.setPoint(9, sf::Vector2f(-2*SIZE_MULTIP, 3*SIZE_MULTIP));
					Asteroid.setPoint(10, sf::Vector2f(-3*SIZE_MULTIP, 3*SIZE_MULTIP));
					Asteroid.setPoint(11, sf::Vector2f(-4*SIZE_MULTIP, 2*SIZE_MULTIP));
					Asteroid.setPoint(12, sf::Vector2f(-3*SIZE_MULTIP, 1*SIZE_MULTIP));
					Asteroid.setPoint(13, sf::Vector2f(-3*SIZE_MULTIP, -1*SIZE_MULTIP));
					Asteroid.setPoint(14, sf::Vector2f(-4*SIZE_MULTIP, -2*SIZE_MULTIP));
					Asteroid.setPoint(15, sf::Vector2f(-4*SIZE_MULTIP, -3*SIZE_MULTIP));
					Asteroid.setPoint(16, sf::Vector2f(-3*SIZE_MULTIP, -4*SIZE_MULTIP));
					Asteroid.setOutlineThickness(OUTLINE_THICKNESS);
					Asteroid.setFillColor(sf::Color(0, 0, 0, 0));
					break;

				case 3:
					Asteroid.setPointCount(17);
					Asteroid.setPoint(0, sf::Vector2f(-1*SIZE_MULTIP, -4*SIZE_MULTIP));
					Asteroid.setPoint(1, sf::Vector2f(3*SIZE_MULTIP, -4*SIZE_MULTIP));
					Asteroid.setPoint(2, sf::Vector2f(3*SIZE_MULTIP, -3*SIZE_MULTIP));
					Asteroid.setPoint(3, sf::Vector2f(4*SIZE_MULTIP, -2*SIZE_MULTIP));
					Asteroid.setPoint(4, sf::Vector2f(4*SIZE_MULTIP, -1*SIZE_MULTIP));
					Asteroid.setPoint(5, sf::Vector2f(3*SIZE_MULTIP, -1*SIZE_MULTIP));
					Asteroid.setPoint(6, sf::Vector2f(3*SIZE_MULTIP, 2*SIZE_MULTIP));
					Asteroid.setPoint(7, sf::Vector2f(1*SIZE_MULTIP, 4*SIZE_MULTIP));
					Asteroid.setPoint(8, sf::Vector2f(-3*SIZE_MULTIP, 4*SIZE_MULTIP));
					Asteroid.setPoint(9, sf::Vector2f(-4*SIZE_MULTIP, 3*SIZE_MULTIP));
					Asteroid.setPoint(10, sf::Vector2f(-4*SIZE_MULTIP, 0*SIZE_MULTIP));
					Asteroid.setPoint(11, sf::Vector2f(-5*SIZE_MULTIP, -1*SIZE_MULTIP));
					Asteroid.setPoint(12, sf::Vector2f(-5*SIZE_MULTIP, -2*SIZE_MULTIP));
					Asteroid.setPoint(13, sf::Vector2f(-4*SIZE_MULTIP, -2*SIZE_MULTIP));
					Asteroid.setPoint(14, sf::Vector2f(-3*SIZE_MULTIP, -3*SIZE_MULTIP));
					Asteroid.setPoint(15, sf::Vector2f(-2*SIZE_MULTIP, -3*SIZE_MULTIP));
					Asteroid.setPoint(16, sf::Vector2f(-1*SIZE_MULTIP, -4*SIZE_MULTIP));
					Asteroid.setOutlineThickness(OUTLINE_THICKNESS);
					Asteroid.setFillColor(sf::Color(0, 0, 0, 0));
					break;

			}
		} 
		std::pair<Status, int> Update(double FrameTime){
			Asteroid.rotate(ROTATE_SPD);
			const int Degrees_Rotation = DIRECTION*180/3.14159265;
			Asteroid.move(VELOCITY*FrameTime*cos(Degrees_Rotation), VELOCITY*FrameTime*sin(Degrees_Rotation));
			if (Asteroid.getPosition().y > WINDOW_HEIGHT){
				Asteroid.setPosition(sf::Vector2f(Asteroid.getPosition().x, 0));
			}
			if (Asteroid.getPosition().y < 0){
				Asteroid.setPosition(sf::Vector2f(Asteroid.getPosition().x, WINDOW_HEIGHT));
			}
			if (Asteroid.getPosition().x > WINDOW_LENGTH){
				Asteroid.setPosition(sf::Vector2f(0, Asteroid.getPosition().y));
			}
			if (Asteroid.getPosition().x <0){
				Asteroid.setPosition(sf::Vector2f(WINDOW_LENGTH, Asteroid.getPosition().y));
			}
			for (int i=0; i<GlobalRockets.size(); i++){
				if (GlobalRockets.at(i).Rocket.getGlobalBounds().intersects(Asteroid.getGlobalBounds())){
					return std::make_pair(Status::Remove, i); 
				}
			}
			
			return std::make_pair(Status::Keep, -1); 
		}
};

std::vector<AsteroidClass> GlobalAsteroids = {};
class ScoreClass {
	public:
		std::string NumString;
		double ScoreInt =0;
		sf::Text Text;
		sf::Font SansSerif;
		ScoreClass(){
			SansSerif.loadFromFile("SansSerif.otf");
			Text.setFont(SansSerif);
			Text.setCharacterSize(30);
		}
		void SetScore(double NumInt){
			NumString = std::to_string(NumInt);
			Text.setString(NumString);
			ScoreInt = NumInt;
		}
};

class SpaceShip {
	private:
		const Turn TURN_TYPE = Turn::Static;
		const int OUTLINE_THICKNESS = 1;
		const int SIZE_MULTIP=4;
		const double ACCELERATION_SPD=0.02;
		const double DECELERATION_SPD=0.018;
		const double MAX_SPD = 4; 
		double TURN_SPD=2.5;
		const double GLIDE_TURN_SPD = 0.07;
		const double MAX_TURN_SPD = 7.5;
		double Velocity =0;
		double TurnVelocity =0;
		double Degrees_Rad = 0;
		const double FRAME_TIME_MULTIP=75;
		bool SpaceKeyPressed = false;
	public:
		sf::ConvexShape Ship{(5)};
		SpaceShip(){
			Ship.setPoint(0, sf::Vector2f(4*SIZE_MULTIP, 0));
			Ship.setPoint(1, sf::Vector2f(-4*SIZE_MULTIP, -3*SIZE_MULTIP));
			Ship.setPoint(2, sf::Vector2f(-2*SIZE_MULTIP, 0));
			Ship.setPoint(3, sf::Vector2f(-4*SIZE_MULTIP, 3*SIZE_MULTIP));
			Ship.setPoint(4, sf::Vector2f(4*SIZE_MULTIP, 0));
			Ship.setOutlineThickness(OUTLINE_THICKNESS);
			Ship.setPosition(sf::Vector2f(100, 100));
			Ship.setFillColor(sf::Color(0, 0, 0, 0));
			if (TURN_TYPE == Turn::Glide){
				TURN_SPD = GLIDE_TURN_SPD;
			}
		}
		WinState Update(double FrameTime, sf::RenderWindow &Window){
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::K)){
				Velocity+=ACCELERATION_SPD;	
				if (Velocity > MAX_SPD){
					Velocity = MAX_SPD;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::J)){
				Velocity-=DECELERATION_SPD;
				if (Velocity < MAX_SPD*-1){
					Velocity=MAX_SPD*-1;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::L)){ 
			TurnVelocity+=TURN_SPD;	
				if (TurnVelocity >= MAX_TURN_SPD){
					TurnVelocity = MAX_TURN_SPD;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::H)){
				TurnVelocity -= TURN_SPD;
				if (TurnVelocity <= MAX_TURN_SPD*-1){
					TurnVelocity = MAX_TURN_SPD*-1;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !SpaceKeyPressed){
				RocketClass Projectile(Ship.getPosition(), Ship.getRotation());
				GlobalRockets.push_back(Projectile);
				SpaceKeyPressed = true;
			}
			else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && SpaceKeyPressed){
				SpaceKeyPressed = false;
			}
			switch (TURN_TYPE){
				case Turn::Static:
					Ship.rotate(TurnVelocity*(FrameTime*FRAME_TIME_MULTIP));
					TurnVelocity = 0;
					break;
				case Turn::Glide:
					Ship.rotate(TurnVelocity*(FrameTime*FRAME_TIME_MULTIP));
					break;
			}
			Degrees_Rad = Ship.getRotation() *3.14159/180;
			Ship.move(sf::Vector2f(Velocity*FrameTime*FRAME_TIME_MULTIP*cos(Degrees_Rad), Velocity*FrameTime*FRAME_TIME_MULTIP*sin(Degrees_Rad)));

			if (Ship.getPosition().y > WINDOW_HEIGHT){
				Ship.setPosition(sf::Vector2f(Ship.getPosition().x, 0));
			}
			if (Ship.getPosition().y < 0){
				Ship.setPosition(sf::Vector2f(Ship.getPosition().x, WINDOW_HEIGHT));
			}
			if (Ship.getPosition().x > WINDOW_LENGTH){
				Ship.setPosition(sf::Vector2f(0, Ship.getPosition().y));
			}
			if (Ship.getPosition().x <0){
				Ship.setPosition(sf::Vector2f(WINDOW_LENGTH, Ship.getPosition().y));
			}
			std::vector<Status> RocketStatus;
			for (int i=0;i<GlobalRockets.size();i++){
				RocketStatus.push_back(GlobalRockets.at(i).Update(FrameTime));
				if (RocketStatus.at(i) == Status::Keep){
					Window.draw(GlobalRockets.at(i).Rocket);
				}
			}
			for (int i=0; i< RocketStatus.size();i++){
				if (RocketStatus.at(i) == Status::Remove){
					if (i < GlobalRockets.size()){
						GlobalRockets.erase(GlobalRockets.begin()+i);
					}
				}
			}
			for (int i=0; i<GlobalAsteroids.size();i++){
				if (Ship.getGlobalBounds().intersects(GlobalAsteroids.at(i).Asteroid.getGlobalBounds())){
					return WinState::Loss;
				}
			}
			return WinState::Playing;
		}
		void Restart(){
			Velocity = 0;
			TurnVelocity= 0;
			Ship.setRotation(0);
			Ship.setPosition(WINDOW_LENGTH/2, WINDOW_HEIGHT/2);
		}
};

void SpawnAsteroid(AsteroidType Type, sf::Vector2f Pos);
void UpdateAsteroids(double FrameTime, sf::RenderWindow &Window, ScoreClass &Score);

int main(){
	sf::Font SansSerif;
	SansSerif.loadFromFile("SansSerif.otf");
	sf::Text GameStateText;
	GameStateText.setFont(SansSerif);
	GameStateText.setCharacterSize(100);
	sf::Text GameStateSub;
	GameStateSub.setFont(SansSerif);
	GameStateSub.setCharacterSize(50);
	ScoreClass Score;
	Score.SetScore(0);
	srand(time(NULL));
	const double MAX_FPS = 60;
	SpaceShip Player;
	Player.Ship.setPosition(WINDOW_LENGTH/2, WINDOW_HEIGHT/2);
	double ASTEROID_SPAWN_TIME = 5;
	double AsteroidSpawnCooldown=0;
	const double MIN_ASTEROID_SPAWN_TIME = 0.5;
	const int ASTEROID_START_SPAWN_AMOUNT=8;
	sf::RenderWindow Window(sf::VideoMode(WINDOW_LENGTH, WINDOW_HEIGHT), "Asteroids");
	sf::Clock FrameTime;
	for (int i=0; i<ASTEROID_START_SPAWN_AMOUNT; i++){
		switch(rand()%5){
			case 0:
				SpawnAsteroid(AsteroidType::Large, sf::Vector2f(rand()%WINDOW_LENGTH, 0));
				break;
			case 1:
				SpawnAsteroid(AsteroidType::Large, sf::Vector2f(rand()%WINDOW_LENGTH, WINDOW_HEIGHT));
				break;
			case 2:
				SpawnAsteroid(AsteroidType::Large, sf::Vector2f(WINDOW_LENGTH, rand()%WINDOW_HEIGHT));
				break;
			case 3:
				SpawnAsteroid(AsteroidType::Large, sf::Vector2f(0, rand()%WINDOW_HEIGHT));
				break;
		}
	}
	while (Window.isOpen()){
		sf::Event Event;
		while (Window.pollEvent(Event)){
			if (Event.type == sf::Event::Closed){
				Window.close();
			}
		//	if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::F2){
		//		SpawnAsteroid(AsteroidType::Large, sf::Vector2f(150, 150));
		//	}
			if (GameState == WinState::Loss){
				GameStateText.setString("You lost!");
				GameStateText.setPosition(sf::Vector2f(WINDOW_LENGTH/2-GameStateText.getLocalBounds().width/2, WINDOW_HEIGHT/2-GameStateText.getLocalBounds().height));
				GameStateSub.setString("Press enter to restart.");
				GameStateSub.setPosition(sf::Vector2f(WINDOW_LENGTH/2-GameStateSub.getLocalBounds().width/2, WINDOW_HEIGHT/2-GameStateSub.getLocalBounds().height));
				GameStateSub.move(sf::Vector2f(0, GameStateText.getLocalBounds().height/2+WINDOW_HEIGHT/50));
				GameStateText.move(sf::Vector2f(0, GameStateSub.getLocalBounds().height/2*-1));
				if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Enter){
					GlobalRockets.clear();
					GlobalAsteroids.clear();
					Score.SetScore(0);
					GameState = WinState::Playing;
					Player.Restart();
				}
			}
		}

		if (FrameTime.getElapsedTime().asSeconds() >= 1/MAX_FPS){
			Window.clear();
			if (GameState != WinState::Loss){
			AsteroidSpawnCooldown+=FrameTime.getElapsedTime().asSeconds();
				if (AsteroidSpawnCooldown >= ASTEROID_SPAWN_TIME){
					switch(rand()%5){
						case 0:
							SpawnAsteroid(AsteroidType::Large, sf::Vector2f(rand()%WINDOW_LENGTH, 0));
							break;
						case 1:
							SpawnAsteroid(AsteroidType::Large, sf::Vector2f(rand()%WINDOW_LENGTH, WINDOW_HEIGHT));
							break;
						case 2:
							SpawnAsteroid(AsteroidType::Large, sf::Vector2f(WINDOW_LENGTH, rand()%WINDOW_HEIGHT));
							break;
						case 3:
							SpawnAsteroid(AsteroidType::Large, sf::Vector2f(0, rand()%WINDOW_HEIGHT));
							break;
					}
					AsteroidSpawnCooldown = 0;
					ASTEROID_SPAWN_TIME -= Score.ScoreInt*1000;
					if (ASTEROID_SPAWN_TIME <= MIN_ASTEROID_SPAWN_TIME){
						ASTEROID_SPAWN_TIME = MIN_ASTEROID_SPAWN_TIME;
					}
				}
			}
			if (GameState == WinState::Loss){
				Window.draw(GameStateText);
				Window.draw(GameStateSub);
			}
			if (GameState != WinState::Loss){
				GameState = Player.Update(FrameTime.getElapsedTime().asSeconds(), Window);
				UpdateAsteroids(FrameTime.getElapsedTime().asSeconds(), Window, Score);
			}
			for (int i=0; i<GlobalAsteroids.size();i++){
				Window.draw(GlobalAsteroids.at(i).Asteroid);
			}
			Window.draw(Player.Ship);
			Window.draw(Score.Text);
			Window.display();
			FrameTime.restart();
		}
	}
}

void UpdateAsteroids(double FrameTime, sf::RenderWindow &Window, ScoreClass &Score){
	std::vector<AsteroidClass> AsteroidSpawnStack = {};
	for (int i=0; i<GlobalAsteroids.size(); i++){
		std::pair AsteroidStatuses = GlobalAsteroids.at(i).Update(FrameTime);
		sf::Vector2f AsteroidPos = GlobalAsteroids.at(i).Asteroid.getPosition();
		switch(AsteroidStatuses.first){
		//	case Status::Keep:
			//	Window.draw(GlobalAsteroids.at(i).Asteroid);
		//		break;
			case Status::Remove:
				if (GlobalAsteroids.at(i).Type == AsteroidType::Large){
					AsteroidClass AsteroidSection(AsteroidType::Medium, AsteroidPos);
					AsteroidClass AsteroidSection1(AsteroidType::Medium, AsteroidPos);
					AsteroidSection.Asteroid.setPosition(GlobalAsteroids.at(i).Asteroid.getPosition());
					AsteroidSection1.Asteroid.setPosition(GlobalAsteroids.at(i).Asteroid.getPosition());
					AsteroidSpawnStack.push_back(AsteroidSection);
					AsteroidSpawnStack.push_back(AsteroidSection1);
					Score.SetScore(Score.ScoreInt + 0.000015);
				}
				else if (GlobalAsteroids.at(i).Type == AsteroidType::Medium){
					AsteroidClass AsteroidSection(AsteroidType::Small, AsteroidPos);
					AsteroidClass AsteroidSection1(AsteroidType::Small, AsteroidPos);
					AsteroidSection.Asteroid.setPosition(GlobalAsteroids.at(i).Asteroid.getPosition());
					AsteroidSection1.Asteroid.setPosition(GlobalAsteroids.at(i).Asteroid.getPosition());
					AsteroidSpawnStack.push_back(AsteroidSection);
					AsteroidSpawnStack.push_back(AsteroidSection1);
					Score.SetScore(Score.ScoreInt + 0.000010);
				}
				else {
					Score.SetScore(Score.ScoreInt + 0.000005);
				}
				GlobalAsteroids.erase(GlobalAsteroids.begin()+i);
				GlobalRockets.erase(GlobalRockets.begin()+AsteroidStatuses.second);
				i--;
				break;
		}
	}
	for (int i=0; i<AsteroidSpawnStack.size(); i++){
		GlobalAsteroids.push_back(AsteroidSpawnStack.at(i));
	}
}

void SpawnAsteroid(AsteroidType Type, sf::Vector2f Pos){
	AsteroidClass AsteroidInstance(Type, sf::Vector2f(Pos));
	GlobalAsteroids.push_back(AsteroidInstance);
}
