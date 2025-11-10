/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efidevp.h

Abstract:

    Provides EFI device path protocol definitions.

--*/

#pragma once

#ifndef _EFIDEVP_H
#define _EFIDEVP_H

#define EFI_DEVICE_PATH_PROTOCOL_GUID { 0x09576e91, 0x6d3f, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }

//
// Device path node.
//
typedef struct {
    UINT8 Type;
    UINT8 SubType;

    //
    // Interpretation determined by Type and SubType.
    //
    UINT8 Length[2];
} EFI_DEVICE_PATH, EFI_DEVICE_PATH_PROTOCOL;

//
// Device path types.
//

#define HARDWARE_DEVICE_PATH  0x01
#define ACPI_DEVICE_PATH      0x02
#define MESSAGING_DEVICE_PATH 0x03
#define MEDIA_DEVICE_PATH     0x04
#define BBS_DEVICE_PATH       0x05
#define END_DEVICE_PATH_TYPE  0x7f

#define EFI_DP_TYPE_MASK     0x7f
#define EFI_DP_TYPE_UNPACKED 0x80

//
// End device path subtypes.
//
#define END_INSTANCE_DEVICE_PATH_SUBTYPE 0x01
#define END_ENTIRE_DEVICE_PATH_SUBTYPE   0xff

//
// End device path length.
//
#define END_DEVICE_PATH_LENGTH (sizeof(EFI_DEVICE_PATH_PROTOCOL))

//
// Hardware device path subtypes.
//
#define HW_PCI_DP        0x01
#define HW_PCCARD_DP     0x02
#define HW_MEMMAP_DP     0x03
#define HW_VENDOR_DP     0x04
#define HW_CONTROLLER_DP 0x05
#define HW_BMC_DP        0x06

//
// PCI (Peripheral Component Interconnect) hardware subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT8 Function;
    UINT8 Device;
} PCI_DEVICE_PATH;

//
// PCCard hardware subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT8 FunctionNumber;
} PCCARD_DEVICE_PATH;

//
// Memory map hardware subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // EFI_MEMORY_TYPE.
    //
    UINT32 MemoryType;

    EFI_PHYSICAL_ADDRESS StartingAddress;
    EFI_PHYSICAL_ADDRESS EndingAddress;
} MEMMAP_DEVICE_PATH;

//
// Vendor hardware subtype.
//

#define UNKNOWN_DEVICE_GUID \
    { 0xcf31fac5, 0xc24e, 0x11d2, { 0x85, 0xf3, 0x00, 0xa0, 0xc9, 0x3e, 0xc9, 0x3b } }

typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // Vendor-assigned GUID.
    //
    EFI_GUID Guid;

    //
    // Vendor-defined variable-size data.
    //
} VENDOR_DEVICE_PATH;

typedef struct {
    VENDOR_DEVICE_PATH DevicePath;
    UINT8 LegacyDriveLetter;
} UNKNOWN_DEVICE_VENDOR_DEVICE_PATH;

//
// Controller hardware subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT32 ControllerNumber;
} CONTROLLER_DEVICE_PATH;

//
// BMC (Baseboard Management Controller) hardware sutype.
//

#define BMC_INTERFACE_TYPE_UNKNOWN 0x00
#define BMC_INTERFACE_TYPE_KCS     0x01
#define BMC_INTERFACE_TYPE_SMIC    0x02
#define BMC_INTERFACE_TYPE_BT      0x03

typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT8  InterfaceType;
    UINT64 BaseAddress;
} BMC_DEVICE_PATH;

//
// ACPI device path subtypes.
//
#define ACPI_DP          0x01
#define ACPI_EXTENDED_DP 0x02
#define ACPI_ADR_DP      0x03
#define ACPI_NVDIMM_DP   0x04

//
// EISA ID constants.
//
#define PNP_EISA_ID_CONST 0x41d0
#define PNP_EISA_ID_MASK  0xffff

