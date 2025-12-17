# Pocketwav

USB Audio Class (UAC) input device for streaming lossless digital audio to iPhone over USB-C.

## Overview

Pocketwav is a USB audio input device built on the Raspberry Pi Pico (RP2040). It reads audio files from a microSD card and streams PCM audio over USB, appearing to the host (iPhone/Mac) as an external audio input device (like a USB microphone).

**Phase 1 Goal:** Enumerate as a stable USB Audio Class 1.0 device streaming silence/test tone.

## Hardware Components

- Raspberry Pi Pico (RP2040)
- microSD card + SPI breakout module
- USB-C female breakout (with 5.1kΩ pull-downs on CC1/CC2)
- 3× momentary push buttons (Play/Pause, Next, Previous)
- Breadboard + jumper wires

## Firmware Build Instructions

### Prerequisites

1. **Install Pico SDK**
   ```bash
   cd ~/
   git clone https://github.com/raspberrypi/pico-sdk.git
   cd pico-sdk
   git submodule update --init
   ```

2. **Install ARM toolchain**
   ```bash
   # macOS
   brew install cmake
   brew tap ArmMbed/homebrew-formulae
   brew install arm-none-eabi-gcc
   ```

3. **Set environment variable**
   ```bash
   export PICO_SDK_PATH=~/pico-sdk
   ```
   Add this to your `~/.zshrc` to make it permanent.

### Build

```bash
cd firmware
mkdir build
cd build
cmake ..
make
```

This produces `src/blink.uf2` — the flashable firmware image.

### Flash to Pico

1. Hold the BOOTSEL button on the Pico
2. Plug the Pico into your Mac via USB
3. Release BOOTSEL — the Pico appears as a USB drive (RPI-RP2)
4. Copy the `.uf2` file:
   ```bash
   cp src/blink.uf2 /Volumes/RPI-RP2/
   ```
5. The Pico automatically reboots and runs the firmware

### Verify

- **LED:** The onboard LED should blink every 500ms
- **USB Serial:** Connect via serial monitor to see debug output:
  ```bash
  screen /dev/tty.usbmodem* 115200
  ```
  You should see "Pocketwav Phase 1 - Sanity Check" and blink count messages.

## Development Phases

### Phase 1: USB Audio Enumeration (Current)
- [x] Repo + toolchain setup
- [x] Blink sanity test with USB stdio
- [ ] TinyUSB UAC1 descriptors
- [ ] Stream silence/test tone
- [ ] Validate on Mac + iPhone

### Phase 2: SD Card Integration
- [ ] Mount FAT32 filesystem
- [ ] Read WAV files (16-bit, 44.1kHz stereo)
- [ ] Integrate into audio pipeline

### Phase 3: User Controls
- [ ] Play/Pause button
- [ ] Next/Previous track buttons
- [ ] Basic state machine

## Technical Specs

- **Audio Format:** 16-bit PCM, 44.1 kHz, stereo
- **USB:** Audio Class 1.0, device mode (UFP)
- **Storage:** microSD (FAT32)
- **Platform:** RP2040 @ 125 MHz

## License

MIT

