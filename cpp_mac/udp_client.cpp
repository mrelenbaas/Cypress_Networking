#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#ifdef _WIN32
  #include <windows.h>
  #ifdef _MSC_VER
    #define snprintf  _snprintf
    #define vsnprintf _vsnprintf
    #define sockerrno WSAGetLastError()
  #else
    #include <ws2tcpip.h>
    #define sockerrno errno
  #endif
  #define socklen_t int
#else
  #include <sys/socket.h>
  #include <netdb.h>
  #ifdef __APPLE__
    #define SOL_IP IPPROTO_IP
  #endif
  #ifndef INVALID_SOCKET
    #define INVALID_SOCKET -1
  #endif
  #define sockerrno errno
#endif

char szLineBuf[500];

int iNetSock = INVALID_SOCKET;
int iRequest = 1;
struct sockaddr_in oAddr;
socklen_t iAddrLen = sizeof(oAddr);

int perr(const char *pszFormat, ...)
{
   va_list argList;
   va_start(argList, pszFormat);
   vsnprintf(szLineBuf, sizeof(szLineBuf)-10, pszFormat, argList);
   szLineBuf[sizeof(szLineBuf)-10] = '\0';
   printf("Error: %s\n", szLineBuf);
   return 0;
}

int netlog(const char *pszFormat, ...)
{
   char szHeadBuf[100];
   int  iHeadLen = 0;

   va_list argList;
   va_start(argList, pszFormat);
   vsnprintf(szLineBuf+100, sizeof(szLineBuf)-110, pszFormat, argList);
   szLineBuf[sizeof(szLineBuf)-10] = '\0';

   // change all [red] to compact color codes \x1Fr
   for (char *psz=szLineBuf+100; *psz; psz++)
      if (psz[0]=='[')
         for (int i=1; psz[i]; i++)
            if (i>=2 && psz[i]==']') {
               psz[0]=0x1F;
               memmove(psz+2, psz+i+1, strlen(psz+i+1)+1);
               break;
            }

   // add sfktxt header before text
   snprintf(szHeadBuf, sizeof(szHeadBuf)-10,
      ":sfktxt:v100,req%d,cs1\n\n", iRequest++);
   iHeadLen = strlen(szHeadBuf);
   char *pData = szLineBuf+100-iHeadLen;
   memcpy(pData, szHeadBuf, iHeadLen);

   sendto(iNetSock, pData, strlen(pData), 0,
      (struct sockaddr *)&oAddr, iAddrLen);

   return 0;
}

int main(int argc, char *argv[])
{
   const char *pszHost = "192.168.1.3";
   unsigned short iPort = 1234;

   #ifdef _MSC_VER
   WORD wVersionRequested = MAKEWORD(1,1);
   WSADATA wsaData;
   if (WSAStartup(wVersionRequested, &wsaData)!=0)
      return 9+perr("WSAStartup failed");
   #endif

   memset((char *)&oAddr, 0,sizeof(oAddr));
   oAddr.sin_family      = AF_INET;
   oAddr.sin_port        = htons(iPort);

   struct hostent *pHost = gethostbyname(pszHost);
   memcpy(&oAddr.sin_addr.s_addr, pHost->h_addr, pHost->h_length);

   if ((iNetSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
      return 9+perr("cannot create socket");

   netlog("[Red]Foo[def] and [Blue]bar[def] "
          "went to the [Green]zoo[def].\n");

   return 0;
}
