#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/*=============================================================================
  Fonctions du module.
=============================================================================*/
BOOL OpenCOM    (int nId);
BOOL CloseCOM   ();
BOOL ReadCOM    (void* buffer, unsigned long nBytesToRead, unsigned long* pBytesRead);
BOOL WriteCOM   (void* buffer, int nBytesToWrite, unsigned long* pBytesWritten);

#endif // SERIAL_H_INCLUDED
