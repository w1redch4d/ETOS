/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efiprot.h

Abstract:

    EFI protocol definitions.

--*/

#if !defined(_MSC_VER) || _MSC_VER > 1000
#pragma once
#endif

#ifndef _EFIPROT_H
#define _EFIPROT_H

//
// Loaded image protocol.
//

#define EFI_LOADED_IMAGE_PROTOCOL_GUID { 0x5b1b31a1, 0x9562, 0x11d2, { 0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }
#define LOADED_IMAGE_PROTOCOL EFI_LOADED_IMAGE_PROTOCOL_GUID

#define EFI_LOADED_IMAGE_PROTOCOL_REVISION 0x1000
#define EFI_IMAGE_INFORMATION_REVISION EFI_LOADED_IMAGE_PROTOCOL_REVISION

typedef
EFI_STATUS
(EFIAPI *EFI_IMAGE_UNLOAD) (
    IN EFI_HANDLE ImageHandle
    );

typedef struct {
    UINT32                   Revision;
    EFI_HANDLE               ParentHandle;
    struct _EFI_SYSTEM_TABLE *SystemTable;

    EFI_HANDLE               DeviceHandle;
    EFI_DEVICE_PATH          *FilePath;
    VOID                     *Reserved;

    UINT32                   LoadOptionsSize;
    VOID                     *LoadOptions;

    VOID                     *ImageBase;
    UINT64                   ImageSize;
    EFI_MEMORY_TYPE          ImageCodeType;
    EFI_MEMORY_TYPE          ImageDataType;

    EFI_IMAGE_UNLOAD         Unload;
} EFI_LOADED_IMAGE, EFI_LOADED_IMAGE_PROTOCOL;

//
// PXE base code protocol.
//

#define EFI_PXE_BASE_CODE_PROTOCOL_GUID { 0x03c4e603, 0xac28, 0x11d3, { 0x9a, 0x2d, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d } }

//
// Base code packet.
//

typedef struct {
    UINT8  BootpOpcode;
    UINT8  BootpHwType;
    UINT8  BootpHwAddrLen;
    UINT8  BootpGateHops;
    UINT32 BootpIdent;
    UINT16 BootpSeconds;
    UINT16 BootpFlags;
    UINT8  BootpCiAddr[4];
    UINT8  BootpYiAddr[4];
    UINT8  BootpSiAddr[4];
    UINT8  BootpGiAddr[4];
    UINT8  BootpHwAddr[16];
    UINT8  BootpSrvName[64];
    UINT8  BootpBootFile[128];
    UINT32 DhcpMagik;
    UINT8  DhcpOptions[56];
} EFI_PXE_BASE_CODE_DHCPV4_PACKET;

typedef struct {
    UINT32 MessageType   : 8;
    UINT32 TransactionId : 24;
    UINT8  DhcpOptions[1024];
} EFI_PXE_BASE_CODE_DHCPV6_PACKET;

typedef union {
    UINT8                           Raw[1472];
    EFI_PXE_BASE_CODE_DHCPV4_PACKET Dhcpv4;
    EFI_PXE_BASE_CODE_DHCPV6_PACKET Dhcpv6;
} EFI_PXE_BASE_CODE_PACKET;

//
// IP receive filter.
//

#define EFI_PXE_BASE_CODE_IP_FILTER_STATION_IP            0x01
#define EFI_PXE_BASE_CODE_IP_FILTER_BROADCAST             0x02
#define EFI_PXE_BASE_CODE_IP_FILTER_PROMISCUOUS           0x04
#define EFI_PXE_BASE_CODE_IP_FILTER_PROMISCUOUS_MULTICAST 0x08

#define EFI_PXE_BASE_CODE_MAX_IPCNT 8

typedef struct {
    UINT8          Filters;
    UINT8          IpCnt;
    UINT16         reserved;
    EFI_IP_ADDRESS IpList[EFI_PXE_BASE_CODE_MAX_IPCNT];
} EFI_PXE_BASE_CODE_IP_FILTER;

//
// Cache entries.
//

typedef struct {
    EFI_IP_ADDRESS  IpAddr;
    EFI_MAC_ADDRESS MacAddr;
} EFI_PXE_BASE_CODE_ARP_ENTRY;

typedef struct {
    EFI_IP_ADDRESS IpAddr;
    EFI_IP_ADDRESS SubnetMask;
    EFI_IP_ADDRESS GwAddr;
} EFI_PXE_BASE_CODE_ROUTE_ENTRY;

//
// Error descriptors.
//

typedef struct {
    UINT8  Type;
    UINT8  Code;
    UINT16 Checksum;

    union {
      UINT32 Reserved;
      UINT32 Mtu;
      UINT32 Pointer;

      struct {
        UINT16 Identifier;
        UINT16 Sequence;
      } Echo;
    } u;

    UINT8  Data[494];
} EFI_PXE_BASE_CODE_ICMP_ERROR;

typedef struct {
    UINT8 ErrorCode;
    CHAR8 ErrorString[127];
} EFI_PXE_BASE_CODE_TFTP_ERROR;

//
// Base code mode.
//

#define EFI_PXE_BASE_CODE_MAX_ARP_ENTRIES   8
#define EFI_PXE_BASE_CODE_MAX_ROUTE_ENTRIES 8

typedef struct {
    BOOLEAN                       Started;
    BOOLEAN                       Ipv6Available;
    BOOLEAN                       Ipv6Supported;
    BOOLEAN                       UsingIpv6;
    BOOLEAN                       BisSupported;
    BOOLEAN                       BisDetected;
    BOOLEAN                       AutoArp;
    BOOLEAN                       SendGUID;
    BOOLEAN                       DhcpDiscoverValid;
    BOOLEAN                       DhcpAckReceived;
    BOOLEAN                       ProxyOfferReceived;
    BOOLEAN                       PxeDiscoverValid;
    BOOLEAN                       PxeReplyReceived;
    BOOLEAN                       PxeBisReplyReceived;
    BOOLEAN                       IcmpErrorReceived;
    BOOLEAN                       TftpErrorReceived;
    BOOLEAN                       MakeCallbacks;
    UINT8                         TTL;
    UINT8                         ToS;
    EFI_IP_ADDRESS                StationIp;
    EFI_IP_ADDRESS                SubnetMask;
    EFI_PXE_BASE_CODE_PACKET      DhcpDiscover;
    EFI_PXE_BASE_CODE_PACKET      DhcpAck;
    EFI_PXE_BASE_CODE_PACKET      ProxyOffer;
    EFI_PXE_BASE_CODE_PACKET      PxeDiscover;
    EFI_PXE_BASE_CODE_PACKET      PxeReply;
    EFI_PXE_BASE_CODE_PACKET      PxeBisReply;
    EFI_PXE_BASE_CODE_IP_FILTER   IpFilter;
    UINT32                        ArpCacheEntries;
    EFI_PXE_BASE_CODE_ARP_ENTRY   ArpCache[EFI_PXE_BASE_CODE_MAX_ARP_ENTRIES];
    UINT32                        RouteTableEntries;
    EFI_PXE_BASE_CODE_ROUTE_ENTRY RouteTable[EFI_PXE_BASE_CODE_MAX_ROUTE_ENTRIES];
    EFI_PXE_BASE_CODE_ICMP_ERROR  IcmpError;
    EFI_PXE_BASE_CODE_TFTP_ERROR  TftpError;
} EFI_PXE_BASE_CODE_MODE;

#define EFI_PXE_BASE_CODE_PROTOCOL_REVISION 0x00010000
#define EFI_PXE_BASE_CODE_INTERFACE_REVISION EFI_PXE_BASE_CODE_PROTOCOL_REVISION

typedef struct {
    UINT64                 Revision;
    VOID                   *Start;
    VOID                   *Stop;
    VOID                   *Dhcp;
    VOID                   *Discover;
    VOID                   *Mtftp;
    VOID                   *UdpWrite;
    VOID                   *UdpRead;
    VOID                   *SetIpFilter;
    VOID                   *Arp;
    VOID                   *SetParameters;
    VOID                   *SetStationIp;
    VOID                   *SetPackets;

    EFI_PXE_BASE_CODE_MODE *Mode;
} EFI_PXE_BASE_CODE, EFI_PXE_BASE_CODE_PROTOCOL;

#endif // _EFIPROT_H
