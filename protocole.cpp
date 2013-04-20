#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "serial.h"

void    menu();
void    readPS(int);

int     pas_fin(char buffer[5000])
{
    int i = 0;

    while (buffer[i])
    {
        if (buffer[i] == '$')
            return (0);
        i++;
    }
    return (1);
}

int     aff_start(int debut, char buffer[5000], int port)
{
    int i = 0;

    if (debut == 0)
    {
        while (buffer[i])
        {
            if (buffer[i] == '#')
            {
                printf("DEBUT TRANSMISSION\n");
                debut = 1;
            }
            else if (buffer[i] == '$')
            {
                printf("ERREUR !!!\n");
                readPS(port);
                exit(0);
            }
            i++;
        }
    }
    return (debut);
}

void    erreur(int port)
{
    printf("Erreur, impossible de lire sur le port %d\n\n", port);
    system("pause");
    system("cls");
    menu();
    exit(0);
}

unsigned char    *convert(char nb)
{
    unsigned char *mot;
    unsigned char c;
    int i = 2, u  = 0;
    unsigned char div = 1;

    while (nb / div * 10 > 0)
    {
        div *= 10;
        i++;
    }
    i--;
    div /= 10;
    mot = new unsigned char[i];
    while (u < i - 1 && div > 0)
    {
        c = nb / div;
        nb -= c * div;
        mot[u] = c + '0';
        u++;
        div /= 10;
    }
    mot[i - 1] = 13;
    return (mot);
}

int     len(unsigned char *mot)
{
    int i = 0;

    while (mot[i] != 13)
        i++;
    return (i + 1);
}

void    printmot(unsigned char *mot)
{
    int i = 0;

    while (mot[i] != 13)
    {
        std::cout << mot[i];
        i++;
    }
    std::cout << std::endl;
}

void    ordre(float x, float y, float z)
{
    int i = -1, haxe = 0;
    unsigned long written;
    unsigned char axe[2] = {0};
    unsigned char *mot;
    unsigned char nb;

   // if (strlen(buffer) > 0)
      //  std::cout << "Elec : " << buffer << "\n";
        nb = 255;
        if (WriteCOM(&nb, 1, &written))
            std::cout << std::endl << "Debut" << std::endl;
        else
            std::cout << "\nECHEC\n";

        nb = x + 50;
        haxe = nb;
        if (WriteCOM(&nb, 1, &written))
            std::cout << "Prog : " << haxe << "X\n";
        else
            std::cout << "\nECHEC\n";

        nb = y + 50;
        haxe = nb;
        if (WriteCOM(&nb, 1, &written))
            std::cout << "Prog : " << haxe << "Y\n";
        else
            std::cout << "\nECHEC\n";

        nb = z + 50;
        haxe = nb;
        if (WriteCOM(&nb, 1, &written))
            std::cout << "Prog : " << haxe << "Z\n";
        else
            std::cout << "\nECHEC\n";
}

void    catch_event(sf::RenderWindow &window, float &x, float &y, float &z)
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
             z = sf::Joystick::getAxisPosition(0, sf::Joystick::R);
            }
        }
    }
}

void    readPS(int port)
{
    char buffer[5000] = {0};
    unsigned long  nBytesRead = 0;
    FILE *fd;
    float x, y, z;
    sf::RenderWindow window(sf::VideoMode(800, 600), "JOYSTICK");

     printf("\nOuverture port COM %d OK\n", port);
    fd = fopen("ReceptionPS.txt", "w");
    printf("Attente de transmition des donnees...\n");
    while (window.isOpen())
     {
        catch_event(window, x, y, z);
        ordre(x / 2.01, y / 2.01, z / 2.01);
        window.display();
        usleep(30000);
     }
    printf("FIN TRANSMISSION\n");
    system("pause");
    fclose(fd);
}
