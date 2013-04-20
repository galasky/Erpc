#ifndef ERPC_H_INCLUDED
#define ERPC_H_INCLUDED

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/*=============================================================================
  Définition de constantes
=============================================================================*/
#define RX_SIZE         4096    /* taille tampon d'entrée  */
#define TX_SIZE         4096    /* taille tampon de sortie */
#define MAX_WAIT_READ   1    /* temps max d'attente pour lecture (en ms) */

class   Erpc
{
  public:
    Erpc(int port);
    ~Erpc();
    void    open(int port);
    void    write(unsigned char c);
    BOOL    isOpen();


  private:
  int   _port;
  BOOL  _isOpen;

  /* Handle du port COM ouvert */
HANDLE g_hCOM;

/* Délais d'attente sur le port COM */
COMMTIMEOUTS g_cto;

/* Configuration du port COM */
DCB g_dcb;

  BOOL  openCOM();
  BOOL  closeCOM();
  BOOL  readCOM(void* buffer, unsigned long nBytesToRead, unsigned long* pBytesRead);
  BOOL  writeCOM(void* buffer, int nBytesToWrite, unsigned long* pBytesWritten);
};

#endif // ERPC_H_INCLUDED
