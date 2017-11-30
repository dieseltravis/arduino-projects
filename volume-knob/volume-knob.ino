// https://github.com/adafruit/Adafruit-Trinket-USB/tree/master/TrinketHidCombo
#include "TrinketHidCombo.h"

// original code source: 
// https://learn.adafruit.com/trinket-usb-volume-knob/add-a-mute-button

#define TRINKET_PINx  PINB

const int PIN_ENCODER_A = 2;
const int PIN_ENCODER_B = 0;
const int PIN_ENCODER_SWITCH = 1;

const int ENCODER_NOT_MOVED = 0;
const int ENCODER_MOVED_DOWN = -1;
const int ENCODER_MOVED_UP = 1;

const int DEBOUNCE_DELAY_MS = 100;

static byte enc_prev_pos = 0;
static byte enc_flags = 0;
//static char sw_was_pressed = 0;
static boolean mutePressed = false;

void setup() {
    // set pins as input with internal pull-up resistors enabled
    pinMode(PIN_ENCODER_A, INPUT);
    pinMode(PIN_ENCODER_B, INPUT);
    digitalWrite(PIN_ENCODER_A, HIGH);
    digitalWrite(PIN_ENCODER_B, HIGH);

    pinMode(PIN_ENCODER_SWITCH, INPUT);
    // the switch is active-high, not active-low
    // since it shares the pin with Trinket's built-in LED
    // the LED acts as a pull-down resistor
    digitalWrite(PIN_ENCODER_SWITCH, LOW);
    
    // start the USB device engine and enumerate
    TrinketHidCombo.begin();

    // get an initial reading on the encoder pins
    if (digitalRead(PIN_ENCODER_A) == LOW) {
        enc_prev_pos |= (1 << 0);
    }
    if (digitalRead(PIN_ENCODER_B) == LOW) {
        enc_prev_pos |= (1 << 1);
    }
}

void loop() {
    // 1 or -1 if moved, sign is direction
    int enc_action = ENCODER_NOT_MOVED;

    // note: for better performance, the code will now use
    // direct port access techniques
    // http://www.arduino.cc/en/Reference/PortManipulation
    byte enc_cur_pos = 0;
    // read in the encoder state first
    if (bit_is_clear(TRINKET_PINx, PIN_ENCODER_A)) {
        enc_cur_pos |= (1 << 0);
    }
    if (bit_is_clear(TRINKET_PINx, PIN_ENCODER_B)) {
        enc_cur_pos |= (1 << 1);
    }

    // if any rotation at all
    if (enc_cur_pos != enc_prev_pos) {
        if (enc_prev_pos == 0x00) {
            // this is the first edge
            if (enc_cur_pos == 0x01) {
                enc_flags |= (1 << 0);
            } else if (enc_cur_pos == 0x02) {
                enc_flags |= (1 << 1);
            }
        }

        if (enc_cur_pos == 0x03) {
            // this is when the encoder is in the middle of a "step"
            enc_flags |= (1 << 4);
        } else if (enc_cur_pos == 0x00) {
            // this is the final edge
            if (enc_prev_pos == 0x02) {
                enc_flags |= (1 << 2);
            } else if (enc_prev_pos == 0x01) {
                enc_flags |= (1 << 3);
            }

            // check the first and last edge
            // or maybe one edge is missing, if missing then require the middle state
            // this will reject bounces and false movements
            if (bit_is_set(enc_flags, 0) && (bit_is_set(enc_flags, 2) || bit_is_set(enc_flags, 4))) {
                enc_action = ENCODER_MOVED_UP;
            } else if (bit_is_set(enc_flags, 2) && (bit_is_set(enc_flags, 0) || bit_is_set(enc_flags, 4))) {
                enc_action = ENCODER_MOVED_UP;
            } else if (bit_is_set(enc_flags, 1) && (bit_is_set(enc_flags, 3) || bit_is_set(enc_flags, 4))) {
                enc_action = ENCODER_MOVED_DOWN;
            } else if (bit_is_set(enc_flags, 3) && (bit_is_set(enc_flags, 1) || bit_is_set(enc_flags, 4))) {
                enc_action = ENCODER_MOVED_DOWN;
            }

            // reset for next time
            enc_flags = 0;
        }
    }

    enc_prev_pos = enc_cur_pos;

    if (enc_action == ENCODER_MOVED_UP) {
        TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP);
    } else if (enc_action == ENCODER_MOVED_DOWN) {
        TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN);
    }

    // remember that the switch is active-high
    if (bit_is_set(TRINKET_PINx, PIN_ENCODER_SWITCH)) {
        // only on initial press, so the keystroke is not repeated while the button is held down
        if (!mutePressed) {
            TrinketHidCombo.pressMultimediaKey(MMKEY_MUTE);
            // debounce delay
            delay(DEBOUNCE_DELAY_MS);
        }
        mutePressed = true;
    } else {
        if (mutePressed) {
            // debounce delay
            delay(DEBOUNCE_DELAY_MS);
        }
        mutePressed = false;
    }

    // check if USB needs anything done
    TrinketHidCombo.poll();
}