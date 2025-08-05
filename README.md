# Telium Button Click Code Examples

This repository contains comprehensive button handling code for both **Telink semiconductor microcontrollers** and **Ingenico Telium payment terminals**.

## Overview

The term "Telium" can refer to two different platforms:

1. **Telink Semiconductor Platform** - Modern IoT/BLE microcontrollers
2. **Ingenico Telium Platform** - Traditional payment terminal operating system

This code provides examples for both platforms with robust button handling capabilities.

## Files Structure

```
├── telink_button.h              # Header file for Telink button handling
├── telink_button_example.c      # Basic Telink button implementation
├── telink_button_advanced.c     # Advanced Telink implementation with events
├── ingenico_telium_button.c     # Ingenico payment terminal button handling
├── button_usage_example.c       # Usage examples for both platforms
└── README.md                    # This documentation
```

## Telink Microcontroller Button Handling

### Features

- **Debounced button input** with configurable timing
- **Multiple event types**: Click, double-click, long press, very long press
- **Interrupt and polling modes** supported
- **Event queue system** for reliable event handling
- **State machine implementation** for robust operation

### Basic Usage

```c
#include "telink_button.h"

int main(void) {
    // Initialize system
    cpu_wakeup_init();
    clock_init(SYS_CLK_24M_Crystal);
    gpio_init(1);
    
    // Initialize button
    button_init();
    
    while (1) {
        button_task(); // Handle button events
        sleep_us(5000); // 5ms delay
    }
}

// Implement event handlers
void on_button_click(void) {
    printf("Button clicked!\n");
    // Your action here
}

void on_button_long_press(void) {
    printf("Long press detected!\n");
    // Your action here
}
```

### Configuration

Modify these defines in `telink_button.h`:

```c
#define BUTTON_PIN          GPIO_PC4    // Your button pin
#define BUTTON_PRESSED      0           // 0=active low, 1=active high
#define DEBOUNCE_TIME_MS    50          // Debounce time
#define LONG_PRESS_TIME_MS  1000        // Long press threshold
```

## Ingenico Telium Payment Terminal

### Features

- **Standard payment terminal buttons**: OK, Cancel, F1-F4, arrows, numeric
- **Menu system implementation** with navigation
- **Amount input handling** with proper formatting
- **Transaction workflow examples**
- **Screen display integration**

### Basic Usage

```c
#include "ingenico_telium_button.c"

int main(void) {
    button_system_init();
    
    while (1) {
        int key = wait_for_button(0); // Wait for button press
        handle_button_click(key);
        
        if (key == BUTTON_CANCEL) {
            break; // Exit on cancel
        }
    }
    
    return 0;
}
```

### Button Codes

```c
#define BUTTON_OK           0x0D    // Enter/OK button
#define BUTTON_CANCEL       0x1B    // Cancel/ESC button
#define BUTTON_CLEAR        0x08    // Clear/Backspace
#define BUTTON_F1           0x81    // Function key F1
#define BUTTON_F2           0x82    // Function key F2
#define BUTTON_UP           0x91    // Up arrow
#define BUTTON_DOWN         0x92    // Down arrow
// ... etc
```

## Compilation Instructions

### For Telink Platform

1. Set up Telink SDK environment
2. Add files to your project
3. Configure button pin in header file
4. Compile with Telink toolchain:

```bash
make all
```

### For Ingenico Telium

1. Set up Ingenico SDK/Telium development environment
2. Include Telium libraries
3. Compile with appropriate flags:

```bash
gcc -DTELIUM_PLATFORM ingenico_telium_button.c -o payment_app
```

### For Testing/Simulation

Compile without platform defines for generic testing:

```bash
gcc button_usage_example.c -o button_test
```

## Advanced Features

### Event Queue System (Telink)

The advanced implementation includes an event queue that prevents lost button events:

```c
button_event_t event;
while ((event = button_get_event()) != BUTTON_EVENT_NONE) {
    switch (event) {
        case BUTTON_EVENT_CLICK:
            handle_click();
            break;
        case BUTTON_EVENT_DOUBLE_CLICK:
            handle_double_click();
            break;
        // ...
    }
}
```

### Menu System (Ingenico)

Easy-to-use menu system for payment terminals:

```c
menu_t main_menu = {
    .title = "MAIN MENU",
    .items = {"Sale", "Refund", "Settings", "Exit"},
    .item_count = 4,
    .selected_item = 0
};

int selection = run_menu(&main_menu);
```

## Button Event Types

### Telink Events

- `BUTTON_EVENT_CLICK` - Single button press
- `BUTTON_EVENT_DOUBLE_CLICK` - Two quick presses
- `BUTTON_EVENT_LONG_PRESS` - Hold for 1+ seconds
- `BUTTON_EVENT_VERY_LONG_PRESS` - Hold for 3+ seconds

### Ingenico Events

- Standard keypad input (0-9, A-Z)
- Function keys (F1-F4)
- Navigation (arrows)
- Control keys (OK, Cancel, Clear)

## Customization

### Button Timing

Adjust timing parameters:

```c
#define DEBOUNCE_TIME_MS         50     // Button debounce
#define LONG_PRESS_TIME_MS       1000   // Long press threshold
#define DOUBLE_CLICK_TIMEOUT_MS  300    // Double-click window
#define VERY_LONG_PRESS_TIME_MS  3000   // Very long press
```

### GPIO Configuration

For Telink, modify pin and pull-up settings:

```c
#define BUTTON_PIN          GPIO_PC4
gpio_setup_up_down_resistor(BUTTON_PIN, PM_PIN_PULLUP_10K);
```

## Error Handling

### Telink Platform

- Debounce filtering prevents false triggers
- Event queue prevents lost events
- State machine ensures reliable operation

### Ingenico Platform

- Timeout handling for user input
- Menu validation and bounds checking
- Graceful error recovery

## Performance Considerations

### Telink

- Use interrupt mode for low-power applications
- Polling mode for simple implementations
- Event queue size can be adjusted for memory constraints

### Ingenico

- Minimal processing in button handlers
- Efficient screen updates
- Proper memory management for strings

## Examples

### Simple LED Toggle (Telink)

```c
void on_button_click(void) {
    static u8 led_state = 0;
    led_state = !led_state;
    gpio_write(GPIO_PB5, led_state);
}
```

### Payment Amount Entry (Ingenico)

```c
long amount = get_amount_input();
if (amount > 0) {
    printf("Processing $%ld.%02ld\n", amount/100, amount%100);
    process_payment(amount);
}
```

## Troubleshooting

### Common Issues

1. **Button not responding**
   - Check GPIO pin configuration
   - Verify pull-up/pull-down settings
   - Ensure proper debounce timing

2. **Multiple events triggered**
   - Increase debounce time
   - Check for hardware issues (noisy button)
   - Verify event queue is being processed

3. **Compilation errors**
   - Ensure correct platform defines
   - Include proper SDK headers
   - Check toolchain setup

### Debug Tips

- Use printf statements to trace button events
- Monitor GPIO state with oscilloscope
- Test with different button hardware
- Verify timing parameters are appropriate

## License

This code is provided as examples for educational and development purposes. Modify as needed for your specific application requirements.

## Support

For questions about:
- **Telink platform**: Refer to Telink SDK documentation
- **Ingenico platform**: Consult Ingenico Telium development guides
- **General programming**: Standard C programming resources

---

**Note**: This code provides examples for button handling. Always test thoroughly in your specific hardware environment before production use.