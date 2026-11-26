# Wiring ESP32 To A Cheap Walkie Talkie

This firmware is designed for cheap handheld radios that expose microphone,
ground, and push-to-talk wiring through an accessory connector. Many low-cost
radios use a Kenwood-style two-pin connector, but pinouts still vary. Confirm
the radio pinout before wiring.

## Recommended Interface

Use isolation or transistor switching. Do not connect an ESP32 GPIO directly to
a radio PTT line until you know the radio voltage and current.

```text
ESP32 GPIO 25 ---- resistor ---- NPN base
NPN emitter -------------------- radio/accessory ground
NPN collector ------------------ radio PTT line

ESP32 GPIO 26 ---- RC filter ---- coupling capacitor ---- level pot ---- radio mic input
ESP32 GND ----------------------------------------------- radio/accessory ground
```

For better protection, replace the NPN PTT switch with an optocoupler. This is
especially useful if the radio accessory ground is noisy or if the radio is
powered from a different battery.

## Audio Level

Start with the ESP32 audio level very low. Handheld microphone inputs are
sensitive and will over-deviate easily.

Suggested starting audio chain:

```text
GPIO 26 -> 1 k resistor -> 100 nF capacitor to ground -> 1 uF coupling capacitor -> 10 k trimpot -> mic input
```

Set the radio volume low while testing. Listen on a second receiver and adjust
the trimpot until the CW and tone are clean but not distorted.

## PTT Notes

Most simple handhelds transmit when the radio PTT line is shorted to radio
ground. With the recommended NPN transistor wiring above, the ESP32 GPIO goes
HIGH, the transistor turns on, and the radio PTT line is pulled to ground. That
means the firmware should use `active_high`, which is the default.

Use this command for the recommended NPN/MOSFET/opto driver:

```text
set ptt active_high
```

Use `active_low` only if your interface circuit keys the radio when the ESP32
GPIO is LOW.

Before connecting a radio, run:

```text
ptt_test
```

Check the PTT output with an LED or multimeter. It should turn on for about 1.2
seconds, then turn off.

## Default Firmware Pins

| Function | ESP32 GPIO |
| --- | ---: |
| PTT control | 25 |
| Audio tone | 26 |
| Status LED | 2 |
| Test button | 0 |
| Battery ADC | 34 |

ESP32-S3 and ESP32-C3 environments override some pins in `platformio.ini`
because those boards expose different ADC and boot-safe pins.

## First Test

1. Upload the firmware.
2. Connect only an LED or multimeter to the PTT output first.
3. Open the serial monitor at 115200 baud.
4. Type `ptt_test`.
5. Confirm that PTT switching works and releases reliably.
6. Connect the PTT interface to the radio.
7. Type `test` and confirm that audio output works.
8. Test into a dummy load or at the lowest practical power.
9. Connect audio only after the PTT switching is reliable.

## Competition Node Power And RF Layout

For a node with RC522 and SX127x sharing SPI, keep separate chip-select lines,
verify the pin map for the exact board revision, and confirm that both devices
can be initialized and read reliably on the shared bus. Place local 100 nF
ceramic and 10 µF bulk decoupling close to each module's supply pins; use a
regulated 3.3 V rail with enough transient headroom for the ESP32 and LoRa
transmit bursts. Do not feed module current through an undersized USB-UART
adapter or thin jumper leads.

Keep the RC522 antenna loop and LoRa antenna physically separated. Before field
use, test tag reads and LoRa packet delivery while the beacon is transmitting,
while Wi-Fi is active, and at the intended battery voltage. Use the exact
regional frequency, permitted conducted power, antenna gain, and duty cycle;
account for antenna gain when checking EIRP. The LoRa module is a data link, not
the ARDF FM transmitter.

For PTT, retain the isolated transistor/opto interface and verify that reset,
watchdog recovery, brownout, and an unplugged ESP32 leave PTT released. Do not
connect an unknown accessory pin directly to an ESP32 GPIO. Test into a dummy
load or at minimum power after confirming the radio pinout and audio level.

## Cheap Radio Caveats

- Some radios combine mic bias, PTT, and audio in ways that need extra
  resistors.
- Some speaker-mic cables use non-obvious wire colors.
- Some radios produce RF noise that can reset a nearby ESP32. Keep wires short,
  add decoupling, and use a separate enclosure ground plan if needed.
- VOX can work for tests, but wired PTT is more predictable for a beacon.
