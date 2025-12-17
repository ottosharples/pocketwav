add_executable(usb_audio
    usb_audio.c
    usb_descriptors.c
)

# Add current directory to include path for tusb_config.h
target_include_directories(usb_audio PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Pull in pico libraries
target_link_libraries(usb_audio
    pico_stdlib
    pico_unique_id
    tinyusb_device
    tinyusb_board
)

# Enable UART output for debugging (USB will be audio device)
pico_enable_stdio_usb(usb_audio 0)
pico_enable_stdio_uart(usb_audio 1)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(usb_audio)

# Keep blink for reference
add_executable(blink
    blink.c
)

target_link_libraries(blink
    pico_stdlib
    pico_unique_id
)

pico_enable_stdio_usb(blink 1)
pico_enable_stdio_uart(blink 0)
pico_add_extra_outputs(blink)

