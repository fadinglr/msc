// Module Name: provider.h
//
// Description:
//
//    This sample illustrates how to develop a layered service provider that is
//    capable of counting all bytes transmitted through a TCP/IP socket.
//
//    This file contains all datatypes and function prototypes used
//    throughout this project.
//
// Compile:
//
//    This project is managed through the LSP.DSW project file.
//
// Execute:
//
//    This project produces a DLL named lsp.dll. This dll should be copied to the
//    %SystemRoot%\System32 directory. Once the file is in place you should execute
//    the application instlsp.exe to insert this provider in the Winsock 2 catalog
//    of service providers.
//
#ifndef _PROVIDER_H_
#define _PROVIDER_H_ 


#define UNICODE
#define _UNICODE

#include <ws2spi.h>
#include <mswsock.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifndef DEBUGLSP
#define DEBUGLSP
#endif

extern WSPUPCALLTABLE MainUpCallTable;
extern WSPPROC_TABLE NextProcTable;
extern GUID ProviderGuid;

LPWSAPROTOCOL_INFOW GetProviders(LPINT TotalProtocols);

LPWSAOVERLAPPED GetOverlappedStructure(SOCKET CallerSocket,
                                       SOCKET ProviderSocket,
                                       LPWSAOVERLAPPED lpCallerOverlapped,
                                       LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCallerCompletionRoutine, 
                                       LPWSATHREADID lpCallerThreadId,
                                       DWORD *ByteCount);

typedef struct {
    SOCKET ProviderSocket;
    DWORD  dwOutstandingAsync;
    BOOL   bClosing;
    DWORD  BytesSent;
    DWORD  BytesRecv;
	BOOL   bNamed;
	sockaddr stDestAddr;
	IP_ADDRESS_STRING AdapterAddrString;
	IP_ADDRESS_STRING GatewayAddrString;
	short PeerSID;
	short HostSID;
} SOCK_INFO;

typedef struct {
	short fromSID;
	short toSID;
	short type;
	short length;
} SessionLayerHeader;

typedef struct {
	IP_ADDRESS_STRING OldIP;
	IP_ADDRESS_STRING NewIP;
} SessionLayerBindingUpdate;

#define MHLSP_DATA_PDU 1
#define MHLSP_UPDATE_PDU 2

#define WM_SOCKET (WM_USER + 1)

HWND SetWorkerWindow(SOCKET ProvSock, SOCKET AppSock, HWND hWnd, UINT uMsg);
BOOL DuplicateAsyncSocket(SOCKET ProvSock, SOCKET NewProvSock, SOCKET NewSock);
BOOL RemoveSockInfo(SOCKET ProvSock);

void InitAsyncSelectCS(void);
void InitOverlappedCS(void);

int WSPAPI WSPCloseSocket (  
    SOCKET s,        
    LPINT lpErrno
);

BOOL PASCAL FAR ExtTransmitFile (
    IN SOCKET hSocket,
    IN HANDLE hFile,
    IN DWORD nNumberOfBytesToWrite,
    IN DWORD nNumberOfBytesPerSend,
    IN LPOVERLAPPED lpOverlapped,
    IN LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers,
    IN DWORD dwReserved);


BOOL PASCAL FAR ExtAcceptEx(
    IN SOCKET sListenSocket,
    IN SOCKET sAcceptSocket,
    IN PVOID lpOutputBuffer,
    IN DWORD dwReceiveDataLength,
    IN DWORD dwLocalAddressLength,
    IN DWORD dwRemoteAddressLength,
    OUT LPDWORD lpdwBytesReceived,
    IN LPOVERLAPPED lpOverlapped);


DWORD MyGetIpForwardTable(PMIB_IPFORWARDTABLE& pIpRouteTab, BOOL fOrder);
void DoDeleteIpForwardEntry(char* pszDest);
bool InterfaceIpToIdxAndMask(PMIB_IPADDRTABLE pIpAddrTable, char str[], DWORD& dwIndex, DWORD& dwMask);
DWORD MyGetIpAddrTable(PMIB_IPADDRTABLE& pIpAddrTable, BOOL fOrder);
DWORD GetAdapterToUse(IP_ADDRESS_STRING &adapterAddrStr, IP_ADDRESS_STRING &gwAddrStr);
BOOL UpdateRoutingTable(IP_ADDRESS_STRING adapterAddrStr, IP_ADDRESS_STRING gwAddrStr, sockaddr_in DestinationAddress);


#endif
