#pragma once
#include "hardware/dma.h"

#define KSZ_BASIC_CONTROL_REG _u(0)    // Register 0h – Basic Control
#define KSZ_BASIC_STATUS_REG _u(1)     // Register 1h - Basic Status

#define KSZ_RST _u(2)
#define KSZ_CRS_OR_CONFIG1 _u(4)
#define KSZ_COL_OR_CONFIG0 _u(5)
#define KSZ_INTRP_OR_NAND_TREE _u(6)
#define KSZ_RXDV_OR_CONFIG2 _u(7)
#define KSZ_RXER_OR_ISO _u(8)
#define KSZ_TXEN _u(9)
#define KSZ_TXD3 _u(10)
#define KSZ_TXD2 _u(11)
#define KSZ_TXD1 _u(12)
#define KSZ_TXD0 _u(13)
#define KSZ_TXC _u(14)
#define KSZ_RXC_OR_B_CAST_OFF _u(15)
#define KSZ_RXD0_OR_DUPLEX _u(16)
#define KSZ_RXD1_OR_PHYAD2 _u(17)
#define KSZ_RXD2_OR_PHYAD1 _u(18)
#define KSZ_RXD3_OR_PHYDA0 _u(19)
#define KSZ_MDC _u(20)
#define KSZ_MDIO _u(21)

#define RPP_SENSOR1 _u(26)     // 10 - 80 cm (Effective Range)
#define RPP_SENSOR2 _u(27)     // 20 - 150 cm (Effective Range)

#define VCO_FREQ_1500KHZ _u(1500000000)
#define SYSTEM_FREQ_125000_KHZ _u(125000)
#define SYSTEM_FREQ_100000_KHZ _u(100000)

#define TX_FRAME_SIZE _u(1492)


// Default packet with sensors data
static uint8_t ethernet_packet[] = {
    //  Ethernet II
    0xC8, 0x7F, 0x54, 0xA5, 0x83, 0x5D,     // Destination MAC                        
    0xC8, 0x7F, 0x54, 0x69, 0x8A, 0xDA,     // Source MAC
    0x08, 0x00,                             // Type (IPv4)
    //  IP
    0x45, 0x00,                             // Header Length, Diff. Services Field
    0x00, 0x58,                             // Total Length
    0xA2, 0xD8,                             // Identification
    0x00, 0x00,                             // Fragment Offset
    0x80,                                   // Time To Live
    0x11,                                   // Protocol (UDP)
    0x13, 0xB8,                             // Header checksum
    0xC0, 0xA8, 0x01, 0x82,                 // Source Address
    0xC0, 0xA8, 0x01, 0x32,                 // Destination Address
    //  UDP
    0xD5, 0x2A,                             // Source Port
    0x04, 0xD2,                             // Destination Port
    0x00, 0x44,                             // Length 
    0x00, 0x00,                             // Checksum                     
    0x50, 0x61, 0x63, 0x6B, 0x65, 0x74, 0x20, 0x6E, 0x75, 0x6D, 0x62, 0x65, 0x72, 0x3A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x09,
    0x53, 0x65, 0x6E, 0x7A, 0x6F, 0x72, 0x5B, 0x31, 0x5D, 0x20, 0x3D, 0x20, 0x20, 0x20, 0x20, 0x20, 0x63, 0x6D, 0x09,
    0x53, 0x65, 0x6E, 0x7A, 0x6F, 0x72, 0x5B, 0x32, 0x5D, 0x20, 0x3D, 0x20, 0x20, 0x20, 0x20, 0x20, 0x63, 0x6D, 0x0A
    };

// Optimalized packet for 32bit DMA
static uint32_t ethernet_packet2[] = {
    0xC87F54A5, 0x835DC87F, 0x54698ADA,     
    0x08004500, 0x0042A2D8, 0x00008011,                                   
    0x13D9C0A8, 0x0182C0A8, 0x0132D52A,                             
    0x04D2002E, 0xD4F35365, 0x6E7A6F72, 0x5B315D20, 
    0x3D202020, 0x2020636D, 0x0953656E, 0x7A6F725B,
    0x325D203D, 0x20202020, 0x20636D0A
    };

// Test packet
const static uint8_t netcat_packet[] = {
    //  Ethernet II
    0xC8, 0x7F, 0x54, 0xA5, 0x83, 0x5D,     // Destination MAC                        
    0xC8, 0x7F, 0x54, 0x69, 0x8A, 0xDA,     // Source MAC
    0x08, 0x00,                             // Type (IPv4)
    //  IP
    0x45, 0x00,                             // Header Length, Diff. Services Field
    0x00, 0x37,                             // Total Length
    0xA2, 0xD8,                             // Identification
    0x00, 0x00,                             // Fragment Offset
    0x80,                                   // Time To Live
    0x11,                                   // Protocol (UDP)
    0x13, 0xD9,                             // Header checksum
    0xC0, 0xA8, 0x01, 0x82,                 // Source Address
    0xC0, 0xA8, 0x01, 0x32,                 // Destination Address
    //  UDP
    0xD5, 0x2A,                             // Source Port
    0x04, 0xD2,                             // Destination Port
    0x00, 0x23,                             // Length
    0xD4, 0xF3,                             // Checksum
    0x54, 0x65, 0x73, 0x74, 0x6F, 0x76, 0x61, 0x63, 0x69, 0x20, 0x7A, 0x70, 0x72,       // Data
    0x61, 0x76, 0x61, 0x20, 0x7A, 0x20, 0x4E, 0x65, 0x74, 0x43, 0x61, 0x74, 0x0D, 0x0A  // Data
    };

static uint8_t tx_frame[TX_FRAME_SIZE];
static uint8_t tx_frame_bits[TX_FRAME_SIZE * 2u];
static uint32_t tx_frame_32b[TX_FRAME_SIZE/4];

static uint sm_tx = 0;
static uint tx_dma;
static dma_channel_config tx_dma_config;