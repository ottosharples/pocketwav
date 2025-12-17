/**
 * TinyUSB Configuration for Pocketwav USB Audio Device
 */

#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
// COMMON CONFIGURATION
//--------------------------------------------------------------------

// defined by compiler flags for flexibility
#ifndef CFG_TUSB_MCU
#define CFG_TUSB_MCU OPT_MCU_RP2040
#endif

#ifndef CFG_TUSB_OS
#define CFG_TUSB_OS OPT_OS_NONE
#endif

#ifndef CFG_TUSB_DEBUG
#define CFG_TUSB_DEBUG 0
#endif

// Enable Device stack
#define CFG_TUD_ENABLED 1

// RHPort number used for device
#define BOARD_TUD_RHPORT 0

// RHPort max operational speed
#define BOARD_TUD_MAX_SPEED OPT_MODE_FULL_SPEED

// RHPort mode - device only
#define CFG_TUSB_RHPORT0_MODE   (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)
#define CFG_TUSB_RHPORT1_MODE   0

//--------------------------------------------------------------------
// DEVICE CONFIGURATION
//--------------------------------------------------------------------

#ifndef CFG_TUD_ENDPOINT0_SIZE
#define CFG_TUD_ENDPOINT0_SIZE 64
#endif

//------------- CLASS DRIVERS -------------//

// Enable Audio Class
#define CFG_TUD_AUDIO 1

// Disable other classes for now
#define CFG_TUD_CDC 0
#define CFG_TUD_MSC 0
#define CFG_TUD_HID 0
#define CFG_TUD_MIDI 0
#define CFG_TUD_VENDOR 0

//--------------------------------------------------------------------
// AUDIO CLASS DRIVER CONFIGURATION
//--------------------------------------------------------------------

// Audio format type (16-bit PCM)
#define CFG_TUD_AUDIO_FUNC_1_FORMAT_TYPE AUDIO_FORMAT_TYPE_I

// Number of channels (stereo = 2)
#define CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX 2

// Audio sample width (16-bit)
#define CFG_TUD_AUDIO_FUNC_1_TX_ITEMSIZE 2

// Sample rate (44.1 kHz)
#define CFG_TUD_AUDIO_FUNC_1_SAMPLE_RATE 44100

// EP buffer size (enough for multiple frames)
#define CFG_TUD_AUDIO_FUNC_1_EP_IN_SW_BUF_SZ (CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX * \
                                               CFG_TUD_AUDIO_FUNC_1_TX_ITEMSIZE * \
                                               48)  // ~1ms at 48kHz (works for 44.1)

// Control buffer size
#define CFG_TUD_AUDIO_FUNC_1_CTRL_BUF_SZ 64

// Number of Standard AS Interface Descriptors (we have 2: alt 0 and alt 1)
#define CFG_TUD_AUDIO_FUNC_1_N_AS_INT 1

// Audio descriptor length
#define CFG_TUD_AUDIO_FUNC_1_DESC_LEN (TUD_AUDIO_DESC_IAD_LEN + \
                                        TUD_AUDIO_DESC_STD_AC_LEN + TUD_AUDIO_DESC_CS_AC_LEN + \
                                        TUD_AUDIO_DESC_INPUT_TERM_LEN + TUD_AUDIO_DESC_OUTPUT_TERM_LEN + \
                                        TUD_AUDIO_DESC_FEATURE_UNIT_ONE_CHANNEL_LEN + \
                                        TUD_AUDIO_DESC_STD_AS_INT_LEN + TUD_AUDIO_DESC_STD_AS_INT_LEN + \
                                        TUD_AUDIO_DESC_CS_AS_INT_LEN + TUD_AUDIO_DESC_TYPE_I_FORMAT_LEN + \
                                        TUD_AUDIO_DESC_STD_AS_ISO_EP_LEN + TUD_AUDIO_DESC_CS_AS_ISO_EP_LEN)

// Number of bytes per audio sample (2 for 16-bit)
#define CFG_TUD_AUDIO_FUNC_1_N_BYTES_PER_SAMPLE_TX CFG_TUD_AUDIO_FUNC_1_TX_ITEMSIZE

// Enable EP IN flow control
#define CFG_TUD_AUDIO_ENABLE_EP_IN 1

// EP IN size - calculate for 44.1kHz, stereo, 16-bit
// Formula: (sample_rate / 1000 + 1) * channels * bytes_per_sample
#define CFG_TUD_AUDIO_FUNC_1_EP_IN_SZ_MAX ((44100 / 1000 + 1) * CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX * CFG_TUD_AUDIO_FUNC_1_TX_ITEMSIZE)

// Biggest EP IN size across all functions
#define CFG_TUD_AUDIO_EP_SZ_IN CFG_TUD_AUDIO_FUNC_1_EP_IN_SZ_MAX

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */

