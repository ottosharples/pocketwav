add_executable(blink
    blink.c
)

# Pull in pico libraries
target_link_libraries(blink
    pico_stdlib
    pico_unique_id
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(blink 1)
pico_enable_stdio_uart(blink 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(blink)

