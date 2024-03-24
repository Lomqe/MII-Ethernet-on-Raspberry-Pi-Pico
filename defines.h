#pragma once

#define KSZ_BASIC_CONTROL_REG 0u    // Register 0h – Basic Control
#define KSZ_BASIC_STATUS_REG 1u     // Register 1h - Basic Status

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

#define RPP_SENSOR1 26u     // 10 - 80 cm (Effective Range)
#define RPP_SENSOR2 27u     // 20 - 150 cm (Effective Range)

#define VCO_FREQ_1500KHZ 1500000000u
#define SYSTEM_FREQ_125000_KHZ 125000u
#define SYSTEM_FREQ_100000_KHZ 100000u

#define TX_FRAME_SIZE 1492u


// The default PHY address is 00001 and may vary from 1-7 (0 = broadcast)
uint8_t phy_address = 1u;

uint8_t preamble[] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xD5};

uint8_t ethernet_packet[] = {
    //  Ethernet II
    0xC8, 0x7F, 0x54, 0xA5, 0x83, 0x5D,     // Destination MAC                        
    0xC8, 0x7F, 0x54, 0x69, 0x8A, 0xDA,     // Source MAC
    0x08, 0x00,                             // Type (IPv4)
    //  IP
    0x45, 0x00,                             // Header Length, Diff. Services Field
    0x00, 0x42,                             // Total Length
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
    0x00, 0x2E,                             // Length 
    0xD4, 0xF3,                             // Checksum                     ----- 3 byte----
    0x53, 0x65, 0x6E, 0x7A, 0x6F, 0x72, 0x5B, 0x31, 0x5D, 0x20, 0x3D, 0x20, 0x20, 0x20, 0x20, 0x20, 0x63, 0x6D, 0x0A,
    0x53, 0x65, 0x6E, 0x7A, 0x6F, 0x72, 0x5B, 0x32, 0x5D, 0x20, 0x3D, 0x20, 0x20, 0x20, 0x20, 0x20, 0x63, 0x6D, 0x0A
    };

// optimalizovany packet pro PIO opt2.
uint32_t ethernet_packet2[] = {
    0xC87F54A5, 0x835DC87F, 0x54698ADA,     
    0x08004500, 0x0042A2D8, 0x00008011,                                   
    0x13D9C0A8, 0x0182C0A8, 0x0132D52A,                             
    0x04D2002E, 0xD4F35365, 0x6E7A6F72, 0x5B315D20, 
    0x3D202020, 0x2020636D, 0x0A53656E, 0x7A6F725B,
    0x325D203D, 0x20202020, 0x20636D0A
    };

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

static uint32_t tx_frame_32b[TX_FRAME_SIZE/4];

uint tx_dma;
dma_channel_config tx_dma_config;

uint sm_tx = 0;

// FLAG to pace both cores
volatile bool sampling = true;  

#define ADC_PIN_LEVEL 3.3   // ADC max value in Volts
#define ADC_12B_LEVEL 4095  // ADC max 12 bit value

struct sensor{
    uint adc_input;         // Input to ADC 
    uint8_t size;           // Size of the buffers
    float sample_val;       // Last sampled valued by ADC

    uint8_t* lut_cm;        // Pointer to buffer with datasheet values
    float* lut_volt;        // Pointer to buffer with datasheet values

    float max_val;
    float min_val;
}sensor1, sensor2;

#define SENSOR1_MAX_VAL sensor1_lut_volt[0]
#define SENSOR1_MIN_VAL sensor1_lut_volt[18]
// Sensors 10-80 prox. LUT from datasheet 
float sensor1_lut_volt[] = {2.60, 2.45, 2.11, 1.87, 1.65, 1.51, 1.39, 1.13, 0.98, 0.85, 0.75, 0.68, 0.61, 0.58, 0.52, 0.50, 0.47, 0.43, 0.41};
uint8_t sensor1_lut_cm[] = {10, 10, 12, 14, 16, 18, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80};

#define SENSOR2_MAX_VAL sensor2_lut_volt[0]
#define SENSOR2_MIN_VAL sensor2_lut_volt[13]
// Sensors 20-150 prox. LUT from datasheet    1551  1303
float sensor2_lut_volt[] = {2.55, 1.99, 1.52, 1.25, 1.05, 0.90, 0.80, 0.73, 0.67, 0.60, 0.55, 0.50, 0.49, 0.48};
uint8_t sensor2_lut_cm[] = {20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150};

  bool first_transm = 1;
    