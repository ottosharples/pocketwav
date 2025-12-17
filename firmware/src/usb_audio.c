/**
 * Pocketwav - Phase 2: USB Audio Enumeration
 * 
 * Tests USB Audio Class 1.0 enumeration reliability
 * Uses LED patterns and UART logging for status
 */

#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/unique_id.h"
#include "hardware/clocks.h"
#include "tusb.h"

//--------------------------------------------------------------------
// HARDWARE CONFIGURATION
//--------------------------------------------------------------------

#define LED_PIN 25

// LED blink patterns (intervals in ms)
#define LED_UNMOUNTED_BLINK   250   // Fast blink = not mounted
#define LED_MOUNTED_BLINK     1000  // Slow blink = mounted
#define LED_SUSPENDED_BLINK   2500  // Very slow = suspended

//--------------------------------------------------------------------
// STATE TRACKING
//--------------------------------------------------------------------

static uint32_t blink_interval_ms = LED_UNMOUNTED_BLINK;
static bool usb_mounted = false;
static bool usb_suspended = false;

//--------------------------------------------------------------------
// LED CONTROL
//--------------------------------------------------------------------

void led_init(void) {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void led_task(void) {
    static uint32_t start_ms = 0;
    static bool led_state = false;
    
    uint32_t now = to_ms_since_boot(get_absolute_time());
    
    // Blink every interval ms
    if (now - start_ms < blink_interval_ms) return;
    start_ms += blink_interval_ms;
    
    gpio_put(LED_PIN, led_state);
    led_state = !led_state;
}

//--------------------------------------------------------------------
// USB DEVICE CALLBACKS
//--------------------------------------------------------------------

// Invoked when device is mounted
void tud_mount_cb(void) {
    usb_mounted = true;
    blink_interval_ms = LED_MOUNTED_BLINK;
    printf("[USB] MOUNTED - Device enumerated successfully\n");
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
    usb_mounted = false;
    blink_interval_ms = LED_UNMOUNTED_BLINK;
    printf("[USB] UNMOUNTED - Device disconnected\n");
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
    usb_suspended = true;
    blink_interval_ms = LED_SUSPENDED_BLINK;
    printf("[USB] SUSPENDED\n");
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
    usb_suspended = false;
    blink_interval_ms = usb_mounted ? LED_MOUNTED_BLINK : LED_UNMOUNTED_BLINK;
    printf("[USB] RESUMED\n");
}

//--------------------------------------------------------------------
// AUDIO CALLBACKS (Minimal for enumeration phase)
//--------------------------------------------------------------------

// Invoked when audio class specific set request received for an EP
bool tud_audio_set_req_ep_cb(uint8_t rhport, tusb_control_request_t const * p_request, uint8_t *pBuff) {
    (void) rhport;
    (void) p_request;
    (void) pBuff;
    return true;
}

// Invoked when audio class specific set request received for an interface
bool tud_audio_set_req_itf_cb(uint8_t rhport, tusb_control_request_t const * p_request, uint8_t *pBuff) {
    (void) rhport;
    (void) p_request;
    (void) pBuff;
    return true;
}

// Invoked when audio class specific get request received for an EP
bool tud_audio_get_req_ep_cb(uint8_t rhport, tusb_control_request_t const * p_request) {
    (void) rhport;
    (void) p_request;
    return true;
}

// Invoked when audio class specific get request received for an interface
bool tud_audio_get_req_itf_cb(uint8_t rhport, tusb_control_request_t const * p_request) {
    (void) rhport;
    (void) p_request;
    return true;
}

//--------------------------------------------------------------------
// MAIN
//--------------------------------------------------------------------

int main(void) {
    // Initialize hardware (use default 125 MHz clock)
    led_init();
    
    // Initialize UART for debug logging
    stdio_init_all();
    
    // Small delay for UART to settle
    sleep_ms(100);
    
    // Print startup banner
    printf("\n");
    printf("========================================\n");
    printf("  Pocketwav Phase 2: USB Enumeration\n");
    printf("========================================\n");
    
    // Print board ID
    pico_unique_board_id_t board_id;
    pico_get_unique_board_id(&board_id);
    printf("Board ID: ");
    for (int i = 0; i < PICO_UNIQUE_BOARD_ID_SIZE_BYTES; i++) {
        printf("%02X", board_id.id[i]);
    }
    printf("\n");
    
    printf("\nInitializing USB Audio Device...\n");
    printf("Format: 16-bit PCM, 44.1 kHz, Stereo\n");
    printf("\nLED Status:\n");
    printf("  Fast blink (250ms)  = Unmounted\n");
    printf("  Slow blink (1000ms) = Mounted\n");
    printf("  Very slow (2500ms)  = Suspended\n");
    printf("\n");
    
    // Initialize TinyUSB
    tusb_init();
    
    printf("USB stack initialized. Waiting for enumeration...\n\n");
    
    uint32_t status_report_ms = 0;
    
    // Main loop
    while (true) {
        // TinyUSB device task
        tud_task();
        
        // LED blink task
        led_task();
        
        // Periodic status report (every 5 seconds)
        uint32_t now = to_ms_since_boot(get_absolute_time());
        if (now - status_report_ms > 5000) {
            status_report_ms = now;
            
            printf("[Status] USB: %s | Suspended: %s | Uptime: %lu ms\n",
                   usb_mounted ? "MOUNTED  " : "UNMOUNTED",
                   usb_suspended ? "YES" : "NO ",
                   now);
        }
    }
    
    return 0;
}

