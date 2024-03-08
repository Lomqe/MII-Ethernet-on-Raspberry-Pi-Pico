#pragma once

#define KSZ_RST 2u
#define KSZ_CRS_OR_CONFIG1 4u
#define KSZ_COL_OR_CONFIG0 5u
#define KSZ_INTRP_OR_NAND_TREE 6u
#define KSZ_RXDV_OR_CONFIG2 7u
#define KSZ_RXER_OR_ISO 8u
#define KSZ_TXEN 9u
#define KSZ_TXD3 10u
#define KSZ_TXD2 11u
#define KSZ_TXD1 12u
#define KSZ_TXD0 13u
#define KSZ_TXC 14u
#define KSZ_RXC_OR_B_CAST_OFF 15u
#define KSZ_RXD0_OR_DUPLEX 16u
#define KSZ_RXD1_OR_PHYAD2 17u
#define KSZ_RXD2_OR_PHYAD1 18u
#define KSZ_RXD3_OR_PHYDA0 19u
#define KSZ_MDC 20u
#define KSZ_MDIO 21u

#define RPP_SENSOR1 26u
#define RPP_SENSOR2 27u

#define SYSTEM_FREQ_125000_KHZ 125000u
#define TX_FRAME_SIZE 1024u


// The default PHY address is 00001 and may vary from 1-7 (0 = broadcast)
uint8_t phy_address = 1u;

uint8_t preamble[] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xD5};

#define NCAT_PACKET_LENGTH 69u
uint8_t netcat_packet[] = {
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

uint8_t crc[4];
static const uint32_t ethernet_polynom = 0xEDB88320U;

static uint8_t tx_frame[TX_FRAME_SIZE];
static uint8_t tx_frame_bits[TX_FRAME_SIZE * 2u];

uint tx_dma;
dma_channel_config tx_dma_config;

uint sm_tx;