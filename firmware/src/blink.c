/**
 * Pocketwav - Phase 1 Sanity Test
 * Simple blink program with USB stdio enabled
 * Verifies toolchain, flashing, and USB functionality
 */

#include "pico/stdlib.h"
#include "pico/unique_id.h"
#include <stdio.h>

#define LED_PIN 25
#define BLINK_DELAY_MS 500

int main() {
    // Initialize standard I/O (USB)
    stdio_init_all()
    
    // Initialize LED pin
    gpio_init(LED_PIN)
    gpio_set_dir(LED_PIN, GPIO_OUT)
    
    // Get and print unique board ID
    pico_unique_board_id_t board_id
    pico_get_unique_board_id(&board_id)
    
    printf("Pocketwav Phase 1 - Sanity Check\n")
    printf("Pico Board ID: ")
    for (int i = 0; i < PICO_UNIQUE_BOARD_ID_SIZE_BYTES; i++) {
        printf("%02X", board_id.id[i])
    }
    printf("\n")
    
    uint32_t count = 0
    
    while (true) {
        gpio_put(LED_PIN, 1)
        printf("LED ON  [%lu]\n", count)
        sleep_ms(BLINK_DELAY_MS)
        
        gpio_put(LED_PIN, 0)
        printf("LED OFF [%lu]\n", count)
        sleep_ms(BLINK_DELAY_MS)
        
        count++
    }
    
    return 0
}