//
// EISA ID helpers.
//
#define EISA_ID(name, num) ((num) << 16) | (UINT32)((name))
#define EISA_ID_TO_NUM(id) ((id) >> 16)
#define EISA_PNP_ID(pnpId) (EISA_ID(PNP_EISA_ID_CONST, (pnpId)))
#define EFI_PNP_ID         EISA_PNP_ID

//
// ACPI subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // EISA-compressed PnP hardware ID.
    //
    UINT32 HID;

    //
    // Unique ID.
    //
    UINT32 UID;
} ACPI_HID_DEVICE_PATH;

//
// Extended ACPI subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // EISA-compressed PnP hardware ID.
    //
    UINT32 HID;

    //
    // Unique ID.
    //
    UINT32 UID;

    //
    // EISA-compressed compatible PnP hardware ID.
    //
    UINT32 CID;
} ACPI_EXTENDED_HID_DEVICE_PATH;

//
// ADR ACPI subtype.
//

#define ACPI_ADR_DISPLAY_TYPE_OTHER            0
#define ACPI_ADR_DISPLAY_TYPE_VGA              1
#define ACPI_ADR_DISPLAY_TYPE_TV               2
#define ACPI_ADR_DISPLAY_TYPE_EXTERNAL_DIGITAL 3
#define ACPI_ADR_DISPLAY_TYPE_INTERNAL_DIGITAL 4

#define ACPI_DISPLAY_ADR(scheme, headId, nonVgaOutput, biosCanDetect, vendorInfo, type, port, index) \
    ((UINT32)( \
        ((UINT32)((scheme) & 0x1) << 31) | \
        (((headId) & 0x7) << 18) | \
        (((nonVgaOutput) & 0x1) << 17) | \
        (((biosCanDetect) & 0x1) << 16) | \
        (((vendorInfo) & 0xf) << 12) | \
        (((type) & 0xf) << 8) | \
        (((port) & 0xf) << 4) | \
        ((index) & 0xf) \
    ))

typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // ACPI _ADR value.
    //
    UINT32 ADR;
} ACPI_ADR_DEVICE_PATH;

//
// NVDIMM ACPI subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // Unique physical identifier of NFIT device.
    //
    UINT32 NFITDeviceHandle;
} ACPI_NVDIMM_DEVICE_PATH;

//
// Messaging device subtypes.
//
#define MSG_ATAPI_DP               0x01
#define MSG_SCSI_DP                0x02
#define MSG_FIBRECHANNEL_DP        0x03
#define MSG_1394_DP                0x04
#define MSG_USB_DP                 0x05
#define MSG_I2O_DP                 0x06
#define MSG_INFINIBAND_DP          0x09
#define MSG_VENDOR_DP              0x0a
#define MSG_MAC_ADDR_DP            0x0b
#define MSG_IPv4_DP                0x0c
#define MSG_IPv6_DP                0x0d
#define MSG_UART_DP                0x0e
#define MSG_USB_CLASS_DP           0x0f
#define MSG_USB_WWID_DP            0x10
#define MSG_DEVICE_LOGICAL_UNIT_DP 0x11
#define MSG_SATA_DP                0x12
#define MSG_ISCSI_DP               0x13
#define MSG_VLAN_DP                0x14
#define MSG_FIBRECHANNELEX_DP      0x15
#define MSG_SASEX_DP               0x16
#define MSG_NVME_NAMESPACE_DP      0x17
#define MSG_URI_DP                 0x18
#define MSG_UFS_DP                 0x19
#define MSG_SD_DP                  0x1a
#define MSG_BLUETOOTH_DP           0x1b
#define MSG_WIFI_DP                0x1c
#define MSG_EMMC_DP                0x1d
#define MSG_BLUETOOTH_LE_DP        0x1e
#define MSG_DNS_DP                 0x1f
#define MSG_NVDIMM_NAMESPACE_DP    0x20
#define MSG_NVME_OF_NAMESPACE_DP   0x22

//
// ATAPI (ATA Packet Interface) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // 0 (primary) or 1 (secondary).
    //
    UINT8  PrimarySecondary;

    //
    // 0 (master) or 1 (slave).
    //
    UINT8  SlaveMaster;

    //
    // LUN (Logical Unit Number).
    //
    UINT16 Lun;
} ATAPI_DEVICE_PATH;

