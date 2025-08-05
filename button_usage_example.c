/*
 * Button Usage Example for Telium Code
 * 
 * This file demonstrates how to use the button handling code
 * for both Telink microcontrollers and Ingenico payment terminals
 */

#include <stdio.h>

// For Telink platform
#ifdef TELINK_PLATFORM
#include "telink_button.h"

// Custom event handlers for your application
void on_button_click(void) {
    static int click_count = 0;
    click_count++;
    printf("Button clicked %d times!\n", click_count);
    
    // Example: Toggle LED or perform action
    // toggle_led();
    // send_bluetooth_notification();
}

void on_button_double_click(void) {
    printf("Double click detected - entering menu mode\n");
    // Example: Enter configuration mode
    // enter_config_mode();
}

void on_button_long_press(void) {
    printf("Long press detected - starting pairing\n");
    // Example: Start Bluetooth pairing
    // start_pairing_mode();
}

void on_button_very_long_press(void) {
    printf("Very long press - factory reset!\n");
    // Example: Factory reset
    // perform_factory_reset();
}

int main(void) {
    // Initialize Telink system
    cpu_wakeup_init();
    clock_init(SYS_CLK_24M_Crystal);
    gpio_init(1);
    
    // Initialize button system
    button_init();
    
    printf("Telink Button Example Started\n");
    printf("Press the button to test different events:\n");
    printf("- Single click: Toggle action\n");
    printf("- Double click: Menu mode\n");
    printf("- Long press: Pairing mode\n");
    printf("- Very long press: Factory reset\n");
    
    while (1) {
        button_task(); // Handle button events
        
        // Your main application code here
        // ...
        
        sleep_us(5000); // 5ms delay
    }
    
    return 0;
}

#endif

// For Ingenico Telium platform
#ifdef TELIUM_PLATFORM

// Include the Ingenico button handling
extern void button_system_init(void);
extern int wait_for_button(int timeout_ms);
extern void handle_button_click(int button_code);

// Application-specific button handlers
void on_ok_button(void) {
    printf("Confirming current action...\n");
    // Process confirmation
}

void on_cancel_button(void) {
    printf("Cancelling current operation...\n");
    // Handle cancellation
}

void on_f1_button(void) {
    printf("F1 - Quick Sale\n");
    // Start quick sale process
}

void on_f2_button(void) {
    printf("F2 - Reports\n");
    // Open reports menu
}

void simple_payment_app(void) {
    printf("=== Payment Terminal Demo ===\n");
    printf("Press buttons to interact:\n");
    printf("F1 - Quick Sale\n");
    printf("F2 - Reports\n");
    printf("OK - Confirm\n");
    printf("Cancel - Exit\n");
    
    while (1) {
        int key = wait_for_button(0); // Wait for button press
        
        handle_button_click(key);
        
        if (key == 0x1B) { // Cancel button
            printf("Exiting application...\n");
            break;
        }
    }
}

int main(void) {
    button_system_init();
    simple_payment_app();
    return 0;
}

#endif

// Generic example that works on any platform
#ifndef TELINK_PLATFORM
#ifndef TELIUM_PLATFORM

#include <stdio.h>
#include <stdlib.h>

// Simulate button events for testing
typedef enum {
    BTN_CLICK = 1,
    BTN_DOUBLE_CLICK,
    BTN_LONG_PRESS,
    BTN_EXIT
} button_event_sim_t;

void simulate_button_events(void) {
    printf("=== Button Event Simulation ===\n");
    printf("Choose an event to simulate:\n");
    printf("1 - Single Click\n");
    printf("2 - Double Click\n");
    printf("3 - Long Press\n");
    printf("4 - Exit\n");
    
    while (1) {
        int choice;
        printf("\nEnter choice (1-4): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case BTN_CLICK:
                printf("Simulating single click...\n");
                printf("Action: LED toggled\n");
                break;
                
            case BTN_DOUBLE_CLICK:
                printf("Simulating double click...\n");
                printf("Action: Entering menu mode\n");
                break;
                
            case BTN_LONG_PRESS:
                printf("Simulating long press...\n");
                printf("Action: Starting pairing mode\n");
                break;
                
            case BTN_EXIT:
                printf("Exiting simulation...\n");
                return;
                
            default:
                printf("Invalid choice!\n");
                break;
        }
    }
}

int main(void) {
    printf("Generic Button Handler Demo\n");
    printf("This simulates button events since no specific platform is detected.\n\n");
    
    simulate_button_events();
    
    return 0;
}

#endif
#endif