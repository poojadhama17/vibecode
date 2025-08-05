#ifndef TELINK_BUTTON_H
#define TELINK_BUTTON_H

#include "tl_common.h"
#include "gpio.h"

// Button configuration defines
#define BUTTON_PIN          GPIO_PC4    // Modify this to your actual button pin
#define BUTTON_PRESSED      0           // Active low button (change to 1 for active high)
#define BUTTON_RELEASED     1
#define DEBOUNCE_TIME_MS    50          // Button debounce time in milliseconds
#define LONG_PRESS_TIME_MS  1000        // Long press threshold in milliseconds

// Button event types
typedef enum {
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_CLICK,
    BUTTON_EVENT_DOUBLE_CLICK,
    BUTTON_EVENT_LONG_PRESS,
    BUTTON_EVENT_VERY_LONG_PRESS
} button_event_t;

// Button state enumeration
typedef enum {
    BUTTON_STATE_IDLE = 0,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_DEBOUNCE,
    BUTTON_STATE_LONG_PRESS,
    BUTTON_STATE_WAIT_DOUBLE_CLICK
} button_state_t;

// Button structure
typedef struct {
    button_state_t state;
    u32 press_time;
    u32 release_time;
    u32 last_check_time;
    u8 current_level;
    u8 last_level;
    u8 click_count;
    u8 event_pending;
} button_context_t;

// Function prototypes
void button_init(void);
void button_task(void);
button_event_t button_get_event(void);
void button_interrupt_init(void);
void button_state_machine(void);

// Event handler function prototypes (implement these in your application)
void on_button_click(void);
void on_button_double_click(void);
void on_button_long_press(void);
void on_button_very_long_press(void);

// Utility functions
u8 button_is_pressed(void);
u32 button_get_press_duration(void);

#endif // TELINK_BUTTON_H