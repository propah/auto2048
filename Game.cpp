#include "Game.h"

void Game::initWindow()
{
	ifstream ifs("r_config/window.ini");
	string title_name = "None";
	VideoMode window_res(1280, 720);
	unsigned int framerate_limit = 144;
	bool vsyncEnabled = false;
	if (ifs.is_open()) {
		getline(ifs, title_name);
		ifs >> window_res.width >> window_res.height;
		ifs >> framerate_limit;
		ifs >> vsyncEnabled;
	}
	ifs.close();
	this->window = new RenderWindow(window_res, title_name, Style::Titlebar | Style::Close);
	this->window->setFramerateLimit(framerate_limit);
	this->window->setVerticalSyncEnabled(vsyncEnabled);
	//this->window->setKeyRepeatEnabled(false);
}

void Game::initVariables(int size)
{
	this->gameSize = size;
	this->board = Board(size);
	this->window = nullptr;
}

void Game::initSounds()
{
	this->bg_music.openFromFile("r_sounds/bg_music.ogg");
	this->bg_music.setLoop(true);
	this->bg_music.setVolume(60);
}

void Game::initDrawables()
{
	bg_square = RectangleShape(Vector2f(0.85 * this->window->getSize().y, 0.85 * this->window->getSize().y));
	bg_square.setFillColor(*c_tileBgColor);
	float tileSizeSide = bg_square.getSize().x / gameSize * 0.83;
	tile_square = RectangleShape(Vector2f(tileSizeSide, tileSizeSide));
	tile_square.setFillColor(*c_tileColor);
	font_nums.loadFromFile("r_fonts/ClearSans-Bold.ttf");
	tx_nums = Text("2", font_nums);
	tx_nums.setCharacterSize(30);
	tx_nums.setLetterSpacing(1);
}

Game::Game(int size)
{
	this->initVariables(size);
	this->initSounds();
	this->initWindow();
	this->initDrawables();
}

Game::~Game()
{
}

const bool Game::running() const
{
	return this->window->isOpen();
}

void Game::render()
{
	//

	this->window->display();
	this->window->clear(*c_bgColor);
}

void Game::update()
{
	this->pollEvents();
	this->updateShapes();
	//this->updateMusic();
}

void Game::updateMusic()
{
	if (this->bg_music.getStatus() != Music::Status::Playing) {
		this->bg_music.play();
	}
}

void Game::updateShapes()
{
	this->bg_square.setPosition((this->window->getSize().x / 2.f) - (bg_square.getSize().x/2), (this->window->getSize().y / 2.f)- (bg_square.getSize().y / 2));
	this->window->draw(this->bg_square);
	for (int i =0; i < gameSize; i++){
		for (int y = 0; y < gameSize; y++) {
			switch (board.getState()[y][i]) {
			case 0:
				tile_square.setFillColor(*c_tileColor);
				tx_nums.setString("");
				break;
			case 2:
				tile_square.setFillColor(*c_oneTile);
				tx_nums.setString("2");
				break;
			case 4:
				tile_square.setFillColor(*c_twoTile);
				tx_nums.setString("4");
				break;
			case 8:
				tile_square.setFillColor(*c_threeTile);
				tx_nums.setString("8");
				break;
			case 16:
				tile_square.setFillColor(*c_fourTile);
				tx_nums.setString("16");
				break;
			case 32:
				tile_square.setFillColor(*c_fiveTile);
				tx_nums.setString("32");
				break;
			case 64:
				tile_square.setFillColor(*c_sixTile);
				tx_nums.setString("64");
				break;
			case 128:
				tile_square.setFillColor(*c_sevenTile);
				tx_nums.setString("128");
				break;
			case 256:
				tile_square.setFillColor(*c_eightTile);
				tx_nums.setString("256");
				break;
			case 512:
				tile_square.setFillColor(*c_moreTile);
				tx_nums.setString("512");
				break;
			case 1024:
				tile_square.setFillColor(*c_moreTile);
				tx_nums.setString("1024");
				break;
			case 2048:
				tile_square.setFillColor(*c_moreTile);
				tx_nums.setString("2048");
				break;
			default:
				tile_square.setFillColor(*c_moreTile);
				tx_nums.setString("++");
				break;
			}
			this->tx_nums.setOrigin(tx_nums.getLocalBounds().width / 2, tx_nums.getLocalBounds().height / 2);
			float xTileCor = bg_square.getPosition().x + tile_square.getSize().x * i + (bg_square.getSize().x / gameSize) * 0.14 * (i+1);
			float yTileCor = bg_square.getPosition().y + tile_square.getSize().y * y + (bg_square.getSize().y / gameSize) * 0.14 * (y+1);
			this->tile_square.setPosition( xTileCor, yTileCor);
			this->tx_nums.setPosition((xTileCor + tile_square.getSize().x/2), yTileCor + tile_square.getSize().y / 2);
			this->window->draw(this->tile_square);
			this->window->draw(this->tx_nums);
		}
	}

}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case Event::Closed:
			this->window->close();
			break;
		case Event::KeyPressed:
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				this->window->close();
			}
			if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
				this->resetBoard();
			}
			if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Z)) {
				this->board.moveUp();
				this->board.addPoints();
			}
			if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
				this->board.moveDown();
				this->board.addPoints();
			}
			if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Q)) {
				this->board.moveLeft();
				this->board.addPoints();
			}
			if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
				this->board.moveRight();
				this->board.addPoints();
			}
		}
	}
}

void Game::resetBoard()
{
	board = Board(gameSize);
}

void Game::run()
{
	while (this->running()) {
		this->update();
		this->render();
	}
}
