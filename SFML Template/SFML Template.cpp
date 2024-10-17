#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <string>
enum class Sides {
	Left,
	Right,
	None
};

#define CLOUD_NUM  3
#define BRANCH_NUM 6
int main()
{
	srand((unsigned int)time(NULL));

	sf::VideoMode vm(1920, 1080);
	sf::RenderWindow window(vm, "!!!Timber!!!", sf::Style::Default);

	//이미지 리소스를 담는 Texture 객체
	sf::Texture texBackground;
	sf::Texture texTree;
	sf::Texture texbee;
	sf::Texture texcloud;
	sf::Texture texAxe;
	sf::Texture texBranch;
	sf::Texture texPlayer;
	sf::Texture texRip;

	texBackground.loadFromFile("graphics/background.png");
	texTree.loadFromFile("graphics/tree2.png");
	texbee.loadFromFile("graphics/bee.png");
	texcloud.loadFromFile("graphics/cloud.png");
	texAxe.loadFromFile("graphics/axe.png");
	texBranch.loadFromFile("graphics/branch.png");
	texPlayer.loadFromFile("graphics/player.png");
	texRip.loadFromFile("graphics/rip.png");

	//이미지 리소스를 가지고 실제로 그리는 Sprite객체
	sf::Sprite spriteBackground;
	spriteBackground.setTexture(texBackground);

	sf::Sprite spriteTree;
	spriteTree.setTexture(texTree);
	spriteTree.setOrigin(texTree.getSize().x / 2, 0);
	spriteTree.setPosition(vm.width / 2, 0);

	sf::Sprite spriteBee;
	spriteBee.setTexture(texbee);
	spriteBee.setPosition(0, 700);

	//x 스케일을 뒤집어서 반대로 플립되게 만듦
	spriteBee.setScale(-1.f, 1.f);

	sf::Sprite spriteCloud[CLOUD_NUM];
	for (int i = 0; i < CLOUD_NUM; i++)
		spriteCloud[i].setTexture(texcloud);

	int flag[CLOUD_NUM];
	for (int i = 0; i < CLOUD_NUM; i++)
		flag[i] = rand() % 2;

	for (int i = 0; i < CLOUD_NUM; i++)
	{
		float cloudSize = rand() % 2 + 1;
		if (flag[i]) {
			spriteCloud[i].setPosition(sf::Vector2f((float)texcloud.getSize().x + vm.width, rand() % 300));
			spriteCloud[i].setScale(cloudSize, cloudSize);
		}
		else
		{
			spriteCloud[i].setPosition(sf::Vector2f(-(float)texcloud.getSize().x, rand() % 300));
			spriteCloud[i].setScale(cloudSize * -1, cloudSize);
		}
	}
	sf::Sprite spriteAxe;
	spriteAxe.setTexture(texAxe);
	spriteAxe.setPosition(100, 100);

	sf::Sprite spriteBranch[BRANCH_NUM];
	for (int i = 0; i < BRANCH_NUM; i++)
	{
		spriteBranch[i].setTexture(texBranch);
		spriteBranch[i].setOrigin(-(texTree.getSize().x / 2.f), (texBranch.getSize().y / 2.f));
	}
	Sides sideBarnches[BRANCH_NUM];
	sf::Vector2f posBranch(vm.width / 2, vm.height / 2 + 150);
	for (int i = 0; i < BRANCH_NUM; i++)
	{
		sideBarnches[i] = (Sides)(rand() % 3);

		while(sideBarnches[0] == Sides::Right)
			sideBarnches[0] = (Sides)(rand() % 3);

		spriteBranch[i].setPosition(posBranch);
		posBranch.y -= 150.f;
		switch (sideBarnches[i])
		{
		case Sides::Left:
			spriteBranch[i].setScale(-1.f, 1);
			break;
		case Sides::Right:
			break;
		}
	}

	Sides sidePlayer = Sides::Right;
	sf::Sprite spritePlayer;
	spritePlayer.setTexture(texPlayer);
	spritePlayer.setOrigin(-(texTree.getSize().x / 2.f + 30), texPlayer.getSize().y);
	spritePlayer.setPosition(vm.width / 2.f, texTree.getSize().y);

	//폰트설정, 폰트로 글 쓸 Text 형 객체 생성
	sf::Font scoreFont;
	scoreFont.loadFromFile("fonts/KOMIKAP_.ttf");

	sf::Text scoreText;
	int score = 0;
	scoreText.setFont(scoreFont);
	scoreText.setString("Score = " + std::to_string(score));
	scoreText.setCharacterSize(100);
	scoreText.setPosition(10.f, 10.f);
	scoreText.setFillColor(sf::Color::Magenta);

	sf::Text pauseText;
	pauseText.setFont(scoreFont);
	pauseText.setString("PRESS ENTER TO START!");
	pauseText.setCharacterSize(100);
	pauseText.setOrigin(pauseText.getLocalBounds().width / 2, pauseText.getLocalBounds().height / 2);
	pauseText.setPosition(vm.width / 2, vm.height / 2);
	pauseText.setFillColor(sf::Color::Magenta);

	sf::Text gameOverText;
	gameOverText.setFont(scoreFont);
	gameOverText.setString("\t\tGame Over\nPRESS ENTER TO NEW GAME!");
	gameOverText.setCharacterSize(100);
	gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2, gameOverText.getLocalBounds().height / 2);
	gameOverText.setPosition(vm.width / 2, vm.height / 2);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setOutlineThickness(5.f);
	gameOverText.setOutlineColor(sf::Color::Black);

	float timeBarWidth = 500;
	float timeBarheight = 80;
	float timeBarDuration = 3.f;
	float timeBarSpeed = timeBarWidth / timeBarDuration;
	float timeAddTime = 0.5f;
	sf::RectangleShape timeBar(sf::Vector2f(timeBarWidth, timeBarheight));
	timeBar.setFillColor(sf::Color::Red);
	timeBar.setPosition(vm.width / 2 - timeBarWidth / 2, texTree.getSize().y + 50);

	//Clock 클래스 = 객체가 생성될때 0 에서 시작해 시간을 재는 클래스
	sf::Clock clock;
	bool pauseFlag = true;
	bool isLeftKey = false;
	bool isRightKey = false;
	bool isLeftKeyDown = false;
	bool isRightKeyDown = false;


	bool gameoverFlag = false;
	//float accountTime = 0.f;
	//float timeScale = 1.f;



	// v = d / t
	// v = speedBee, t = deltaTime
	sf::Vector2f speedBee(200.f, 0.f);
	sf::Vector2f speedCloud[CLOUD_NUM];
	for (int i = 0; i < CLOUD_NUM; i++)
		speedCloud[i] = sf::Vector2f(rand() % 150 + 500, 0.f);


	while (window.isOpen())
	{
		//dt = 델타 타임
		//Clock::restart() = 현재 지난 시간을 반환 하고
		//해당 객체의 시간을 0으로 초기화
		sf::Time dt = clock.restart();

		//asSeconds() = 시간을 float형으로 반환하는 함수
		//deltaTime - 직전 업데이트와 현재 업데이트 사이의 시간차

		float deltaTime = dt.asSeconds();

		isLeftKeyDown = false;
		isRightKeyDown = false;
		isLeftKey = false;
		isRightKey = false;
		//메세지(이벤트) 루프
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			switch (ev.type)
			{
			case sf::Event::EventType::Closed:
				window.close();
				break;

			case sf::Event::EventType::KeyReleased:
				switch (ev.key.code)
				{
				case sf::Keyboard::Left:
					isLeftKeyDown = false;
					break;
				case sf::Keyboard::Right:
					isLeftKeyDown = false;
					break;
				default:
					break;
				}
				break;

			case sf::Event::EventType::KeyPressed:
				switch (ev.key.code)
				{
				case sf::Keyboard::Escape:
					window.close();
					break;

				case sf::Keyboard::Return:
					if (!gameoverFlag)
					{
						pauseFlag = pauseFlag ? !pauseFlag : !pauseFlag;
						break;
					}
					gameoverFlag = false;
					spritePlayer.setTexture(texPlayer);
					timeBar.setSize(sf::Vector2f(timeBarWidth, timeBarheight));
					score = 0;
					break;

				case sf::Keyboard::Left:
					if (!isLeftKey)
					{
						isLeftKeyDown = true;
					}
					isLeftKey = true;
					break;

				case sf::Keyboard::Right:
					if (!isRightKey)
					{
						isRightKeyDown = true;
					}
					isRightKey = true;
					break;
				default:
					break;
				}

				break;

			default:
				break;
			}
		}


		//업데이트
		if (!pauseFlag && !gameoverFlag)
		{
			if (isLeftKeyDown || isRightKeyDown)
			{
				score += 10;
				sidePlayer = isLeftKeyDown ? Sides::Left : Sides::Right;
				spritePlayer.setScale(isLeftKeyDown ? -1.f : 1.f, 1.f);
				if (sideBarnches[0] == sidePlayer)
				{
					spritePlayer.setTexture(texRip);
					gameoverFlag = true;
				}
				for (int i = 1; i < BRANCH_NUM; i++)
					sideBarnches[i - 1] = sideBarnches[i];
				sideBarnches[BRANCH_NUM - 1] = (Sides)(rand() % 3);

				for (int i = 0; i < BRANCH_NUM; i++)
					spriteBranch[i].setScale(sideBarnches[i] == Sides::Right ? 1.f : -1.f, 1);


				sf::Vector2f timerBarSize = timeBar.getSize();
				timerBarSize.x += timeBarSpeed * timeAddTime;
				if (timerBarSize.x > timeBarWidth)
				{
					timerBarSize.x = timeBarWidth;
				}
				timeBar.setSize(timerBarSize);
			}
			//벌의 위치를 속도와 시간을 곱해 업데이트할 때 마다 바꾼다.
			sf::Vector2f beePos = spriteBee.getPosition();
			beePos += speedBee * deltaTime;
			beePos.y = 700 + (sin(beePos.x / 100) * 50);
			spriteBee.setPosition(beePos);
			sf::Vector2f cloudPos[CLOUD_NUM];
			for (int i = 0; i < CLOUD_NUM; i++)
				cloudPos[i] = spriteCloud[i].getPosition();

			for (int i = 0; i < CLOUD_NUM; i++)
			{
				if (cloudPos[i].x > (vm.width + (float)texcloud.getSize().x * 2) || cloudPos[i].x < (-(float)texcloud.getSize().x) * 2)
				{
					float cloudSize = rand() % 2 + 1;
					flag[i] = rand() % 2;
					if (flag[i]) {
						cloudPos[i] = sf::Vector2f(sf::Vector2f((float)texcloud.getSize().x + vm.width, rand() % 300));
						spriteCloud[i].setScale(cloudSize, cloudSize);
					}
					else
					{
						cloudPos[i] = cloudPos[i] = sf::Vector2f(sf::Vector2f(-(float)texcloud.getSize().x, rand() % 300));
						spriteCloud[i].setScale(cloudSize * -1, cloudSize);
					}

					speedCloud[i] = sf::Vector2f(rand() % 150 + (rand() % 500), 0.f);
				}

				if (flag[i])
				{
					cloudPos[i] -= speedCloud[i] * deltaTime;
					spriteCloud[i].setPosition(cloudPos[i]);
				}
				else
				{
					cloudPos[i] += speedCloud[i] * deltaTime;
					spriteCloud[i].setPosition(cloudPos[i]);
				}
			}
			//스코어를 최신화한다
			scoreText.setString("Score = " + std::to_string(score));

			//타이머가 모두 사라질 때 까지 줄인다.
			sf::Vector2f size = timeBar.getSize();
			size.x -= timeBarSpeed * deltaTime;
			if (size.x < 0.f)
			{
				size.x = 0.f;
			}
			timeBar.setSize(size);

			if (timeBar.getSize().x < 0.1)
			{
				spritePlayer.setTexture(texRip);
				gameoverFlag = true;
			}
		}
		window.clear();

		// 그리기
		// 그리기는 항상 뒤쪽 코드가 앞에 그려진 
		// 그림 위에 덮어써지기 때문에
		// 순서를 신경써서 코드를 작성해야함
		window.draw(spriteBackground);

		for (int i = 0; i < CLOUD_NUM; i++)
			window.draw(spriteCloud[i]);

		window.draw(spriteTree);
		for (int i = 0; i < BRANCH_NUM; i++)
			if (sideBarnches[i] != (Sides)2)
				window.draw(spriteBranch[i]);

		window.draw(spriteBee);
		window.draw(spritePlayer);
		window.draw(scoreText);
		if (pauseFlag)
			window.draw(pauseText);
		if (gameoverFlag)
			window.draw(gameOverText);
		window.draw(timeBar);
		window.display();
	}
	return 0;
}