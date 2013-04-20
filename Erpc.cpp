#include "Erpc.h"

Erpc::Erpc(int port)
{
    _port = port;
    g_hCOM = NULL;
    g_cto  =
    {
        MAX_WAIT_READ,  /* ReadIntervalTimeOut          */
        0,              /* ReadTotalTimeOutMultiplier   */
        MAX_WAIT_READ,  /* ReadTotalTimeOutConstant     */
        0,              /* WriteTotalTimeOutMultiplier  */
        0               /* WriteTotalTimeOutConstant    */
    };
    g_dcb =
    {
        sizeof(DCB),        /* DCBlength            */
        9600,               /* BaudRate             */
        TRUE,               /* fBinary              */
        FALSE,              /* fParity              */
        FALSE,              /* fOutxCtsFlow         */
        FALSE,              /* fOutxDsrFlow         */
        DTR_CONTROL_ENABLE, /* fDtrControl          */
        FALSE,              /* fDsrSensitivity      */
        FALSE,              /* fTXContinueOnXoff    */
        FALSE,              /* fOutX                */
        FALSE,              /* fInX                 */
        FALSE,              /* fErrorChar           */
        FALSE,              /* fNull                */
        RTS_CONTROL_ENABLE, /* fRtsControl          */
        FALSE,              /* fAbortOnError        */
        0,                  /* fDummy2              */
        0,                  /* wReserved            */
        0x100,              /* XonLim               */
        0x100,              /* XoffLim              */
        8,                  /* ByteSize             */
        NOPARITY,           /* Parity               */
        ONESTOPBIT,         /* StopBits             */
        0x11,               /* XonChar              */
        0x13,               /* XoffChar             */
        '?',                /* ErrorChar            */
        0x1A,               /* EofChar              */
        0x10                /* EvtChar              */
    };
  _isOpen = openCOM();

}

Erpc::~Erpc()
{
    closeCOM();
}

void    Erpc::open(int port)
{
    _port = port;
    _isOpen = openCOM();
}

BOOL    Erpc::isOpen()
{
    return (_isOpen);
}

void    Erpc::write(unsigned char c)
{
    unsigned long nbwritten;

    writeCOM(&c, 1, &nbwritten);
}

BOOL Erpc::openCOM()
{
    /* variables locales */
    char szCOM[16];

    /* construction du nom du port, tentative d'ouverture */
    sprintf(szCOM, "COM%d", _port);
    g_hCOM = CreateFile(szCOM, GENERIC_READ|GENERIC_WRITE, 0, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, NULL);
    if(g_hCOM == INVALID_HANDLE_VALUE)
    {
        printf("Erreur lors de l'ouverture du port COM%d\n\n", _port);
        return FALSE;
    }

    /* affectation taille des tampons d'émission et de réception */
    SetupComm(g_hCOM, RX_SIZE, TX_SIZE);

    /* configuration du port COM */
    if(!SetCommTimeouts(g_hCOM, &g_cto) || !SetCommState(g_hCOM, &g_dcb))
    {
        printf("Erreur lors de la configuration du port COM%d", _port);
        CloseHandle(g_hCOM);
        return FALSE;
    }

    /* on vide les tampons d'émission et de réception, mise à 1 DTR */
    PurgeComm(g_hCOM, PURGE_TXCLEAR|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_RXABORT);
    EscapeCommFunction(g_hCOM, SETDTR);
    return TRUE;
}

BOOL Erpc::closeCOM()
{
    /* fermeture du port COM */
    CloseHandle(g_hCOM);
    return TRUE;
}

BOOL Erpc::readCOM(void* buffer, unsigned long nBytesToRead, unsigned long* pBytesRead)
{
    return ReadFile(g_hCOM, buffer, nBytesToRead, pBytesRead, NULL);
}

BOOL Erpc::writeCOM(void* buffer, int nBytesToWrite, unsigned long* pBytesWritten)
{
    /* écriture sur le port */
    return WriteFile(g_hCOM, buffer, nBytesToWrite, pBytesWritten, NULL);
}
