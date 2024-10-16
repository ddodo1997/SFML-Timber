#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>

int main()
{
	srand((unsigned int)time(NULL));

	sf::VideoMode vm(1920, 1080);
	sf::RenderWindow window(vm, "!!!Timber!!!", sf::Style::Default);

	//이미지 리소스를 담는 Texture 객체
	sf::Texture texBackground;
	texBackground.loadFromFile("graphics/background.png");
	sf::Texture texTree;
	texTree.loadFromFile("graphics/tree.png");
	sf::Texture texbee;
	texbee.loadFromFile("graphics/bee.png");
	sf::Texture texcloud;
	texcloud.loadFromFile("graphics/cloud.png");

	//이미지 리소스를 가지고 실제로 그리는 Sprite객체
	sf::Sprite spriteBackground;
	spriteBackground.setTexture(texBackground);

	sf::Sprite spriteTree;
	spriteTree.setTexture(texTree);
	spriteTree.setOrigin(texTree.getSize().x / 2, 0);
	spriteTree.setPosition(1920 / 2, 0);

	sf::Sprite spriteBee;
	spriteBee.setTexture(texbee);
	spriteBee.setPosition(0, 700);

	//x 스케일을 뒤집어서 반대로 플립되게 만듦
	spriteBee.setScale(-1.f, 1.f);

	sf::Sprite spriteCloud[3];
	for (int i = 0; i < 3; i++)
		spriteCloud[i].setTexture(texcloud);

	int flag[3];
	for (int i = 0; i < 3; i++)
		flag[i] = rand() % 2;

	for (int i = 0; i < 3; i++)
	{
		float cloudSize = rand() % 2 + 1;
		if (flag[i]) {
			spriteCloud[i].setPosition(sf::Vector2f((float)texcloud.getSize().x + vm.width, (float)texcloud.getSize().y * i));

			spriteCloud[i].setScale(cloudSize, cloudSize);
		}
		else
		{
			spriteCloud[i].setPosition(sf::Vector2f(-(float)texcloud.getSize().x, (float)texcloud.getSize().y * i));

			spriteCloud[i].setScale(cloudSize * -1, cloudSize);
		}
	}

	//Clock 클래스 = 객체가 생성될때 0 에서 시작해 시간을 재는 클래스
	sf::Clock clock;

	// v = d / t
	// v = speedBee, t = deltaTime
	sf::Vector2f speedBee(100.f, 0.f);
	sf::Vector2f speedCloud[3];
	for (int i = 0; i < 3; i++)
		speedCloud[i] = sf::Vector2f(rand() % 150 + 500, 0.f);

	while (window.isOpen())
	{
		//dt = 델타 타임
		//Clock::restart() = 현재 지난 시간을 반환 하고
		//해당 객체의 시간을 0으로 초기화
		sf::Time dt = clock.restart();

		//asSeconds() = 시간을 float형으로 반환하는 함수
		float deltaTime = dt.asSeconds();

		//메세지(이벤트) 루프
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			switch (ev.type)
			{
			case sf::Event::EventType::Closed:
				window.close();
				break;

				//키보드가 눌렸을 경우, 눌린 키가 ESC일 때, 윈도우를 닫음
			case sf::Event::EventType::KeyPressed:
				switch (ev.key.code)
				{
				case sf::Keyboard::Escape:
					window.close();
					break;
				case sf::Keyboard::Space:

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

		//벌의 위치를 속도와 시간을 곱해 업데이트할 때 마다 바꾼다.
		sf::Vector2f beePos = spriteBee.getPosition();
		beePos += speedBee * deltaTime;
		beePos.y = 700 + (sin(beePos.x / 10) * 10);
		spriteBee.setPosition(beePos);

		sf::Vector2f cloudPos[3];
		for (int i = 0; i < 3; i++)
			cloudPos[i] = spriteCloud[i].getPosition();

		for (int i = 0; i < 3; i++)
		{
			if (cloudPos[i].x > (vm.width + (float)texcloud.getSize().x * 2) || cloudPos[i].x < (-(float)texcloud.getSize().x) * 2)
			{
				float cloudSize = rand() % 2 + 1;
				flag[i] = rand() % 2;
				if (flag[i]) {
					cloudPos[i] = sf::Vector2f(sf::Vector2f((float)texcloud.getSize().x + vm.width, (float)texcloud.getSize().y * i));
					spriteCloud[i].setScale(cloudSize, cloudSize);
				}
				else
				{
					cloudPos[i] = cloudPos[i] = sf::Vector2f(sf::Vector2f(-(float)texcloud.getSize().x, (float)texcloud.getSize().y * i));
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

		window.clear();

		// 그리기
		// 그리기는 항상 뒤쪽 코드가 앞에 그려진 
		// 그림 위에 덮어써지기 때문에
		// 순서를 신경써서 코드를 작성해야함
		window.draw(spriteBackground);
		for (int i = 0; i < 3; i++)
			window.draw(spriteCloud[i]);
		window.draw(spriteTree);
		window.draw(spriteBee);

		window.display();
	}
	return 0;
}