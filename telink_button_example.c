#include "tl_common.h"
#include "drivers.h"
#include "gpio.h"

// Button configuration
#define BUTTON_PIN          GPIO_PC4    // Define your button pin
#define BUTTON_PRESSED      0           // Assuming active low button
#define BUTTON_RELEASED     1

// Button state variables
static u8 button_state = BUTTON_RELEASED;
static u8 button_last_state = BUTTON_RELEASED;
static u32 button_press_time = 0;
static u8 button_clicked = 0;

// Button debounce parameters
#define DEBOUNCE_TIME_MS    50
#define LONG_PRESS_TIME_MS  1000

// Function prototypes
void button_init(void);
void button_task(void);
void button_click_handler(void);
void button_long_press_handler(void);

/**
 * @brief Initialize button GPIO and configuration
 */
void button_init(void) {
    // Configure button pin as input with pull-up
    gpio_set_func(BUTTON_PIN, AS_GPIO);
    gpio_set_input_en(BUTTON_PIN, 1);
    gpio_set_output_en(BUTTON_PIN, 0);
    gpio_setup_up_down_resistor(BUTTON_PIN, PM_PIN_PULLUP_10K);
    
    // Initialize button state
    button_state = gpio_read(BUTTON_PIN);
    button_last_state = button_state;
    button_press_time = 0;
    button_clicked = 0;
}

/**
 * @brief Button state machine and debounce handling
 * Call this function periodically in main loop
 */
void button_task(void) {
    static u32 last_check_time = 0;
    u32 current_time = clock_time();
    
    // Check button every few milliseconds
    if (clock_time_exceed(last_check_time, 10 * 1000)) { // 10ms interval
        last_check_time = current_time;
        
        u8 current_button_state = gpio_read(BUTTON_PIN);
        
        // Debounce logic
        if (current_button_state != button_state) {
            static u32 debounce_start_time = 0;
            static u8 debounce_active = 0;
            
            if (!debounce_active) {
                debounce_start_time = current_time;
                debounce_active = 1;
            }
            
            // Check if debounce time has passed
            if (clock_time_exceed(debounce_start_time, DEBOUNCE_TIME_MS * 1000)) {
                button_last_state = button_state;
                button_state = current_button_state;
                debounce_active = 0;
                
                // Button press detected
                if (button_state == BUTTON_PRESSED && button_last_state == BUTTON_RELEASED) {
                    button_press_time = current_time;
                }
                
                // Button release detected
                if (button_state == BUTTON_RELEASED && button_last_state == BUTTON_PRESSED) {
                    u32 press_duration = current_time - button_press_time;
                    
                    if (press_duration < LONG_PRESS_TIME_MS * 1000) {
                        // Short press - button click
                        button_clicked = 1;
                    }
                }
            }
        } else {
            // Check for long press
            if (button_state == BUTTON_PRESSED && 
                clock_time_exceed(button_press_time, LONG_PRESS_TIME_MS * 1000)) {
                button_long_press_handler();
                button_press_time = current_time; // Reset to avoid repeated triggers
            }
        }
        
        // Handle button click
        if (button_clicked) {
            button_clicked = 0;
            button_click_handler();
        }
    }
}

/**
 * @brief Handle button click event
 */
void button_click_handler(void) {
    // Add your button click logic here
    
    // Example: Toggle LED
    static u8 led_state = 0;
    led_state = !led_state;
    gpio_write(GPIO_PB5, led_state); // Assuming LED on PB5
    
    // Example: Send BLE notification
    // ble_send_notification(BUTTON_CLICK_EVENT);
    
    // Example: Print debug message
    printf("Button clicked!\n");
}

/**
 * @brief Handle button long press event
 */
void button_long_press_handler(void) {
    // Add your long press logic here
    
    // Example: Enter pairing mode
    printf("Button long pressed - entering pairing mode\n");
    
    // Example: Reset device
    // device_reset();
}

/**
 * @brief Alternative interrupt-based button handling
 */
void button_interrupt_init(void) {
    // Configure button pin for interrupt
    gpio_set_func(BUTTON_PIN, AS_GPIO);
    gpio_set_input_en(BUTTON_PIN, 1);
    gpio_set_output_en(BUTTON_PIN, 0);
    gpio_setup_up_down_resistor(BUTTON_PIN, PM_PIN_PULLUP_10K);
    
    // Enable interrupt on falling edge (button press)
    gpio_set_interrupt(BUTTON_PIN, POL_FALLING);
    
    // Enable IRQ
    irq_enable();
}

/**
 * @brief GPIO interrupt handler
 */
_attribute_ram_code_ void gpio_irq_handler(void) {
    u32 src = gpio_irq_get();
    
    if (src & BUTTON_PIN) {
        // Clear interrupt flag
        gpio_irq_clr(BUTTON_PIN);
        
        // Handle button press in interrupt
        // Note: Keep interrupt handlers short and fast
        button_clicked = 1; // Set flag for main loop processing
    }
}

/**
 * @brief Main application loop with button handling
 */
int main(void) {
    // System initialization
    cpu_wakeup_init();
    clock_init(SYS_CLK_24M_Crystal);
    
    // Initialize GPIO and button
    gpio_init(1);
    button_init();
    
    // Initialize LED for testing
    gpio_set_func(GPIO_PB5, AS_GPIO);
    gpio_set_output_en(GPIO_PB5, 1);
    gpio_set_input_en(GPIO_PB5, 0);
    
    // Main application loop
    while (1) {
        button_task(); // Handle button processing
        
        // Other application tasks
        // ...
        
        // Small delay
        sleep_us(1000); // 1ms delay
    }
}

// Alternative using state machine approach
typedef enum {
    BUTTON_STATE_IDLE,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_DEBOUNCE,
    BUTTON_STATE_LONG_PRESS
} button_state_t;

typedef struct {
    button_state_t state;
    u32 press_time;
    u32 last_check_time;
    u8 current_level;
    u8 last_level;
} button_t;

static button_t button;

/**
 * @brief Advanced button state machine
 */
void button_state_machine(void) {
    u32 current_time = clock_time();
    u8 pin_level = gpio_read(BUTTON_PIN);
    
    switch (button.state) {
        case BUTTON_STATE_IDLE:
            if (pin_level == BUTTON_PRESSED) {
                button.state = BUTTON_STATE_DEBOUNCE;
                button.press_time = current_time;
            }
            break;
            
        case BUTTON_STATE_DEBOUNCE:
            if (clock_time_exceed(button.press_time, DEBOUNCE_TIME_MS * 1000)) {
                if (pin_level == BUTTON_PRESSED) {
                    button.state = BUTTON_STATE_PRESSED;
                } else {
                    button.state = BUTTON_STATE_IDLE;
                }
            }
            break;
            
        case BUTTON_STATE_PRESSED:
            if (pin_level == BUTTON_RELEASED) {
                // Button released - trigger click
                button_click_handler();
                button.state = BUTTON_STATE_IDLE;
            } else if (clock_time_exceed(button.press_time, LONG_PRESS_TIME_MS * 1000)) {
                button.state = BUTTON_STATE_LONG_PRESS;
                button_long_press_handler();
            }
            break;
            
        case BUTTON_STATE_LONG_PRESS:
            if (pin_level == BUTTON_RELEASED) {
                button.state = BUTTON_STATE_IDLE;
            }
            break;
    }
    
    button.current_level = pin_level;
}