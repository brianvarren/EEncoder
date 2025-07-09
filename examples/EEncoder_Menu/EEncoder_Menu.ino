/*
  EEncoder Menu Navigation Example
  
  Demonstrates using an encoder for menu navigation with wraparound.
  This example shows how to handle position tracking in your application
  code while using the library's increment-based approach.
  
  - Rotate: Navigate through menu items
  - Press: Select current item
  - Long press: Go back/exit
  
  The menu wraps around at both ends for smooth navigation.
*/

#include <EEncoder.h>

// Hardware setup
const uint8_t ENCODER_PIN_A = 2;
const uint8_t ENCODER_PIN_B = 3;
const uint8_t ENCODER_BUTTON = 4;

// Create encoder
// Using 4 counts per detent (typical for most encoders)
EEncoder menuEncoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_BUTTON, 4);

// Menu structure
struct MenuItem {
    const char* name;
    void (*action)();  // Function to call when selected
};

// Menu actions (placeholder functions)
void actionOscillator() { Serial.println("  → Entering Oscillator settings..."); }
void actionFilter() { Serial.println("  → Entering Filter settings..."); }
void actionEnvelope() { Serial.println("  → Entering Envelope settings..."); }
void actionLFO() { Serial.println("  → Entering LFO settings..."); }
void actionEffects() { Serial.println("  → Entering Effects settings..."); }
void actionSave() { Serial.println("  → Saving patch..."); }
void actionLoad() { Serial.println("  → Loading patch..."); }

// Main menu items
MenuItem mainMenu[] = {
    {"Oscillator", actionOscillator},
    {"Filter", actionFilter},
    {"Envelope", actionEnvelope},
    {"LFO", actionLFO},
    {"Effects", actionEffects},
    {"Save Patch", actionSave},
    {"Load Patch", actionLoad}
};

const int MENU_SIZE = sizeof(mainMenu) / sizeof(MenuItem);

// Menu state
struct MenuState {
    int currentIndex = 0;
    int previousIndex = -1;  // Force initial display
    bool needsRedraw = true;
} menu;

// Display the menu (would be on OLED/LCD in real application)
void displayMenu() {
    Serial.println("\n=== SYNTHESIZER MENU ===");
    
    for (int i = 0; i < MENU_SIZE; i++) {
        if (i == menu.currentIndex) {
            Serial.print(" > ");  // Cursor
        } else {
            Serial.print("   ");
        }
        
        Serial.print(i + 1);
        Serial.print(". ");
        Serial.println(mainMenu[i].name);
    }
    
    Serial.println("\n[Rotate to navigate, press to select, long press to exit]");
}

// Handle encoder rotation
void onMenuNavigate(EEncoder& enc) {
    // Get normalized increment - always ±1 per physical click
    int8_t increment = enc.getIncrement();
    
    // Update position with wraparound
    menu.currentIndex += increment;
    
    // Wrap around menu
    if (menu.currentIndex >= MENU_SIZE) {
        menu.currentIndex = 0;  // Wrap to beginning
    } else if (menu.currentIndex < 0) {
        menu.currentIndex = MENU_SIZE - 1;  // Wrap to end
    }
    
    // Mark for redraw if position changed
    if (menu.currentIndex != menu.previousIndex) {
        menu.needsRedraw = true;
        
        // Immediate feedback
        Serial.print("\n→ ");
        Serial.println(mainMenu[menu.currentIndex].name);
    }
}

// Handle button press (select)
void onMenuSelect(EEncoder& enc) {
    Serial.print("\nSelected: ");
    Serial.println(mainMenu[menu.currentIndex].name);
    
    // Call the associated action
    if (mainMenu[menu.currentIndex].action) {
        mainMenu[menu.currentIndex].action();
    }
    
    // In a real application, you might enter a submenu here
    delay(1000);  // Brief pause for demo
    menu.needsRedraw = true;
}

// Handle long press (exit/back)
void onMenuExit(EEncoder& enc) {
    Serial.println("\n[Long press - Exiting menu]");
    // In a real application, this might go back to previous menu
    // or exit to main synthesizer view
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    
    Serial.println("EEncoder Menu Navigation Example");
    Serial.println("================================");
    
    // Configure encoder
    menuEncoder.setEncoderHandler(onMenuNavigate);
    menuEncoder.setButtonHandler(onMenuSelect);
    menuEncoder.setLongPressHandler(onMenuExit);
    
    // Menu doesn't need acceleration - disable it
    menuEncoder.setAcceleration(false);
    
    // Shorter long press for menu navigation
    menuEncoder.setLongPressDuration(400);
    
    // Initial display
    displayMenu();
}

void loop() {
    // Update encoder
    menuEncoder.update();
    
    // Redraw menu if needed
    if (menu.needsRedraw) {
        menu.previousIndex = menu.currentIndex;
        menu.needsRedraw = false;
        displayMenu();
    }
    
    // In a real synthesizer, you would also:
    // - Update display/OLED
    // - Process audio
    // - Handle other controls
}