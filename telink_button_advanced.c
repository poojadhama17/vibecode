#include "telink_button.h"

// Advanced button context
static button_context_t btn_ctx = {0};

// Configuration parameters
#define DOUBLE_CLICK_TIMEOUT_MS     300     // Max time between clicks for double-click
#define VERY_LONG_PRESS_TIME_MS     3000    // Very long press threshold

// Button event queue
#define EVENT_QUEUE_SIZE 8
static button_event_t event_queue[EVENT_QUEUE_SIZE];
static u8 event_queue_head = 0;
static u8 event_queue_tail = 0;

/**
 * @brief Initialize button system
 */
void button_init(void) {
    // Configure GPIO
    gpio_set_func(BUTTON_PIN, AS_GPIO);
    gpio_set_input_en(BUTTON_PIN, 1);
    gpio_set_output_en(BUTTON_PIN, 0);
    gpio_setup_up_down_resistor(BUTTON_PIN, PM_PIN_PULLUP_10K);
    
    // Initialize button context
    memset(&btn_ctx, 0, sizeof(btn_ctx));
    btn_ctx.state = BUTTON_STATE_IDLE;
    btn_ctx.current_level = gpio_read(BUTTON_PIN);
    btn_ctx.last_level = btn_ctx.current_level;
    
    // Clear event queue
    memset(event_queue, 0, sizeof(event_queue));
    event_queue_head = 0;
    event_queue_tail = 0;
}

/**
 * @brief Add event to queue
 */
static void button_queue_event(button_event_t event) {
    u8 next_head = (event_queue_head + 1) % EVENT_QUEUE_SIZE;
    if (next_head != event_queue_tail) {
        event_queue[event_queue_head] = event;
        event_queue_head = next_head;
    }
}

/**
 * @brief Get event from queue
 */
button_event_t button_get_event(void) {
    if (event_queue_tail == event_queue_head) {
        return BUTTON_EVENT_NONE;
    }
    
    button_event_t event = event_queue[event_queue_tail];
    event_queue_tail = (event_queue_tail + 1) % EVENT_QUEUE_SIZE;
    return event;
}

/**
 * @brief Advanced button state machine with multiple event types
 */
void button_task(void) {
    u32 current_time = clock_time();
    
    // Read current button state
    btn_ctx.current_level = gpio_read(BUTTON_PIN);
    
    switch (btn_ctx.state) {
        case BUTTON_STATE_IDLE:
            if (btn_ctx.current_level == BUTTON_PRESSED && 
                btn_ctx.last_level == BUTTON_RELEASED) {
                btn_ctx.state = BUTTON_STATE_DEBOUNCE;
                btn_ctx.press_time = current_time;
            }
            break;
            
        case BUTTON_STATE_DEBOUNCE:
            if (clock_time_exceed(btn_ctx.press_time, DEBOUNCE_TIME_MS * 1000)) {
                if (btn_ctx.current_level == BUTTON_PRESSED) {
                    btn_ctx.state = BUTTON_STATE_PRESSED;
                } else {
                    btn_ctx.state = BUTTON_STATE_IDLE;
                }
            }
            break;
            
        case BUTTON_STATE_PRESSED:
            if (btn_ctx.current_level == BUTTON_RELEASED) {
                // Button released
                btn_ctx.release_time = current_time;
                btn_ctx.click_count++;
                
                // Check if this could be first click of double-click
                if (btn_ctx.click_count == 1) {
                    btn_ctx.state = BUTTON_STATE_WAIT_DOUBLE_CLICK;
                } else {
                    // Second click
                    button_queue_event(BUTTON_EVENT_DOUBLE_CLICK);
                    btn_ctx.click_count = 0;
                    btn_ctx.state = BUTTON_STATE_IDLE;
                }
            } else if (clock_time_exceed(btn_ctx.press_time, VERY_LONG_PRESS_TIME_MS * 1000)) {
                // Very long press
                button_queue_event(BUTTON_EVENT_VERY_LONG_PRESS);
                btn_ctx.state = BUTTON_STATE_LONG_PRESS;
            } else if (clock_time_exceed(btn_ctx.press_time, LONG_PRESS_TIME_MS * 1000)) {
                // Long press
                button_queue_event(BUTTON_EVENT_LONG_PRESS);
                btn_ctx.state = BUTTON_STATE_LONG_PRESS;
            }
            break;
            
        case BUTTON_STATE_LONG_PRESS:
            if (btn_ctx.current_level == BUTTON_RELEASED) {
                btn_ctx.state = BUTTON_STATE_IDLE;
                btn_ctx.click_count = 0;
            }
            break;
            
        case BUTTON_STATE_WAIT_DOUBLE_CLICK:
            if (btn_ctx.current_level == BUTTON_PRESSED) {
                // Second press detected
                btn_ctx.state = BUTTON_STATE_DEBOUNCE;
                btn_ctx.press_time = current_time;
            } else if (clock_time_exceed(btn_ctx.release_time, DOUBLE_CLICK_TIMEOUT_MS * 1000)) {
                // Timeout - single click
                button_queue_event(BUTTON_EVENT_CLICK);
                btn_ctx.state = BUTTON_STATE_IDLE;
                btn_ctx.click_count = 0;
            }
            break;
    }
    
    btn_ctx.last_level = btn_ctx.current_level;
    
    // Process events
    button_event_t event;
    while ((event = button_get_event()) != BUTTON_EVENT_NONE) {
        switch (event) {
            case BUTTON_EVENT_CLICK:
                on_button_click();
                break;
            case BUTTON_EVENT_DOUBLE_CLICK:
                on_button_double_click();
                break;
            case BUTTON_EVENT_LONG_PRESS:
                on_button_long_press();
                break;
            case BUTTON_EVENT_VERY_LONG_PRESS:
                on_button_very_long_press();
                break;
            default:
                break;
        }
    }
}

