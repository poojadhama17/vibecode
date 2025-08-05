# Telium Button Click Examples

This repository contains comprehensive examples of button click functionality in Telium, a JavaScript framework for building terminal applications.

## Files

- `telium-button-examples.js` - Individual button examples and patterns
- `telium-app-example.js` - Complete application demonstrating button usage
- `package.json` - Project configuration and dependencies

## Quick Start

1. Install dependencies:
```bash
npm install
```

2. Run the complete application:
```bash
npm start
```

3. Or run individual examples:
```bash
npm run examples
```

## Button Click Patterns

### 1. Basic Button Click
```javascript
const button = new Button({
  label: 'Click Me!',
  onClick: () => {
    console.log('Button clicked!');
  }
});
```

### 2. Button with State Management
```javascript
const button = new Button({
  label: 'Toggle State',
  onClick: (button) => {
    const currentState = button.getState('toggled') || false;
    button.setState('toggled', !currentState);
    button.setLabel(currentState ? 'Toggle State' : 'Toggled!');
  }
});
```

### 3. Async Button Operations
```javascript
const button = new Button({
  label: 'Load Data',
  onClick: async (button) => {
    button.setLabel('Loading...');
    button.setDisabled(true);
    
    try {
      await someAsyncOperation();
      button.setLabel('Data Loaded');
    } catch (error) {
      button.setLabel('Error - Retry');
    } finally {
      button.setDisabled(false);
    }
  }
});
```

### 4. Styled Button with Events
```javascript
const button = new Button({
  label: 'Styled Button',
  style: {
    border: 'double',
    borderColor: 'blue',
    backgroundColor: 'cyan',
    color: 'black'
  },
  onClick: (button, event) => {
    console.log('Button clicked with event:', event);
    button.setStyle({
      backgroundColor: 'green',
      color: 'white'
    });
  },
  onFocus: (button) => {
    button.setStyle({ borderColor: 'yellow' });
  },
  onBlur: (button) => {
    button.setStyle({ borderColor: 'blue' });
  }
});
```

### 5. Button Groups
```javascript
const buttonGroup = new Box({
  direction: 'horizontal',
  children: [
    new Button({
      label: 'Option 1',
      onClick: (button) => {
        buttonGroup.setState('selected', 'option1');
        updateButtonGroup(buttonGroup);
      }
    }),
    // ... more buttons
  ]
});
```

## Key Concepts

### Event Handling
- `onClick`: Primary click handler
- `onFocus`: Called when button gains focus
- `onBlur`: Called when button loses focus
- `onKeyPress`: Handle keyboard events

### State Management
- `setState(key, value)`: Store button state
- `getState(key)`: Retrieve button state
- `setLabel(text)`: Update button text
- `setDisabled(boolean)`: Enable/disable button

### Styling
- `setStyle(styleObject)`: Update button appearance
- Common styles: `backgroundColor`, `color`, `border`, `borderColor`

### Async Operations
- Use `async/await` in click handlers
- Update button state during operations
- Handle errors gracefully
- Provide user feedback

## Best Practices

1. **Always provide user feedback** - Update button labels during operations
2. **Handle errors gracefully** - Show error states and recovery options
3. **Use appropriate styling** - Make buttons visually distinct for different actions
4. **Implement keyboard shortcuts** - Allow Enter/Space to activate buttons
5. **Group related buttons** - Use containers to organize button layouts
6. **Disable buttons during operations** - Prevent multiple simultaneous actions

## Examples Included

- Basic click handling
- State management
- Async operations
- Custom styling
- Button groups
- Keyboard shortcuts
- Conditional rendering
- Confirmation dialogs
- Progress indicators
- Tooltips

## Running the Examples

The examples demonstrate:
- Counter functionality
- Data loading with progress
- Input form submission
- Navigation controls
- Action buttons (Save/Delete)
- Event logging

Each example shows different aspects of button interaction in Telium applications.