//
// SCSI (Small Computer System Interface) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // PUN (Physical Unit Number), aka SCSI ID.
    //
    UINT16 Pun;

    //
    // LUN (Logical Unit Number).
    //
    UINT16 Lun;
} SCSI_DEVICE_PATH;

//
// Fibre Channel messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT32 Reserved;

    //
    // WWN (WorldWide Number).
    //
    UINT64 WWN;

    //
    // LUN (Logical Unit Number).
    //
    UINT64 Lun;
} FIBRECHANNEL_DEVICE_PATH;

//
// 1394 messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT32 Reserved;
    UINT64 Guid;
} F1394_DEVICE_PATH;

//
// USB (Universal Serial Bus) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT8 ParentPortNumber;
    UINT8 InterfaceNumber;
} USB_DEVICE_PATH;

//
// I2O (Intelligent I/O) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // TID (Target ID).
    //
    UINT32 Tid;
} I2O_DEVICE_PATH;

//
// InfiniBand messaging subtype.
//

#define INFINIBAND_RESOURCE_FLAG_IOC_SERVICE               0x01
#define INFINIBAND_RESOURCE_FLAG_EXTENDED_BOOT_ENVIRONMENT 0x02
#define INFINIBAND_RESOURCE_FLAG_CONSOLE_PROTOCOL          0x04
#define INFINIBAND_RESOURCE_FLAG_STORAGE_PROTOCOL          0x08
#define INFINIBAND_RESOURCE_FLAG_NETWORK_PROTOCOL          0x10

typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // INFINIBAND_RESOURCE_FLAG_*.
    //
    UINT32 ResourceFlags;

    UINT8  PortGid[16];
    UINT64 ServiceId;
    UINT64 TargetPortId;
    UINT64 DeviceId;
} INFINIBAND_DEVICE_PATH;

//
// Vendor messaging subtype.
// Use VENDOR_DEVICE_PATH struct.
//

//
// UART flow control vendor protocol.
//

#define EFI_VT_100_PLUS_GUID \
    { 0x7baec70b, 0x57e0, 0x4c76, { 0x8e, 0x87, 0x2f, 0x9e, 0x28, 0x08, 0x83, 0x43 } }

#define EFI_VT_UTF8_GUID \
    { 0xad15a0d6, 0x8bec, 0x4acf, { 0xa0, 0x73, 0xd0, 0x1d, 0xe7, 0x7e, 0x2d, 0x88 } }

#define EFI_VT_100_GUID \
    { 0xdfa66065, 0xb419, 0x11d3, { 0x9a, 0x2d, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d } }

#define EFI_PC_ANSI_GUID \
    { 0xe0c14753, 0xf9be, 0x11d2, { 0x9a, 0x0c, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d } }

#define DEVICE_PATH_MESSAGING_PC_ANSI     EFI_PC_ANSI_GUID
#define DEVICE_PATH_MESSAGING_VT_100      EFI_VT_100_GUID
#define DEVICE_PATH_MESSAGING_VT_100_PLUS EFI_VT_100_PLUS_GUID
#define DEVICE_PATH_MESSAGING_VT_UTF8     EFI_VT_UTF8_GUID

#define UART_FLOW_CONTROL_HARDWARE 0x00000001
#define UART_FLOW_CONTROL_XON_XOFF 0x00000010

typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    EFI_GUID Guid;
    UINT32   FlowControlMap;
} UART_FLOW_CONTROL_DEVICE_PATH;

//
// SAS (Serial-Attached SCSI) vendor protocol.
//

#define DEVICE_PATH_MESSAGING_SAS EFI_SAS_DEVICE_PATH_GUID

typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    EFI_GUID Guid;
    UINT32   Reserved;

    //
    // SAS address.
    //
    UINT64   SasAddress;

    //
    // LUN (Logical Unit Number).
    //
    UINT64   Lun;

    UINT16   DeviceTopology;
    UINT16   RelativeTargetPort;
} SAS_DEVICE_PATH;