/**
 * @brief Check if button is currently pressed
 */
u8 button_is_pressed(void) {
    return (gpio_read(BUTTON_PIN) == BUTTON_PRESSED);
}

/**
 * @brief Get current press duration in milliseconds
 */
u32 button_get_press_duration(void) {
    if (btn_ctx.state == BUTTON_STATE_PRESSED || btn_ctx.state == BUTTON_STATE_LONG_PRESS) {
        return (clock_time() - btn_ctx.press_time) / 1000; // Convert to ms
    }
    return 0;
}

/**
 * @brief Interrupt-based button initialization
 */
void button_interrupt_init(void) {
    // Configure GPIO for interrupt
    gpio_set_func(BUTTON_PIN, AS_GPIO);
    gpio_set_input_en(BUTTON_PIN, 1);
    gpio_set_output_en(BUTTON_PIN, 0);
    gpio_setup_up_down_resistor(BUTTON_PIN, PM_PIN_PULLUP_10K);
    
    // Enable interrupt on both edges
    gpio_set_interrupt(BUTTON_PIN, POL_RISING | POL_FALLING);
    
    // Enable global interrupts
    irq_enable();
}

/**
 * @brief GPIO interrupt handler for button
 */
_attribute_ram_code_ void gpio_irq_handler(void) {
    u32 src = gpio_irq_get();
    
    if (src & BIT(BUTTON_PIN & 0xFF)) {
        // Clear interrupt
        gpio_irq_clr(BIT(BUTTON_PIN & 0xFF));
        
        // Wake up system if needed
        // Note: Keep interrupt handler minimal
        // The actual button processing happens in button_task()
    }
}

// Default event handlers (weak functions - can be overridden)
__attribute__((weak)) void on_button_click(void) {
    // Default single click handler
    printf("Button clicked!\n");
}

__attribute__((weak)) void on_button_double_click(void) {
    // Default double click handler
    printf("Button double-clicked!\n");
}

__attribute__((weak)) void on_button_long_press(void) {
    // Default long press handler
    printf("Button long pressed!\n");
}

__attribute__((weak)) void on_button_very_long_press(void) {
    // Default very long press handler
    printf("Button very long pressed!\n");
}

/**
 * @brief Example application with button handling
 */
void button_demo_application(void) {
    // Initialize system
    cpu_wakeup_init();
    clock_init(SYS_CLK_24M_Crystal);
    gpio_init(1);
    
    // Initialize button
    button_init();
    
    // Main loop
    while (1) {
        button_task();
        
        // Your application code here
        // ...
        
        // Small delay
        sleep_us(5000); // 5ms delay
    }
}

/**
 * @brief Custom event handlers example
 */
void custom_button_handlers(void) {
    // You can override the default handlers like this:
    
    // Single click - toggle LED
    static u8 led_state = 0;
    led_state = !led_state;
    gpio_write(GPIO_PB5, led_state);
    
    // Double click - enter menu mode
    // enter_menu_mode();
    
    // Long press - start pairing
    // start_bluetooth_pairing();
    
    // Very long press - factory reset
    // perform_factory_reset();
}