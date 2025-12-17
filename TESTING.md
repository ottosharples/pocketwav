# Phase 2 Testing: USB Enumeration Reliability

## Goal
Verify the Pico enumerates as a USB Audio Class 1.0 device 100% reliably across different scenarios.

## Hardware Setup for Testing

**Required:**
- Pico with `usb_audio.uf2` flashed
- USB-C cable to Mac
- **USB-to-UART adapter** (for debug logging)
  - Connect adapter RX → Pico GPIO 0 (TX)
  - Connect adapter TX → Pico GPIO 1 (RX)  
  - Connect adapter GND → Pico GND

**Why UART?** USB is now the audio device, so we use UART for debug output.

## LED Status Codes

Watch the onboard LED for enumeration status:

| Blink Pattern | Status | Meaning |
|--------------|--------|---------|
| Fast (250ms) | Unmounted | Not enumerated, waiting for host |
| Slow (1s) | Mounted | Successfully enumerated! |
| Very slow (2.5s) | Suspended | Host suspended the device |

## Test 1: Initial Enumeration

### Steps:
1. Flash `usb_audio.uf2` to Pico
2. Connect UART adapter to your Mac
3. Open UART monitor: `screen /dev/tty.usbserial* 115200`
4. Plug Pico into Mac via USB

### Expected Results:
- LED changes from fast blink → slow blink
- UART shows:
  ```
  Pocketwav Phase 2: USB Enumeration
  Board ID: [hex]
  USB stack initialized. Waiting for enumeration...
  [USB] MOUNTED - Device enumerated successfully
  ```

### Verify in macOS:
1. Open **System Information** (⌘ + Space, type "System Information")
2. Go to **USB** section
3. Look for "Pocketwav Audio Device"
   - Vendor ID: 0xCAFE
   - Product ID: 0x4001
   - Manufacturer: Pocketwav

4. Open **Audio MIDI Setup** (Applications → Utilities)
5. Look for "Pocketwav Audio Device" in input devices

**Pass criteria:** Device appears in both locations with correct info.

## Test 2: Plug/Unplug Reliability (Critical)

### Steps:
1. With UART monitor running, unplug and replug the Pico **20 times**
2. Watch UART output for mount/unmount messages
3. Count failures (device doesn't mount or mounts inconsistently)

### Expected Results:
- Each unplug shows: `[USB] UNMOUNTED`
- Each replug shows: `[USB] MOUNTED`
- LED transitions: fast → slow on each plug
- **20/20 successful enumerations**

### If failures occur:
- Note the failure count
- Check for error messages in UART
- Try different USB port
- Try different cable

**Pass criteria:** 100% success rate (20/20)

## Test 3: Different Ports/Cables/Adapters

Test enumeration across:

- [ ] Different USB ports on Mac
- [ ] USB-C → USB-A adapter
- [ ] Different USB cables (try 3 if available)
- [ ] USB hub (if available)

For each configuration:
1. Plug in device
2. Verify LED goes to slow blink
3. Check UART for `MOUNTED` message
4. Verify device appears in System Information

**Pass criteria:** Device enumerates successfully in all configurations

## Test 4: Host Reboot

1. Leave Pico plugged in
2. Reboot your Mac
3. After boot, check System Information
4. Verify device re-enumerated

**Pass criteria:** Device appears after reboot without needing to replug

## Test 5: Sleep/Wake

1. With device plugged in and mounted
2. Put Mac to sleep (⌘ + Option + Power)
3. Wake Mac
4. Check UART logs
5. Verify device still mounted

### Expected Results:
- UART shows `[USB] SUSPENDED` when Mac sleeps
- UART shows `[USB] RESUMED` when Mac wakes
- LED transitions to very slow → back to slow blink
- Device remains functional

**Pass criteria:** Device survives sleep/wake cycle

## Test 6: iPhone Enumeration (If available)

If you have iPhone with USB-C:

1. Install a USB audio monitoring app (e.g., "Audio Devices" or similar)
2. Connect Pico to iPhone via USB-C
3. Check if device appears

**Note:** iOS may not show all device details, but should recognize it as an audio input.

**Pass criteria:** iPhone recognizes device (will test properly with app in Phase 3)

## Debug Output Examples

### Successful enumeration:
```
[USB] MOUNTED - Device enumerated successfully
[Status] USB: MOUNTED   | Suspended: NO  | Uptime: 5000 ms
[Status] USB: MOUNTED   | Suspended: NO  | Uptime: 10000 ms
```

### Unplug/replug cycle:
```
[USB] UNMOUNTED - Device disconnected
[USB] MOUNTED - Device enumerated successfully
```

### Suspend/resume:
```
[USB] SUSPENDED
[USB] RESUMED
```

## Troubleshooting

### LED stays in fast blink (never mounts)
- Check UART for error messages
- Verify USB cable supports data (not charge-only)
- Try different USB port
- Check TinyUSB configuration in `tusb_config.h`

### Intermittent mounting
- Cable quality issue - try different cable
- Power supply issue - try powered USB hub
- Check for UART error messages during failure

### No UART output
- Verify UART adapter connections (TX/RX might be swapped)
- Try reversing TX/RX connections
- Check baud rate is 115200
- Verify adapter GND is connected to Pico GND

### Device shows in System Info but not Audio MIDI Setup
- This is OK for now - we're just testing enumeration
- Audio MIDI Setup may not show devices without active streaming

## Acceptance Criteria

Phase 2 is complete when:

- ✅ Device enumerates successfully on first plug
- ✅ 20/20 plug/unplug cycles succeed
- ✅ Device enumerates on different ports/cables
- ✅ Device survives host reboot
- ✅ Device survives sleep/wake cycle
- ✅ Device appears in macOS System Information with correct details
- ✅ LED status indicators work correctly
- ✅ UART logging shows all state transitions

**Once all tests pass, Phase 2 is DONE and ready for Phase 3 (audio streaming).**

