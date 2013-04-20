#ifndef PROTOCOLE_H_INCLUDED
#define PROTOCOLE_H_INCLUDED

int     pas_fin(char buffer[5000]);
int     aff_start(int debut, char buffer[5000], int port);
void    readPS(int port);

#endif // PROTOCOLE_H_INCLUDED
