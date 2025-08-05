/*
 * Ingenico Telium Payment Terminal Button Handling
 * Example code for handling button clicks on Ingenico terminals
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Telium platform includes (adjust based on your SDK version)
#ifdef TELIUM_PLATFORM
#include "AYGSHELL.H"
#include "GL_GraphicLib.h"
#include "cu_term.h"
#include "GL_Message.h"
#include "Training.h"
#include "cu_entry.h"
#endif

// Button definitions for Ingenico terminals
#define BUTTON_OK           0x0D    // Enter/OK button
#define BUTTON_CANCEL       0x1B    // Cancel/ESC button
#define BUTTON_CLEAR        0x08    // Clear/Backspace button
#define BUTTON_F1           0x81    // Function key F1
#define BUTTON_F2           0x82    // Function key F2
#define BUTTON_F3           0x83    // Function key F3
#define BUTTON_F4           0x84    // Function key F4
#define BUTTON_UP           0x91    // Up arrow
#define BUTTON_DOWN         0x92    // Down arrow
#define BUTTON_LEFT         0x93    // Left arrow
#define BUTTON_RIGHT        0x94    // Right arrow

// Button event structure
typedef struct {
    int button_code;
    unsigned long timestamp;
    int event_type;  // 0=press, 1=release, 2=long_press
} button_event_t;

// Event types
#define EVENT_BUTTON_PRESS      0
#define EVENT_BUTTON_RELEASE    1
#define EVENT_BUTTON_LONG_PRESS 2

/**
 * @brief Initialize button handling system
 */
void button_system_init(void) {
#ifdef TELIUM_PLATFORM
    // Initialize the keyboard/button system
    // This is typically done automatically by the Telium OS
    
    // Set keyboard mode if needed
    // Keyboard_SetMode(KEYBOARD_MODE_ALPHA);
#endif
}

/**
 * @brief Wait for button press and return button code
 * @param timeout_ms Timeout in milliseconds (0 = no timeout)
 * @return Button code or -1 if timeout
 */
int wait_for_button(int timeout_ms) {
#ifdef TELIUM_PLATFORM
    int key = 0;
    
    if (timeout_ms > 0) {
        // Wait with timeout
        key = Keyboard_GetKey(timeout_ms);
    } else {
        // Wait indefinitely
        key = Keyboard_GetKey(0);
    }
    
    return key;
#else
    // Simulation for non-Telium platforms
    printf("Waiting for button press...\n");
    return getchar();
#endif
}

/**
 * @brief Check if a button is currently pressed (non-blocking)
 * @return Button code or 0 if no button pressed
 */
int check_button_pressed(void) {
#ifdef TELIUM_PLATFORM
    return Keyboard_CheckKey();
#else
    // Simulation - not accurate for real implementation
    return 0;
#endif
}

/**
 * @brief Handle button click events
 * @param button_code The button that was pressed
 */
void handle_button_click(int button_code) {
    switch (button_code) {
        case BUTTON_OK:
            printf("OK button pressed\n");
            on_ok_button();
            break;
            
        case BUTTON_CANCEL:
            printf("Cancel button pressed\n");
            on_cancel_button();
            break;
            
        case BUTTON_CLEAR:
            printf("Clear button pressed\n");
            on_clear_button();
            break;
            
        case BUTTON_F1:
            printf("F1 button pressed\n");
            on_f1_button();
            break;
            
        case BUTTON_F2:
            printf("F2 button pressed\n");
            on_f2_button();
            break;
            
        case BUTTON_F3:
            printf("F3 button pressed\n");
            on_f3_button();
            break;
            
        case BUTTON_F4:
            printf("F4 button pressed\n");
            on_f4_button();
            break;
            
        case BUTTON_UP:
            printf("Up arrow pressed\n");
            on_up_button();
            break;
            
        case BUTTON_DOWN:
            printf("Down arrow pressed\n");
            on_down_button();
            break;
            
        case BUTTON_LEFT:
            printf("Left arrow pressed\n");
            on_left_button();
            break;
            
        case BUTTON_RIGHT:
            printf("Right arrow pressed\n");
            on_right_button();
            break;
            
        default:
            if (button_code >= '0' && button_code <= '9') {
                printf("Number key %c pressed\n", button_code);
                on_number_key(button_code - '0');
            } else if (button_code >= 'A' && button_code <= 'Z') {
                printf("Letter key %c pressed\n", button_code);
                on_letter_key(button_code);
            } else {
                printf("Unknown button: 0x%02X\n", button_code);
                on_unknown_button(button_code);
            }
            break;
    }
}

/**
 * @brief Simple menu system using buttons
 */
typedef struct {
    char* title;
    char* items[10];
    int item_count;
    int selected_item;
} menu_t;

void display_menu(menu_t* menu) {
#ifdef TELIUM_PLATFORM
    // Clear screen
    GL_GraphicLib_ClearScreen();
    
    // Display title
    GL_GraphicLib_Print(0, 0, menu->title);
    
    // Display menu items
    for (int i = 0; i < menu->item_count; i++) {
        if (i == menu->selected_item) {
            GL_GraphicLib_Print(0, (i + 2) * 16, "> %s", menu->items[i]);
        } else {
            GL_GraphicLib_Print(0, (i + 2) * 16, "  %s", menu->items[i]);
        }
    }
#else
    // Console simulation
    printf("\n=== %s ===\n", menu->title);
    for (int i = 0; i < menu->item_count; i++) {
        if (i == menu->selected_item) {
            printf("> %s\n", menu->items[i]);
        } else {
            printf("  %s\n", menu->items[i]);
        }
    }
    printf("\nUse UP/DOWN to navigate, OK to select, CANCEL to exit\n");
#endif
}

