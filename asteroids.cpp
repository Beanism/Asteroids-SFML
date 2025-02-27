#include <iostream>
#include <SFML/Graphics.hpp> // Graphical library of choice.
#include <cmath> // Sine and cosine. For converting direction into a gradient.
#include <vector> // Stores all objects, such as the asteroids, and rockets.
#include <utility>
#include <string>

// Compilation: g++ ./asteroids.cpp -lsfml-window -lsfml-system -lsfml-graphics -fcompare-debug-second
	// Compilation code. Use this to prevent linker errors and 16 lines of notes. 

enum WinState { // Current state of the game.
	Loss,
	Playing	
};

WinState GameState = WinState::Playing; // Used in logic to determine whether or not the game is to end.

enum Turn {
	Static,
	Glide // Turn type. 'Glide' makes the ship turn without user input, whilst static will stop immediatly if lacking input.
};

enum AsteroidType {
	Small,
	Medium, // Type of asteroid. Determines size, NOT shape.
	Large
};

enum Status {
	Remove,
	Keep // Whether or not an entity is to be kept or deleted.
};

const int WINDOW_LENGTH = 800;
const int WINDOW_HEIGHT = 600; # Self explanitory.


void ScreenWrap(sf::Transformable &Transformable) {
	if (Transformable.getPosition().y > WINDOW_HEIGHT){
		Transformable.setPosition(sf::Vector2f(Transformable.getPosition().x, 0));
	} else if (Transformable.getPosition().y < 0){
		Transformable.setPosition(sf::Vector2f(Transformable.getPosition().x, WINDOW_HEIGHT));
	}
	if (Transformable.getPosition().x > WINDOW_LENGTH){
		Transformable.setPosition(sf::Vector2f(0, Transformable.getPosition().y));
	} else if (Transformable.getPosition().x <0){
		Transformable.setPosition(sf::Vector2f(WINDOW_LENGTH, Transformable.getPosition().y));
	}
}


class RocketClass { // Class all the player-spawned projectiles follow
	private:
		double MOVE_SPD=500; // Speed of the rocket.
		double ROTATION_RAD; // The direction of the rocket, in radians. For usage with sine and cosine.
		double OUTLINE_THICKNESS=1.5; # Visual outline thickness of the rocket.
		float SIZE=3; // The size of the rocket. Does NOT change the shape.
	public:
		sf::CircleShape Rocket; // Actual rocket entity.
		RocketClass(sf::Vector2f StartPos, double Rotation){ // Setting values based on initialisation vals.
			Rocket.setRadius(SIZE);
			Rocket.setFillColor(sf::Color(0, 0, 0, 0));
			Rocket.setPosition(StartPos);
			Rocket.setRotation(Rotation);
			Rocket.setOutlineThickness(OUTLINE_THICKNESS);
			ROTATION_RAD = Rotation*3.14159265/180;
			Rocket.setOrigin(sf::Vector2f(SIZE, SIZE)); // Default origin is 0, 0. Set the origin to the centre of the rocket.
		}
		Status Update(double TimeFrame){ // Updates the rocket. Does NOT check for asteroid collisions.
			Rocket.move(MOVE_SPD*TimeFrame*cos(ROTATION_RAD), MOVE_SPD*TimeFrame*sin(ROTATION_RAD)); // Moving the rocket.
			ScreenWrap(Rocket);
			return Status::Keep; // If no collisions with windows, the rocket persists.
		}
};

std::vector<RocketClass> GlobalRockets = {}; // Global vector containing all the rockets currently existing.

