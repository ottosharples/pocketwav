/**
 * USB Descriptors for Pocketwav UAC1 Device
 * 
 * Device appears as a USB Audio Class 1.0 microphone
 * Format: 16-bit PCM, 44.1 kHz, Stereo
 */

#include "tusb.h"

//--------------------------------------------------------------------
// Device Descriptors
//--------------------------------------------------------------------

tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,  // USB 2.0
    .bDeviceClass       = 0x00,    // Use class info from Interface Descriptors
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = 0xCAFE,  // Custom VID (would need real VID for production)
    .idProduct          = 0x4001,  // Pocketwav Audio Device
    .bcdDevice          = 0x0100,  // Device version 1.0
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const * tud_descriptor_device_cb(void) {
    return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------
// Configuration Descriptor - UAC1 Audio Device
//--------------------------------------------------------------------

#define ITF_NUM_AUDIO_CONTROL    0
#define ITF_NUM_AUDIO_STREAMING  1
#define ITF_COUNT                2

#define EPNUM_AUDIO_IN           0x81
#define AUDIO_SAMPLE_RATE        44100

// Descriptor length calculation
#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + CFG_TUD_AUDIO_FUNC_1_DESC_LEN)

uint8_t const desc_configuration[] = {
    // Configuration Descriptor
    TUD_CONFIG_DESCRIPTOR(1, ITF_COUNT, 0, CONFIG_TOTAL_LEN, 0x00, 100),

    // Interface Association Descriptor (IAD)
    TUD_AUDIO_DESC_IAD(
        /*_firstitfs*/ ITF_NUM_AUDIO_CONTROL,
        /*_nitfs*/ ITF_COUNT,
        /*_stridx*/ 0x00
    ),

    // Standard AC Interface Descriptor (4.7.1)
    TUD_AUDIO_DESC_STD_AC(
        /*_itfnum*/ ITF_NUM_AUDIO_CONTROL,
        /*_nEPs*/ 0x00,
        /*_stridx*/ 0x04
    ),

    // Class-Specific AC Interface Descriptor (4.7.2)
    TUD_AUDIO_DESC_CS_AC(
        /*_bcdADC*/ 0x0100,
        /*_category*/ AUDIO_FUNC_MICROPHONE,
        /*_totallen*/ TUD_AUDIO_DESC_CS_AC_LEN + TUD_AUDIO_DESC_INPUT_TERM_LEN + \
                      TUD_AUDIO_DESC_OUTPUT_TERM_LEN + TUD_AUDIO_DESC_FEATURE_UNIT_ONE_CHANNEL_LEN,
        /*_ctrl*/ AUDIO_CS_AS_INTERFACE_CTRL_LATENCY_POS
    ),

    // Input Terminal Descriptor (microphone input)
    TUD_AUDIO_DESC_INPUT_TERM(
        /*_termid*/ 0x01,
        /*_termtype*/ AUDIO_TERM_TYPE_IN_GENERIC_MIC,
        /*_assocTerm*/ 0x00,
        /*_clkid*/ 0x04,
        /*_nchannelslogical*/ 0x02,
        /*_channelcfg*/ AUDIO_CHANNEL_CONFIG_NON_PREDEFINED,
        /*_idxchannelnames*/ 0x00,
        /*_ctrl*/ 0 * (AUDIO_CTRL_R << AUDIO_IN_TERM_CTRL_CONNECTOR_POS),
        /*_stridx*/ 0x00
    ),

    // Feature Unit Descriptor
    TUD_AUDIO_DESC_FEATURE_UNIT_ONE_CHANNEL(
        /*_unitid*/ 0x02,
        /*_srcid*/ 0x01,
        /*_ctrlch0master*/ 0,
        /*_ctrlch1*/ 0,
        /*_stridx*/ 0x00
    ),

    // Output Terminal Descriptor
    TUD_AUDIO_DESC_OUTPUT_TERM(
        /*_termid*/ 0x03,
        /*_termtype*/ AUDIO_TERM_TYPE_USB_STREAMING,
        /*_assocTerm*/ 0x00,
        /*_srcid*/ 0x02,
        /*_clkid*/ 0x04,
        /*_ctrl*/ 0,
        /*_stridx*/ 0x00
    ),

    // Standard AS Interface Descriptor (Alt 0 - zero bandwidth)
    TUD_AUDIO_DESC_STD_AS_INT(
        /*_itfnum*/ ITF_NUM_AUDIO_STREAMING,
        /*_altset*/ 0x00,
        /*_nEPs*/ 0x00,
        /*_stridx*/ 0x05
    ),

    // Standard AS Interface Descriptor (Alt 1 - operational)
    TUD_AUDIO_DESC_STD_AS_INT(
        /*_itfnum*/ ITF_NUM_AUDIO_STREAMING,
        /*_altset*/ 0x01,
        /*_nEPs*/ 0x01,
        /*_stridx*/ 0x05
    ),

    // Class-Specific AS Interface Descriptor
    TUD_AUDIO_DESC_CS_AS_INT(
        /*_termid*/ 0x03,
        /*_ctrl*/ AUDIO_CTRL_NONE,
        /*_formattype*/ AUDIO_FORMAT_TYPE_I,
        /*_formats*/ AUDIO_DATA_FORMAT_TYPE_I_PCM,
        /*_nchannelsphysical*/ 2,
        /*_channelcfg*/ AUDIO_CHANNEL_CONFIG_NON_PREDEFINED,
        /*_stridx*/ 0x00
    ),

    // Type I Format Descriptor
    TUD_AUDIO_DESC_TYPE_I_FORMAT(
        /*_nBytesPerSample*/ CFG_TUD_AUDIO_FUNC_1_TX_ITEMSIZE,
        /*_nBitsUsedPerSample*/ 16
    ),

    // Standard AS Isochronous Audio Data Endpoint Descriptor
    TUD_AUDIO_DESC_STD_AS_ISO_EP(
        /*_ep*/ EPNUM_AUDIO_IN,
        /*_attr*/ (TUSB_XFER_ISOCHRONOUS | TUSB_ISO_EP_ATT_ADAPTIVE | TUSB_ISO_EP_ATT_DATA),
        /*_maxEPsize*/ TUD_AUDIO_EP_SIZE(AUDIO_SAMPLE_RATE, CFG_TUD_AUDIO_FUNC_1_TX_ITEMSIZE, CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX),
        /*_interval*/ 0x01
    ),

    // Class-Specific AS Isochronous Audio Data Endpoint Descriptor
    TUD_AUDIO_DESC_CS_AS_ISO_EP(
        /*_attr*/ AUDIO_CS_AS_ISO_DATA_EP_ATT_NON_MAX_PACKETS_OK,
        /*_ctrl*/ AUDIO_CTRL_NONE,
        /*_lockdelayunit*/ AUDIO_CS_AS_ISO_DATA_EP_LOCK_DELAY_UNIT_UNDEFINED,
        /*_lockdelay*/ 0x0000
    )
};

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    (void) index; // for multiple configurations
    return desc_configuration;
}

//--------------------------------------------------------------------
// String Descriptors
//--------------------------------------------------------------------

// Array of pointer to string descriptors
char const* string_desc_arr[] = {
    (const char[]) { 0x09, 0x04 },  // 0: Language (English)
    "Pocketwav",                     // 1: Manufacturer
    "Pocketwav Audio Device",        // 2: Product
    "123456",                        // 3: Serial (could use pico unique ID)
    "Pocketwav Audio Control",       // 4: Audio Control Interface
    "Pocketwav Audio Stream",        // 5: Audio Streaming Interface
};

static uint16_t _desc_str[32];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void) langid;
    
    uint8_t chr_count;
    
    if (index == 0) {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    } else {
        // Convert ASCII string into UTF-16
        if (!(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0]))) {
            return NULL;
        }
        
        const char* str = string_desc_arr[index];
        
        // Cap at max char
        chr_count = strlen(str);
        if (chr_count > 31) chr_count = 31;
        
        for (uint8_t i = 0; i < chr_count; i++) {
            _desc_str[1+i] = str[i];
        }
    }
    
    // first byte is length (including header), second byte is string type
    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2*chr_count + 2);
    
    return _desc_str;
}