int run_menu(menu_t* menu) {
    int selected = -1;
    
    display_menu(menu);
    
    while (selected == -1) {
        int key = wait_for_button(0); // Wait indefinitely
        
        switch (key) {
            case BUTTON_UP:
                if (menu->selected_item > 0) {
                    menu->selected_item--;
                    display_menu(menu);
                }
                break;
                
            case BUTTON_DOWN:
                if (menu->selected_item < menu->item_count - 1) {
                    menu->selected_item++;
                    display_menu(menu);
                }
                break;
                
            case BUTTON_OK:
                selected = menu->selected_item;
                break;
                
            case BUTTON_CANCEL:
                selected = -1;
                break;
        }
    }
    
    return selected;
}

/**
 * @brief Example payment terminal application with button handling
 */
void payment_terminal_app(void) {
    button_system_init();
    
    // Create main menu
    menu_t main_menu = {
        .title = "PAYMENT TERMINAL",
        .items = {
            "1. Sale",
            "2. Refund", 
            "3. Void",
            "4. Reports",
            "5. Settings",
            "6. Exit"
        },
        .item_count = 6,
        .selected_item = 0
    };
    
    while (1) {
        int selection = run_menu(&main_menu);
        
        if (selection == -1 || selection == 5) {
            // Cancel or Exit
            break;
        }
        
        switch (selection) {
            case 0: // Sale
                handle_sale_transaction();
                break;
            case 1: // Refund
                handle_refund_transaction();
                break;
            case 2: // Void
                handle_void_transaction();
                break;
            case 3: // Reports
                handle_reports_menu();
                break;
            case 4: // Settings
                handle_settings_menu();
                break;
        }
    }
}

/**
 * @brief Get numeric input from user
 */
long get_amount_input(void) {
    char amount_str[20] = {0};
    int pos = 0;
    
#ifdef TELIUM_PLATFORM
    GL_GraphicLib_ClearScreen();
    GL_GraphicLib_Print(0, 0, "Enter Amount:");
    GL_GraphicLib_Print(0, 32, "$");
#else
    printf("Enter Amount: $");
#endif
    
    while (1) {
        int key = wait_for_button(0);
        
        if (key >= '0' && key <= '9' && pos < 18) {
            amount_str[pos++] = key;
            amount_str[pos] = '\0';
            
#ifdef TELIUM_PLATFORM
            GL_GraphicLib_Print(16, 32, "%s", amount_str);
#else
            printf("%c", key);
#endif
        } else if (key == BUTTON_CLEAR && pos > 0) {
            // Backspace
            amount_str[--pos] = '\0';
#ifdef TELIUM_PLATFORM
            GL_GraphicLib_ClearArea(16, 32, 200, 16);
            GL_GraphicLib_Print(16, 32, "%s", amount_str);
#else
            printf("\b \b");
#endif
        } else if (key == BUTTON_OK && pos > 0) {
            // Confirm entry
            break;
        } else if (key == BUTTON_CANCEL) {
            // Cancel
            return -1;
        }
    }
    
    return atol(amount_str);
}

// Button event handler implementations
void on_ok_button(void) {
    // Handle OK button press
    printf("Processing OK action...\n");
}

void on_cancel_button(void) {
    // Handle Cancel button press
    printf("Processing Cancel action...\n");
}

void on_clear_button(void) {
    // Handle Clear button press
    printf("Processing Clear action...\n");
}

void on_f1_button(void) {
    // Handle F1 button press
    printf("F1 function triggered\n");
}

void on_f2_button(void) {
    // Handle F2 button press
    printf("F2 function triggered\n");
}

void on_f3_button(void) {
    // Handle F3 button press
    printf("F3 function triggered\n");
}

void on_f4_button(void) {
    // Handle F4 button press
    printf("F4 function triggered\n");
}

void on_up_button(void) {
    // Handle Up arrow button press
    printf("Navigation: Up\n");
}

void on_down_button(void) {
    // Handle Down arrow button press
    printf("Navigation: Down\n");
}

void on_left_button(void) {
    // Handle Left arrow button press
    printf("Navigation: Left\n");
}

void on_right_button(void) {
    // Handle Right arrow button press
    printf("Navigation: Right\n");
}

void on_number_key(int number) {
    // Handle numeric key press
    printf("Number %d entered\n", number);
}

void on_letter_key(char letter) {
    // Handle letter key press
    printf("Letter %c entered\n", letter);
}

void on_unknown_button(int button_code) {
    // Handle unknown button press
    printf("Unknown button code: 0x%02X\n", button_code);
}

// Transaction handlers (examples)
void handle_sale_transaction(void) {
    printf("Processing Sale Transaction...\n");
    long amount = get_amount_input();
    if (amount > 0) {
        printf("Sale amount: $%ld.%02ld\n", amount/100, amount%100);
        // Process sale...
    }
}

void handle_refund_transaction(void) {
    printf("Processing Refund Transaction...\n");
    // Implementation here...
}

void handle_void_transaction(void) {
    printf("Processing Void Transaction...\n");
    // Implementation here...
}

void handle_reports_menu(void) {
    printf("Opening Reports Menu...\n");
    // Implementation here...
}

void handle_settings_menu(void) {
    printf("Opening Settings Menu...\n");
    // Implementation here...
}

/**
 * @brief Main entry point for Telium application
 */
#ifdef TELIUM_PLATFORM
int main(void) {
    payment_terminal_app();
    return 0;
}
#endif