//
// MAC (Medium Access Control) address messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    EFI_MAC_ADDRESS MacAddress;

    //
    // Interface type (802.11 etc).
    //
    UINT8           IfType;
} MAC_ADDR_DEVICE_PATH;

//
// IPv4 messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    EFI_IPv4_ADDRESS LocalIpAddress;
    EFI_IPv4_ADDRESS RemoteIpAddress;
    UINT16           LocalPort;
    UINT16           RemotePort;
    UINT16           Protocol;
    BOOLEAN          StaticIpAddress;

    //
    // UEFI 2.0+ only.
    //
    EFI_IPv4_ADDRESS GatewayIpAddress;
    EFI_IPv4_ADDRESS SubnetMask;
} IPv4_DEVICE_PATH;

//
// IPv6 messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    EFI_IPv6_ADDRESS LocalIpAddress;
    EFI_IPv6_ADDRESS RemoteIpAddress;
    UINT16           LocalPort;
    UINT16           RemotePort;
    UINT16           Protocol;
    UINT8            IpAddressOrigin;

    //
    // UEFI 2.0+ only.
    //
    UINT8            PrefixLength;
    EFI_IPv6_ADDRESS GatewayIpAddress;
} IPv6_DEVICE_PATH;

//
// UART messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT32 Reserved;
    UINT64 BaudRate;
    UINT8  DataBits;

    //
    // Parity configuration.
    // 0x00 - Default.
    // 0x01 - None.
    // 0x02 - Even.
    // 0x03 - Odd.
    // 0x04 - Mark.
    // 0x05 - Space.
    //
    UINT8  Parity;

    //
    // Number of stop bits.
    // 0x00 - Default.
    // 0x01 - 1.
    // 0x02 - 1.5.
    // 0x03 - 2.
    //
    UINT8  StopBits;
} UART_DEVICE_PATH;

//
// USB class messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // 0xffff will match any vendor/product ID.
    //
    UINT16 VendorId;
    UINT16 ProductId;

    //
    // 0xff will match any class/subclass/protocol.
    //
    UINT8  DeviceClass;
    UINT8  DeviceSubClass;
    UINT8  DeviceProtocol;
} USB_CLASS_DEVICE_PATH;

//
// USB WWID (WorldWide IDentifier) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT16 InterfaceNumber;
    UINT16 VendorId;
    UINT16 ProductId;
    CHAR16 SerialNumber[];
} USB_WWID_DEVICE_PATH;

//
// Device LUN (Logical Unit Number) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // LUN (Logical Unit Number).
    //
    UINT8 Lun;
} DEVICE_LOGICAL_UNIT_DEVICE_PATH;

//
// SATA (Serial ATA) messaging subtype.
//

#define SATA_HBA_DIRECT_CONNECT_FLAG 0x8000

typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // 0xffff is reserved.
    //
    UINT16 HBAPortNumber;

    //
    // 0xffff if directly connected to HBA.
    //
    UINT16 PortMultiplierPortNumber;

    //
    // LUN (Logical Unit Number).
    //
    UINT16 Lun;
} SATA_DEVICE_PATH;

//
// iSCSI (Internet SCSI) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // 0 if TCP.
    //
    UINT16 NetworkProtocol;

    UINT16 LoginOption;

    //
    // LUN (Logical Unit Number).
    //
    UINT64 Lun;

    UINT16 TargetPortalGroupTag;
} ISCSI_DEVICE_PATH;

//
// VLAN (Virtual LAN) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // Must be in range 0-4094.
    //
    UINT16 VlanId;
} VLAN_DEVICE_PATH;

//
// Extended Fibre Channel messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT32 Reserved;

    //
    // WWN (WorldWide Number).
    //
    UINT8  WWN[8];

    //
    // LUN (Logical Unit Number).
    //
    UINT8  Lun[8];
} FIBRECHANNELEX_DEVICE_PATH;

//
// Extended SAS (Serial-Attached SCSI) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT8  SasAddress[8];

    //
    // LUN (Logical Unit Number).
    //
    UINT8  Lun[8];

    UINT16 DeviceTopology;
    UINT16 RelativeTargetPort;
} SASEX_DEVICE_PATH;

