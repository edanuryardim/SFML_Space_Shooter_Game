#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <ctime>

int main()
{
    // Rastgele say� �retmek i�in tohum ayarla
    std::srand(std::time(nullptr));

    // Pencere boyutlar�
    sf::Vector2u windowSize(800, 600);

    // Pencereyi olu�tur
    sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Space Shooter Game");

    // Yaz� fontunu y�kle
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Hata kontrol�
        return 1;
    }

    // Roket resmini y�kle
    sf::Texture rocketTexture;
    if (!rocketTexture.loadFromFile("rocket.bmp")) {
        // Hata kontrol�
        return 1;
    }

    // Mermi resmini y�kle
    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("mermi.bmp")) {
        // Hata kontrol�
        return 1;
    }

    // Y�ld�z resimlerini y�kle
    sf::Texture starTexture1;
    if (!starTexture1.loadFromFile("star1.bmp")) {
        // Hata kontrol�
        return 1;
    }

    sf::Texture starTexture2;
    if (!starTexture2.loadFromFile("star2.bmp")) {
        // Hata kontrol�
        return 1;
    }

    // Genel kar��lama yaz�s�
    sf::Text welcomeText;
    welcomeText.setFont(font);
    welcomeText.setString("SPACE SHOOTER GAME\n\nPress 's' to start\n\n Developed by Eda Nur Yardim");
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::Magenta); // Genel yaz� rengi
    welcomeText.setStyle(sf::Text::Bold);
    welcomeText.setPosition(windowSize.x / 4, windowSize.y / 3);

    // Duraklatma ve ��k�� metni
    sf::Text pauseText;
    pauseText.setFont(font);
    pauseText.setString("Press 'p' to pause");
    pauseText.setCharacterSize(16);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setPosition(windowSize.x - 200, 20);

    sf::Text exitText;
    exitText.setFont(font);
    exitText.setString("Press 'x' to exit");
    exitText.setCharacterSize(16);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(windowSize.x - 200, 40);

    // Oynatma talimatlar�
    sf::Text playInstructionsText;
    playInstructionsText.setFont(font);
    playInstructionsText.setString("Press Left and Right to play");
    playInstructionsText.setCharacterSize(16);
    playInstructionsText.setFillColor(sf::Color::White);
    playInstructionsText.setPosition(20, windowSize.y - 40);

    // Score metni
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: 0");
    scoreText.setCharacterSize(16);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, 20);

    // Game over metni
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over\nPress Enter to start again");
    gameOverText.setCharacterSize(24);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(windowSize.x / 4, windowSize.y / 3);

    // Oyun ba�lamad���n� belirten bayrak
    bool gameStarted = false;

    // Roket sprite'�n� olu�tur ve resmi ekleyerek ba�lang�� konumuna yerle�tir
    sf::Sprite rocketSprite(rocketTexture);
    rocketSprite.setPosition(windowSize.x / 2 - rocketTexture.getSize().x / 2, windowSize.y - rocketTexture.getSize().y);

    // Mermi sprite'�n� olu�tur ve resmi ekleyerek ba�lang�� konumuna yerle�tir (roketin u� k�sm�nda)
    sf::Sprite bulletSprite(bulletTexture);
    bulletSprite.setPosition(rocketSprite.getPosition().x + rocketTexture.getSize().x / 2 - bulletTexture.getSize().x / 2, rocketSprite.getPosition().y);

    // Y�ld�z sprite'lar�n� depolamak i�in vekt�r
    std::vector<sf::Sprite> stars;

    int score = 0;
    bool bulletFired = false;
    float bulletSpeed = 1.0f; // Mermi h�z�
    float starSpeed = 0.2f;   // Y�ld�z h�z�

    // Arka plan rengi
    sf::Color backgroundColor = sf::Color(255, 182, 193); // Pembe rengi

    // Oyun zamanlay�c�s�
    sf::Clock clock;
    sf::Time gameTime;
    sf::Time gameTimeLimit = sf::seconds(20.0f); // Oyun s�resi 60 saniye

    // Oyun biti� kontrol�
    bool gameOver = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (!gameOver)
                {
                    if (event.key.code == sf::Keyboard::S)
                    {
                        gameStarted = true;
                        clock.restart(); // Oyun zamanlay�c�s�n� ba�lat
                        // Y�ld�zlar� olu�tur ve ekran�n �st k�sm�ndan rastgele konumlara yerle�tir
                        for (int i = 0; i < 10; ++i) {
                            sf::Texture& texture = (std::rand() % 2 == 0) ? starTexture1 : starTexture2;
                            sf::Sprite starSprite(texture);
                            starSprite.setPosition(static_cast<float>(std::rand() % static_cast<int>(windowSize.x)),
                                -static_cast<float>(std::rand() % static_cast<int>(windowSize.y)));

                            // Y�ld�z boyutunu k���lt
                            float scaleFactor = static_cast<float>(std::rand() % 5 + 5) / 10.0f; // Rastgele bir �l�ek fakt�r� belirle (0.5 ile 1 aras�nda)
                            starSprite.setScale(scaleFactor, scaleFactor);

                            stars.push_back(starSprite);
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Left)
                    {
                        // Roketi sola hareket ettir
                        if (rocketSprite.getPosition().x > 0)
                            rocketSprite.move(-10.0f, 0.0f);
                    }
                    else if (event.key.code == sf::Keyboard::Right)
                    {
                        // Roketi sa�a hareket ettir
                        if (rocketSprite.getPosition().x < windowSize.x - rocketTexture.getSize().x)
                            rocketSprite.move(10.0f, 0.0f);
                    }
                    else if (event.key.code == sf::Keyboard::Space)
                    {
                        // "Space" tu�una bas�ld���nda mermiyi ��kart
                        bulletFired = true;
                        bulletSprite.setPosition(rocketSprite.getPosition().x + rocketTexture.getSize().x / 2 - bulletTexture.getSize().x / 2, rocketSprite.getPosition().y);
                    }
                    else if (event.key.code == sf::Keyboard::P)
                    {
                        // Oyunu duraklat
                        gameStarted = false;
                    }
                    else if (event.key.code == sf::Keyboard::X)
                    {
                        // ��k�� yap
                        window.close();
                    }
                }
                else // Game over durumunda
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        // Yeniden ba�lat
                        score = 0;
                        gameOver = false;
                        gameStarted = false;
                        bulletFired = false;
                        stars.clear();
                        clock.restart(); // Oyun zamanlay�c�s�n� s�f�rla
                        welcomeText.setString("SPACE SHOOTER GAME\n\nPress 's' to start\n\n Developed by Eda Nur Yard�m");
                        gameOverText.setPosition(windowSize.x / 4, windowSize.y / 3);
                    }
                }
            }
        }

        window.clear(backgroundColor);

        if (!gameOver && gameStarted) // Oyun ba�lad�ysa ve oyun bitmediyse
        {
            // Oyun zaman�n� g�ncelle
            gameTime = clock.getElapsedTime();

            // Geri say�m� ekrana yazd�r
            sf::Text timerText;
            timerText.setFont(font);
            timerText.setString("Time: " + std::to_string(static_cast<int>(gameTimeLimit.asSeconds() - gameTime.asSeconds())) + "s");
            timerText.setCharacterSize(16);
            timerText.setFillColor(sf::Color::White);
            timerText.setPosition(windowSize.x - 120, 20);
            window.draw(timerText);

            // Zaman kontrol�
            if (gameTime >= gameTimeLimit)
            {
                gameOver = true;
                welcomeText.setString("PRESS ENTER TO START AGAIN");
                gameOverText.setPosition(windowSize.x / 3, windowSize.y / 3);
            }
        }

        // Y�ld�zlar� �iz
        for (auto& star : stars) {
            if (gameStarted) // Oyun ba�lamad�ysa y�ld�zlar� d���rme
                star.move(0.0f, starSpeed);
            window.draw(star);

            if (star.getPosition().y > static_cast<float>(windowSize.y)) {
                star.setPosition(static_cast<float>(std::rand() % static_cast<int>(windowSize.x)), -static_cast<float>(std::rand() % static_cast<int>(windowSize.y)));
            }

            // Mermi ile y�ld�z�n �arp��mas�n� kontrol et
            if (bulletFired && bulletSprite.getGlobalBounds().intersects(star.getGlobalBounds())) {
                // �arp��ma ger�ekle�ti�inde, mermiyi tekrar ba�lang�� konumuna al
                bulletFired = false;
                bulletSprite.setPosition(rocketSprite.getPosition().x + rocketTexture.getSize().x / 2 - bulletTexture.getSize().x / 2, rocketSprite.getPosition().y);

                // Y�ld�z� vurdu�umuz i�in puan� art�r
                ++score;
            }
        }

        // Oyun ba�lamad�ysa sadece kar��lama ekran�n� g�ster
        if (!gameStarted)
        {
            if (!gameOver)
            {
                window.draw(welcomeText);
                window.draw(pauseText);
                window.draw(exitText);
            }
            else
            {
                window.draw(gameOverText); // Game over durumunda gameOverText'i ekrana �iz
            }
        }

        else // Oyun ba�lad�ysa roketi ve mermiyi �iz
        {
            // Mermiyi yukar� do�ru hareket ettir sadece bir kere ��ks�n
            if (bulletFired)
                bulletSprite.move(0.0f, -bulletSpeed);

            // Mermi boyutunu k���lt
            bulletSprite.setScale(0.3f, 0.3f);

            // Roket ve mermiyi �iz
            window.draw(rocketSprite);
            window.draw(bulletSprite);
            window.draw(playInstructionsText);

            // Score'u ekrana yazd�r
            scoreText.setString("Score: " + std::to_string(score));
            window.draw(scoreText);
        }

        window.display();
    }
    return 0;
}
