#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "serial.h"
#include "Erpc.h"
#include <unistd.h>
#include <map>
#include <math.h>

/*Fonction d'acquisition du joystick */
void    catch_event(sf::RenderWindow &window, float &x, float &y, float &z, std::map<int, bool> &button)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Close window : exit
        if (event.type == sf::Event::Closed)
        window.close();
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
                window.close();
        }
        else if (event.type == sf::Event::JoystickMoved)
        {
            if (sf::Joystick::hasAxis(0, sf::Joystick::X))
            {
             x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
            }
            if (sf::Joystick::hasAxis(0, sf::Joystick::Y))
            {
             y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
            }
            if (sf::Joystick::hasAxis(0, sf::Joystick::Z))
            {
             z = sf::Joystick::getAxisPosition(0, sf::Joystick::Z);
            }
        }
        else if (event.type == sf::Event::JoystickButtonPressed)
        {
            if (sf::Joystick::isButtonPressed(0, 0))
            {
                button[1] = true;
            }
            if (sf::Joystick::isButtonPressed(0, 1))
            {
                button[2] = true;
            }
            if (sf::Joystick::isButtonPressed(0, 2))
            {
                button[3] = true;
            }
            if (sf::Joystick::isButtonPressed(0, 3))
            {
                button[4] = true;
            }
            if (sf::Joystick::isButtonPressed(0, 4))
            {
                button[5] = true;
            }
            if (sf::Joystick::isButtonPressed(0, 5))
            {
                button[6] = true;
            }
            if (sf::Joystick::isButtonPressed(0, 6))
            {
                button[7] = true;
            }
            if (sf::Joystick::isButtonPressed(0, 7))
            {
                button[8] = true;
            }
        }
        else if (event.type == sf::Event::JoystickButtonReleased)
        {
           if (!sf::Joystick::isButtonPressed(0, 0))
            {
                button[1] = false;
            }
            if (!sf::Joystick::isButtonPressed(0, 1))
            {
                button[2] = false;
            }
            if (!sf::Joystick::isButtonPressed(0, 2))
            {
                button[3] = false;
            }
            if (!sf::Joystick::isButtonPressed(0, 3))
            {
                button[4] = false;
            }
            if (!sf::Joystick::isButtonPressed(0, 4))
            {
                button[5] = false;
            }
            if (!sf::Joystick::isButtonPressed(0, 5))
            {
                button[6] = false;
            }
            if (!sf::Joystick::isButtonPressed(0, 6))
            {
                button[7] = false;
            }
            if (!sf::Joystick::isButtonPressed(0, 7))
            {
                button[8] = false;
            }
        }
    }
}

int    menu()
{
    int port;

    printf("\nEntrez le numero du port COM : ");
    scanf("%d", &port);
    return (port);
}

int main()
{
    int port;
    FILE *fd;   /* file descriptor du ficher texte des donnees recus*/
    float x, y, z;
    std::map<int, bool> button;
    unsigned char nb;

    system("cls");
    std::cout << "PROGRAMME DE TRANSMISSION DES DONNEES JOYSTICK" << std::endl;

    port = menu();
    Erpc erpc(port);

    /* tant que le port est en erreur alors on redemande le n° du port */
    while (erpc.isOpen() == FALSE)
    {
        port = menu();
        erpc.open(port);
    }

    std::cout << std::endl << "Ouverture port COM " << port << " OK" << std::endl;
    fd = fopen("ReceptionPS.txt", "w");
    std::cout << "Attente de transmition des donnees..." << std::endl;

    /* ouverture de la fenetre */
    sf::RenderWindow window(sf::VideoMode(200, 200), "JOYSTICK");

    for (int i = 1; i <= 8; i++)
        button[i] = false;

    /* boucle principale d'envoie des donnees */
    while (window.isOpen())
     {
        catch_event(window, x, y, z, button);
        system("cls");
        erpc.write(254);         // octet d'accroche de tramme
        nb = (x / 2.01) + 50;

        erpc.write(nb);
        nb = (y / 2.01) + 50;
        erpc.write(nb);
        nb = (z / 2.01) + 50;
        erpc.write(nb);

        nb = 0;
        /* boucle d'encryptage sur 1 octet des boutons joystick */
        for (int i = 1; i <= 8; i++)
            nb += (button[i] == true) ? pow(2, (i - 1)) : 0;
        erpc.write(nb);
        window.display();
        usleep(10000);          // rafrachissement 30 ms
     }
    printf("FIN TRANSMISSION\n");
    system("pause");
    return 0;
}