//
// NVME (NVM Express) namespace messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT32 NamespaceId;
    UINT64 NamespaceUuid;
} NVME_NAMESPACE_DEVICE_PATH;

//
// URI (Uniform Resource Identifier) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    CHAR8 Uri[];
} URI_DEVICE_PATH;

//
// UFS (Universal Flash Storage) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // PUN (Physical Unit Number).
    //
    UINT8 Pun;

    //
    // LUN (Logical Unit Number).
    //
    UINT8 Lun;
} UFS_DEVICE_PATH;

//
// SD (Secure Digital) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT8 SlotNumber;
} SD_DEVICE_PATH;

//
// Bluetooth messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    BLUETOOTH_ADDRESS Address;
} BLUETOOTH_DEVICE_PATH;

//
// Wi-Fi messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;

    //
    // SSID (Service Set Identifier).
    //
    UINT8 SSId[32];
} WIFI_DEVICE_PATH;

//
// EMMC (Embedded MMC) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT8 SlotNumber;
} EMMC_DEVICE_PATH;

//
// Bluetooth LE (Low Energy) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    BLUETOOTH_LE_ADDRESS Address;
} BLUETOOTH_LE_DEVICE_PATH;

//
// DNS (Domain Name Service) messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT8          IsIPv6;
    EFI_IP_ADDRESS DnsServerIp[];
} DNS_DEVICE_PATH;

//
// NVDIMM (Non-Volatile DIMM) namespace messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    EFI_GUID Uuid;
} NVDIMM_NAMESPACE_DEVICE_PATH;

//
// NVME-oF (NVME over Fabric) namespace messaging subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT8 NamespaceIdType;
    UINT8 NamespaceId[16];
    CHAR8 SubsystemNqn[];
} NVME_OF_NAMESPACE_DEVICE_PATH;

//
// Media device path subtypes.
//
#define MEDIA_HARDDRIVE_DP             0x01
#define MEDIA_CDROM_DP                 0x02
#define MEDIA_VENDOR_DP                0x03
#define MEDIA_FILEPATH_DP              0x04
#define MEDIA_PROTOCOL_DP              0x05
#define MEDIA_PIWG_FW_FILE_DP          0x06
#define MEDIA_PIWG_FW_VOL_DP           0x07
#define MEDIA_RELATIVE_OFFSET_RANGE_DP 0x08
#define MEDIA_RAM_DISK_DP              0x09

//
// Hard drive media subtype.
//

#define MBR_TYPE_PCAT                       0x01
#define MBR_TYPE_EFI_PARTITION_TABLE_HEADER 0x02

#define NO_DISK_SIGNATURE   0x00
#define SIGNATURE_TYPE_MBR  0x01
#define SIGNATURE_TYPE_GUID 0x02

typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT32 PartitionNumber;
    UINT64 PartitionStart;
    UINT64 PartitionSize;
    UINT8  Signature[16];
    UINT8  MBRType;
    UINT8  SignatureType;
} HARDDRIVE_DEVICE_PATH;

//
// CD-ROM (CD Read-Only Memory) media subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT32 BootEntry;
    UINT64 PartitionStart;
    UINT64 PartitionSize;
} CDROM_DEVICE_PATH;

//
// Vendor media subtype.
// Use VENDOR_DEVICE_PATH structure.
//

//
// File path media subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    CHAR16 PathName[1];
} FILEPATH_DEVICE_PATH;

//
// Protocol media subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    EFI_GUID Protocol;
} MEDIA_PROTOCOL_DEVICE_PATH;

//
// PIWG (Platform Initialization Working Group) firmware file media subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    EFI_GUID FvFileName;
} MEDIA_FW_VOL_FILEPATH_DEVICE_PATH;

//
// PIWG (Platform Initialization Working Group) firmware volume media subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    EFI_GUID FvName;
} MEDIA_FW_VOL_DEVICE_PATH;

//
// Relative offset range media subtype.
//
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT32 Reserved;
    UINT64 StartingOffset;
    UINT64 EndingOffset;
} MEDIA_RELATIVE_OFFSET_RANGE_DEVICE_PATH;