class AsteroidClass { // Class of all the asteroids. Should probably be optimised better, i.e rocket instead checking for collisions. There will most likely be >20 at most times.
	private:
		double ROTATE_SPD =0.8; // Values, such as direction the asteroid is going in, (must be seperate val due to asteroids rotating whilst moving), speed, size, and visual outline thickness.
		double VELOCITY =70;
		double DIRECTION=0;
		double SIZE_MULTIP=0.25;
		double OUTLINE_THICKNESS = 1;
	public:
		AsteroidType Type = AsteroidType::Large; // Type of asteroid. They split into 2 smaller ones. Large > Medium x2 > Small x4
		sf::ConvexShape Asteroid;
		AsteroidClass(AsteroidType ConstType, sf::Vector2f Pos){
			DIRECTION = rand()%361;
			Type = ConstType;
			Asteroid.setPosition(sf::Vector2f(Pos));
			switch(Type){ // Randomly changing asteroid type.
				case AsteroidType::Small:
					SIZE_MULTIP =3.75;  // Enum is just so code is readable. SIZE_MULTIP is the actual definer of size.
					break;
				case AsteroidType::Medium:
					SIZE_MULTIP= 6.25;
					break;
				case AsteroidType::Large:
					SIZE_MULTIP=8.75;
					break;
				}
			switch(rand()%3+1){ // Randomly changing asteroid shape.
				case 1:	
					Asteroid.setPointCount(16); // Setting point count. I spent a few days wondering why I would get 16 different memory errors from these... Do not set it to a too low point count.
					Asteroid.setPoint(0, sf::Vector2f(-2*SIZE_MULTIP, -5*SIZE_MULTIP));
					Asteroid.setPoint(1, sf::Vector2f(0*SIZE_MULTIP, -5*SIZE_MULTIP));
					Asteroid.setPoint(2, sf::Vector2f(0*SIZE_MULTIP, -5*SIZE_MULTIP));
					Asteroid.setPoint(3, sf::Vector2f(2*SIZE_MULTIP, -4*SIZE_MULTIP));	
					Asteroid.setPoint(4, sf::Vector2f(4*SIZE_MULTIP, -2*SIZE_MULTIP));
					Asteroid.setPoint(5, sf::Vector2f(4*SIZE_MULTIP, 2*SIZE_MULTIP));
					Asteroid.setPoint(6, sf::Vector2f(2*SIZE_MULTIP, 4*SIZE_MULTIP));
					Asteroid.setPoint(7, sf::Vector2f(1*SIZE_MULTIP, 4*SIZE_MULTIP)); // Yeah... This bit isn't really that readable. I suggest drawing it on GIMP or something.
					Asteroid.setPoint(8, sf::Vector2f(0*SIZE_MULTIP, 3*SIZE_MULTIP));
					Asteroid.setPoint(9, sf::Vector2f(-3*SIZE_MULTIP, 3*SIZE_MULTIP));
					Asteroid.setPoint(10, sf::Vector2f(-2*SIZE_MULTIP, 2*SIZE_MULTIP));
					Asteroid.setPoint(11, sf::Vector2f(-2*SIZE_MULTIP, 1*SIZE_MULTIP));
					Asteroid.setPoint(12, sf::Vector2f(-4*SIZE_MULTIP, 0*SIZE_MULTIP));
					Asteroid.setPoint(13, sf::Vector2f(-4*SIZE_MULTIP, -4*SIZE_MULTIP));
					Asteroid.setPoint(14, sf::Vector2f(-3*SIZE_MULTIP, -4*SIZE_MULTIP));
					Asteroid.setPoint(15, sf::Vector2f(-2*SIZE_MULTIP, -5*SIZE_MULTIP));
					Asteroid.setOutlineThickness(OUTLINE_THICKNESS); // Set outline thickness.
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
		std::pair<Status, int> Update(double FrameTime){ // Update function. On collision returns an std::pair containing the index of the offending rocket, and the status for it. 
			Asteroid.rotate(ROTATE_SPD);
			const int Degrees_Rotation = DIRECTION*180/3.14159265; // Converting to radians.
			Asteroid.move(VELOCITY*FrameTime*cos(Degrees_Rotation), VELOCITY*FrameTime*sin(Degrees_Rotation)); // Translating degrees in radians to gradient via sine and cosine. Allows moving in x direction.
			ScreenWrap(Asteroid);
			for (int i=0; i<GlobalRockets.size(); i++){ // To be honest, I should probably optimise this. Each asteroid, (usually >20) is looping through a vector of usually large proportions.
				if (GlobalRockets.at(i).Rocket.getGlobalBounds().intersects(Asteroid.getGlobalBounds())){ // If asteroid and rocket intersect,
					return std::make_pair(Status::Remove, i); // Delete offending rocket. The function using this also breaks open the victimised asteroid. Probably should shift it up here.
				}
			}
			
			return std::make_pair(Status::Keep, -1); // Returns -1 for rocket index if no collisions.
		}
};

std::vector<AsteroidClass> GlobalAsteroids = {}; // Vector containing every existing asteroid.
class ScoreClass { // Class displaying player score. Keeps track of as well as visually displays the score.
	public:
		std::string NumString; // Text class only allows for std::string arguments. ScoreInt keeps track of the actual score, whilst it is converted to std::string.
		double ScoreInt =0; // Keeps numerical track of the score.
		sf::Text Text; // The actual text.
		sf::Font SansSerif; // The font. Changing it will change the font displayed.
		ScoreClass(){ // Initialiser.
			SansSerif.loadFromFile("SansSerif.otf"); // Loading font
			Text.setFont(SansSerif); // Setting font.
			Text.setCharacterSize(30); // Setting character size.
		}
		void SetScore(double NumInt){ // Ran every time score is updated.
			NumString = std::to_string(NumInt);
			Text.setString(NumString);
			ScoreInt = NumInt; // I honestly don't remember why I did this.
		}
};

class SpaceShip { // Player class.
	private: // Most of these are self-explanitory.
		const Turn TURN_TYPE = Turn::Static; 
		const int OUTLINE_THICKNESS = 1; 
		const int SIZE_MULTIP=4;
		const double ACCELERATION_SPD=0.02;
		const double DECELERATION_SPD=0.018;
		const double MAX_SPD = 4; 
		double TURN_SPD=2.5;
		const double GLIDE_TURN_SPD = 0.07; // For use with Turn::Glide
		const double MAX_TURN_SPD = 7.5;
		double Velocity =0; // Velocity of the player
		double TurnVelocity =0; // Turn velocity of the player. Used only with Turn::Glide.
		double Degrees_Rad = 0; // For usage with sine and cosine
		const double FRAME_TIME_MULTIP=75; // Set to the time since last frame. Makes 
		bool SpaceKeyPressed = false; // For shooting rocket.
	public:
		sf::ConvexShape Ship{(5)}; // Actual ship. '5' defines the amount of points. It's just a sort of futuristic cursor shape. Like the stellaris one.
		SpaceShip(){
			Ship.setPoint(0, sf::Vector2f(4*SIZE_MULTIP, 0));
			Ship.setPoint(1, sf::Vector2f(-4*SIZE_MULTIP, -3*SIZE_MULTIP));
			Ship.setPoint(2, sf::Vector2f(-2*SIZE_MULTIP, 0));
			Ship.setPoint(3, sf::Vector2f(-4*SIZE_MULTIP, 3*SIZE_MULTIP)); // Draw it in GIMP. Please don't visualise it, it doesn't work.
			Ship.setPoint(4, sf::Vector2f(4*SIZE_MULTIP, 0));
			Ship.setOutlineThickness(OUTLINE_THICKNESS);
			Ship.setPosition(sf::Vector2f(100, 100));
			Ship.setFillColor(sf::Color(0, 0, 0, 0));
			if (TURN_TYPE == Turn::Glide){
				TURN_SPD = GLIDE_TURN_SPD; // For glidey turning.
			}
		}
		WinState Update(double FrameTime, sf::RenderWindow &Window){ // Update function. Contains input usage, etc. For those Vim users, of course it contains a Vim control scheme.
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::K)){ // Forward
				Velocity+=ACCELERATION_SPD;	
				if (Velocity > MAX_SPD){
					Velocity = MAX_SPD;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::J)){ // Backward
				Velocity-=DECELERATION_SPD;
				if (Velocity < MAX_SPD*-1){
					Velocity=MAX_SPD*-1;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::L)){ // Right
			TurnVelocity+=TURN_SPD;	
				if (TurnVelocity >= MAX_TURN_SPD){
					TurnVelocity = MAX_TURN_SPD;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::H)){ // Left
				TurnVelocity -= TURN_SPD;
				if (TurnVelocity <= MAX_TURN_SPD*-1){
					TurnVelocity = MAX_TURN_SPD*-1;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !SpaceKeyPressed){ // Shooting projectile
				RocketClass Projectile(Ship.getPosition(), Ship.getRotation()); // Projectile instance. 
				GlobalRockets.push_back(Projectile); // Stuff into GlobalRockets vector.
				SpaceKeyPressed = true; // This is so you can't just hold space bar and destroy your CPU.
			}
			else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && SpaceKeyPressed){
				SpaceKeyPressed = false;
			}
			switch (TURN_TYPE){
				case Turn::Static:
					Ship.rotate(TurnVelocity*(FrameTime*FRAME_TIME_MULTIP)); // Turning
					TurnVelocity = 0;
					break;
				case Turn::Glide:
					Ship.rotate(TurnVelocity*(FrameTime*FRAME_TIME_MULTIP)); // Literally just Turn::Static but it doesn't reset turn velocity.
					break;
			}
			Degrees_Rad = Ship.getRotation() *3.14159/180; // Converted to radians.
			Ship.move(sf::Vector2f(Velocity*FrameTime*FRAME_TIME_MULTIP*cos(Degrees_Rad), Velocity*FrameTime*FRAME_TIME_MULTIP*sin(Degrees_Rad))); // Converted to gradiant through sine and cosine

			ScreenWrap(Ship);
			std::vector<Status> RocketStatus; // Added into this vector so that they can be removed later. Removing elements whilst looping introduces you to your amazing friend, Segmentation fault!
			for (int i=0;i<GlobalRockets.size();i++){
				RocketStatus.push_back(GlobalRockets.at(i).Update(FrameTime)); // Updating rockets. I like how this is in the player class. Very ergonomic.
				if (RocketStatus.at(i) == Status::Keep){
					Window.draw(GlobalRockets.at(i).Rocket); // Draw rockets.
				}
			}
			for (int i=0; i< RocketStatus.size();i++){
				if (RocketStatus.at(i) == Status::Remove){
					if (i < GlobalRockets.size()){
						GlobalRockets.erase(GlobalRockets.begin()+i); // If rocket is to be removed, remove rocket.
					}
				}
			}
			for (int i=0; i<GlobalAsteroids.size();i++){
				if (Ship.getGlobalBounds().intersects(GlobalAsteroids.at(i).Asteroid.getGlobalBounds())){ // Detect if player ship touches asteroid.
					return WinState::Loss; // End game.
				}
			}
			return WinState::Playing;
		}
		void Restart(){ // Function to be called one the game is restarted.
			Velocity = 0;
			TurnVelocity= 0;
			Ship.setRotation(0);
			Ship.setPosition(WINDOW_LENGTH/2, WINDOW_HEIGHT/2);
		}
};

//			Handler functions.
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
	double ASTEROID_SPAWN_TIME = 3;
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
