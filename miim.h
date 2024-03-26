#pragma once

/// @brief
/// @param bit 
static void mii_mdio_out(bool bit);

/// @brief 
/// @return 
static bool mii_mdio_in();

/// @brief
/// @param addr 
/// @param reg 
/// @param val 
static void mii_mdio_write(uint addr, uint reg, int val);

/// @brief
/// @param addr 
/// @param reg 
/// @return 
static uint16_t mii_mdio_read(uint addr, uint reg);

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

static void mii_mdio_out(bool bit){
    gpio_put(KSZ_MDC, 0);
    gpio_put(KSZ_MDIO, bit);
    gpio_put(KSZ_MDC, 1);
}

static bool mii_mdio_in(){
    gpio_put(KSZ_MDC, 0);
    gpio_put(KSZ_MDC, 1);
    
    bool bit = gpio_get(KSZ_MDIO);
    return bit;
}

static void mii_mdio_write(uint addr, uint reg, int val){
    gpio_init(KSZ_MDIO);
    gpio_init(KSZ_MDC);

    gpio_set_dir(KSZ_MDIO, GPIO_OUT);
    gpio_set_dir(KSZ_MDC, GPIO_OUT);

    // PRE_32
    for (int i = 0; i < 32; ++i){
        mii_mdio_out(1);
    }

    // ST
    mii_mdio_out(0);
    mii_mdio_out(1);

    // OP
    mii_mdio_out(0);
    mii_mdio_out(1);

    // PHY
    for (int i = 0; i < 5; ++i){
        bool bit = (addr >> (4 - i)) & 0x1;
        mii_mdio_out(bit);
    }

    // REG
    for (int i = 0; i < 5; ++i){
        bool bit = (reg >> (4 - i)) & 0x1;
        mii_mdio_out(bit);
    }

    // TA
    mii_mdio_out(1);
    mii_mdio_out(0);

    // DATA
    for (int i = 0; i < 16; ++i){
        uint bit = (val >> (15 - i)) & 0x1;
        mii_mdio_out(bit);
    }

    gpio_set_dir(KSZ_MDIO, GPIO_IN);
}

static uint16_t mii_mdio_read(uint addr, uint reg){
    gpio_init(KSZ_MDIO);
    gpio_init(KSZ_MDC);

    gpio_set_dir(KSZ_MDIO, GPIO_OUT);
    gpio_set_dir(KSZ_MDC, GPIO_OUT);

    // PRE_32
    for (int i = 0; i < 32; ++i){
        mii_mdio_out(1);
    }

    // ST
    mii_mdio_out(0);
    mii_mdio_out(1);

    // OP
    mii_mdio_out(1);
    mii_mdio_out(0);

    // PHY
    for (int i = 0; i < 5; ++i){
        uint bit = (addr >> (4 - i)) & 0x1;

        mii_mdio_out(bit);
    }

    // REG
    for (int i = 0; i < 5; ++i) {
        uint bit = (reg >> (4 - i)) & 0x1;

        mii_mdio_out(bit);
    }

    // TA
    gpio_set_dir(KSZ_MDIO, GPIO_IN);
    mii_mdio_out(0);
    mii_mdio_out(0);

    uint16_t data = 0;

    for (int i = 0; i < 16; ++i){
        data <<= 1;
        data |= mii_mdio_in();
    }

    return data;
}