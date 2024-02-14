#pragma once

// 67 BYTE LENGTH
uint8_t test_packet[] = {
    //  Ethernet II
    0xC8, 0x7F, 0x54, 0xA5, 0x83, 0x5D,     // Destination MAC                        
    0x58, 0x11, 0x22, 0x20, 0x74, 0x28,     // Source MAC
    0x08, 0x00,                             // Type (IPv4)
    //  IP
    0x45, 0x00,                             // Header Length, Diff. Services Field
    0x00, 0x35,                             // Total Length
    0x02, 0xFD,                             // Identification
    0x00, 0x00,                             // Fragment Offset
    0x74,                                   // Time To Live
    0x11,                                   // Protocol (UDP)
    0x43, 0x6B,                             // Header checksum
    0x5F, 0xA8, 0xDE, 0xCD,                 // Source Address
    0xC0, 0xA8, 0x01, 0x32,                 // Destination Address
    //  UDP
    0x01, 0xBB,                             // Source Port
    0xE8, 0x82,                             // Destination Port
    0x00, 0x21,                             // Length
    0x37, 0x7C,                             // Checksum
    0x58, 0x0E, 0xF5, 0x70, 0xAC, 0x33, 0x47, 0x42, 0x69, 0x1B, 0xB7, 0x2E,         // DATA
    0x91, 0xB8, 0x78, 0x8F, 0xA2, 0x67, 0x99, 0xF1, 0x32, 0xF1, 0x6A, 0xCF, 0x98    // DATA
    };

#define SYSTEM_FREQ_100000_KHZ 100000u

// GPIO2 -> KSZ_RESET
#define RST_KSZ 2u