//
// RAM disk media subtype.
//

#define EFI_VIRTUAL_DISK_GUID            EFI_ACPI_6_0_NFIT_GUID_RAM_DISK_SUPPORTING_VIRTUAL_DISK_REGION_VOLATILE
#define EFI_VIRTUAL_CD_GUID              EFI_ACPI_6_0_NFIT_GUID_RAM_DISK_SUPPORTING_VIRTUAL_CD_REGION_VOLATILE
#define EFI_PERSISTENT_VIRTUAL_DISK_GUID EFI_ACPI_6_0_NFIT_GUID_RAM_DISK_SUPPORTING_VIRTUAL_DISK_REGION_PERSISTENT
#define EFI_PERSISTENT_VIRTUAL_CD_GUID   EFI_ACPI_6_0_NFIT_GUID_RAM_DISK_SUPPORTING_VIRTUAL_CD_REGION_PERSISTENT

typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT32   StartingAddr[2];
    UINT32   EndingAddr[2];
    EFI_GUID TypeGuid;
    UINT16   Instance;
} MEDIA_RAM_DISK_DEVICE_PATH;

//
// BBS (BIOS Boot Specification) device path subtypes.
//
#define BBS_BBS_DP 0x01

//
// BBS (BIOS Boot Specification) BBS subtype.
//

#define BBS_TYPE_FLOPPY           0x01
#define BBS_TYPE_HARDDRIVE        0x02
#define BBS_TYPE_CDROM            0x03
#define BBS_TYPE_PCMCIA           0x04
#define BBS_TYPE_USB              0x05
#define BBS_TYPE_EMBEDDED_NETWORK 0x06
#define BBS_TYPE_DEV              0x80
#define BBS_TYPE_UNKNOWN          0xff

typedef struct {
    EFI_DEVICE_PATH_PROTOCOL Header;
    UINT16 DeviceType;
    UINT16 StatusFlag;
    CHAR8  String[1];
} BBS_BBS_DEVICE_PATH;

//
// Device path helpers.
//

FORCEINLINE
UINT8
DevicePathType (
    IN CONST VOID *Node
    )

/*++

Routine Description:

    Gets the type of a device path node.

Arguments:

    Node - The node to get the type from.

Return Value:

    The type of the node.

--*/

{
    return ((EFI_DEVICE_PATH *)(Node))->Type & EFI_DP_TYPE_MASK;
}

FORCEINLINE
UINT8
DevicePathSubType (
    IN CONST VOID *Node
    )

/*++

Routine Description:

    Gets the subtype of a device path node.

Arguments:

    Node - The node to get the subtype from.

Return Value:

    The subtype of the node.

--*/

{
    return ((EFI_DEVICE_PATH *)(Node))->SubType;
}

FORCEINLINE
UINTN
DevicePathNodeLength (
    IN CONST VOID *Node
    )

/*++

Routine Description:

    Gets the length of a device path node.

Arguments:

    Node - The node to get the length from.

Return Value:

    The length of the node.

--*/

{
    return *((UINT16 *)&((EFI_DEVICE_PATH *)(Node))->Length[0]);
}

FORCEINLINE
EFI_DEVICE_PATH *
NextDevicePathNode (
    IN CONST VOID *Node
    )

/*++

Routine Description:

    Finds the next node in a device path.

Arguments:

    Node - The current node to find the next node from.

Return Value:

    A pointer to the next node.

--*/

{
    return (EFI_DEVICE_PATH *)((UINT8 *)(Node) + DevicePathNodeLength(Node));
}

FORCEINLINE
BOOLEAN
IsDevicePathEndType (
    IN CONST VOID *Node
    )

/*++

Routine Description:

    Checks if a device path node is an end node.

Arguments:

    Node - The node to check.

Return Value:

    TRUE if the node is an end node.

    FALSE if the node is not an end node.

--*/

{
    return (BOOLEAN)(DevicePathType(Node) == END_DEVICE_PATH_TYPE);
}

#endif /* !_EFIDEVP_H */
