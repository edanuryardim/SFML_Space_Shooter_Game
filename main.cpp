#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <ctime>

int main()
{
    // Rastgele sayý üretmek için tohum ayarla
    std::srand(std::time(nullptr));

    // Pencere boyutlarý
    sf::Vector2u windowSize(800, 600);

    // Pencereyi oluþtur
    sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Space Shooter Game");

    // Yazý fontunu yükle
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Hata kontrolü
        return 1;
    }

    // Roket resmini yükle
    sf::Texture rocketTexture;
    if (!rocketTexture.loadFromFile("rocket.bmp")) {
        // Hata kontrolü
        return 1;
    }

    // Mermi resmini yükle
    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("mermi.bmp")) {
        // Hata kontrolü
        return 1;
    }

    // Yýldýz resimlerini yükle
    sf::Texture starTexture1;
    if (!starTexture1.loadFromFile("star1.bmp")) {
        // Hata kontrolü
        return 1;
    }

    sf::Texture starTexture2;
    if (!starTexture2.loadFromFile("star2.bmp")) {
        // Hata kontrolü
        return 1;
    }

    // Genel karþýlama yazýsý
    sf::Text welcomeText;
    welcomeText.setFont(font);
    welcomeText.setString("SPACE SHOOTER GAME\n\nPress 's' to start\n\n Developed by Eda Nur Yardim");
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::Magenta); // Genel yazý rengi
    welcomeText.setStyle(sf::Text::Bold);
    welcomeText.setPosition(windowSize.x / 4, windowSize.y / 3);

    // Duraklatma ve çýkýþ metni
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

    // Oynatma talimatlarý
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

    // Oyun baþlamadýðýný belirten bayrak
    bool gameStarted = false;

    // Roket sprite'ýný oluþtur ve resmi ekleyerek baþlangýç konumuna yerleþtir
    sf::Sprite rocketSprite(rocketTexture);
    rocketSprite.setPosition(windowSize.x / 2 - rocketTexture.getSize().x / 2, windowSize.y - rocketTexture.getSize().y);

    // Mermi sprite'ýný oluþtur ve resmi ekleyerek baþlangýç konumuna yerleþtir (roketin uç kýsmýnda)
    sf::Sprite bulletSprite(bulletTexture);
    bulletSprite.setPosition(rocketSprite.getPosition().x + rocketTexture.getSize().x / 2 - bulletTexture.getSize().x / 2, rocketSprite.getPosition().y);

    // Yýldýz sprite'larýný depolamak için vektör
    std::vector<sf::Sprite> stars;

    int score = 0;
    bool bulletFired = false;
    float bulletSpeed = 1.0f; // Mermi hýzý
    float starSpeed = 0.2f;   // Yýldýz hýzý

    // Arka plan rengi
    sf::Color backgroundColor = sf::Color(255, 182, 193); // Pembe rengi

    // Oyun zamanlayýcýsý
    sf::Clock clock;
    sf::Time gameTime;
    sf::Time gameTimeLimit = sf::seconds(20.0f); // Oyun süresi 60 saniye

    // Oyun bitiþ kontrolü
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
                        clock.restart(); // Oyun zamanlayýcýsýný baþlat
                        // Yýldýzlarý oluþtur ve ekranýn üst kýsmýndan rastgele konumlara yerleþtir
                        for (int i = 0; i < 10; ++i) {
                            sf::Texture& texture = (std::rand() % 2 == 0) ? starTexture1 : starTexture2;
                            sf::Sprite starSprite(texture);
                            starSprite.setPosition(static_cast<float>(std::rand() % static_cast<int>(windowSize.x)),
                                -static_cast<float>(std::rand() % static_cast<int>(windowSize.y)));

                            // Yýldýz boyutunu küçült
                            float scaleFactor = static_cast<float>(std::rand() % 5 + 5) / 10.0f; // Rastgele bir ölçek faktörü belirle (0.5 ile 1 arasýnda)
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
                        // Roketi saða hareket ettir
                        if (rocketSprite.getPosition().x < windowSize.x - rocketTexture.getSize().x)
                            rocketSprite.move(10.0f, 0.0f);
                    }
                    else if (event.key.code == sf::Keyboard::Space)
                    {
                        // "Space" tuþuna basýldýðýnda mermiyi çýkart
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
                        // Çýkýþ yap
                        window.close();
                    }
                }
                else // Game over durumunda
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        // Yeniden baþlat
                        score = 0;
                        gameOver = false;
                        gameStarted = false;
                        bulletFired = false;
                        stars.clear();
                        clock.restart(); // Oyun zamanlayýcýsýný sýfýrla
                        welcomeText.setString("SPACE SHOOTER GAME\n\nPress 's' to start\n\n Developed by Eda Nur Yardým");
                        gameOverText.setPosition(windowSize.x / 4, windowSize.y / 3);
                    }
                }
            }
        }

        window.clear(backgroundColor);

        if (!gameOver && gameStarted) // Oyun baþladýysa ve oyun bitmediyse
        {
            // Oyun zamanýný güncelle
            gameTime = clock.getElapsedTime();

            // Geri sayýmý ekrana yazdýr
            sf::Text timerText;
            timerText.setFont(font);
            timerText.setString("Time: " + std::to_string(static_cast<int>(gameTimeLimit.asSeconds() - gameTime.asSeconds())) + "s");
            timerText.setCharacterSize(16);
            timerText.setFillColor(sf::Color::White);
            timerText.setPosition(windowSize.x - 120, 20);
            window.draw(timerText);

            // Zaman kontrolü
            if (gameTime >= gameTimeLimit)
            {
                gameOver = true;
                welcomeText.setString("PRESS ENTER TO START AGAIN");
                gameOverText.setPosition(windowSize.x / 3, windowSize.y / 3);
            }
        }

        // Yýldýzlarý çiz
        for (auto& star : stars) {
            if (gameStarted) // Oyun baþlamadýysa yýldýzlarý düþürme
                star.move(0.0f, starSpeed);
            window.draw(star);

            if (star.getPosition().y > static_cast<float>(windowSize.y)) {
                star.setPosition(static_cast<float>(std::rand() % static_cast<int>(windowSize.x)), -static_cast<float>(std::rand() % static_cast<int>(windowSize.y)));
            }

            // Mermi ile yýldýzýn çarpýþmasýný kontrol et
            if (bulletFired && bulletSprite.getGlobalBounds().intersects(star.getGlobalBounds())) {
                // Çarpýþma gerçekleþtiðinde, mermiyi tekrar baþlangýç konumuna al
                bulletFired = false;
                bulletSprite.setPosition(rocketSprite.getPosition().x + rocketTexture.getSize().x / 2 - bulletTexture.getSize().x / 2, rocketSprite.getPosition().y);

                // Yýldýzý vurduðumuz için puaný artýr
                ++score;
            }
        }

        // Oyun baþlamadýysa sadece karþýlama ekranýný göster
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
                window.draw(gameOverText); // Game over durumunda gameOverText'i ekrana çiz
            }
        }

        else // Oyun baþladýysa roketi ve mermiyi çiz
        {
            // Mermiyi yukarý doðru hareket ettir sadece bir kere çýksýn
            if (bulletFired)
                bulletSprite.move(0.0f, -bulletSpeed);

            // Mermi boyutunu küçült
            bulletSprite.setScale(0.3f, 0.3f);

            // Roket ve mermiyi çiz
            window.draw(rocketSprite);
            window.draw(bulletSprite);
            window.draw(playInstructionsText);

            // Score'u ekrana yazdýr
            scoreText.setString("Score: " + std::to_string(score));
            window.draw(scoreText);
        }

        window.display();
    }
    return 0;
}
