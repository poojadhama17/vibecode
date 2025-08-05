// Telium Button Click Examples

// Example 1: Basic button with click handler
const { Button, Box, Text } = require('telium');

const basicButton = new Button({
  label: 'Click Me!',
  onClick: () => {
    console.log('Button clicked!');
  }
});

// Example 2: Button with state management
const statefulButton = new Button({
  label: 'Toggle State',
  onClick: (button) => {
    const currentState = button.getState('toggled') || false;
    button.setState('toggled', !currentState);
    button.setLabel(currentState ? 'Toggle State' : 'Toggled!');
  }
});

// Example 3: Button with async operations
const asyncButton = new Button({
  label: 'Load Data',
  onClick: async (button) => {
    button.setLabel('Loading...');
    button.setDisabled(true);
    
    try {
      // Simulate API call
      await new Promise(resolve => setTimeout(resolve, 2000));
      console.log('Data loaded successfully!');
      button.setLabel('Data Loaded');
    } catch (error) {
      console.error('Error loading data:', error);
      button.setLabel('Error - Retry');
    } finally {
      button.setDisabled(false);
    }
  }
});

// Example 4: Button with custom styling and events
const styledButton = new Button({
  label: 'Styled Button',
  style: {
    border: 'double',
    borderColor: 'blue',
    backgroundColor: 'cyan',
    color: 'black'
  },
  onClick: (button, event) => {
    console.log('Button clicked with event:', event);
    // Change button appearance on click
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

// Example 5: Button group with shared state
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
    new Button({
      label: 'Option 2',
      onClick: (button) => {
        buttonGroup.setState('selected', 'option2');
        updateButtonGroup(buttonGroup);
      }
    }),
    new Button({
      label: 'Option 3',
      onClick: (button) => {
        buttonGroup.setState('selected', 'option3');
        updateButtonGroup(buttonGroup);
      }
    })
  ]
});

function updateButtonGroup(group) {
  const selected = group.getState('selected');
  group.children.forEach((button, index) => {
    const isSelected = (index === 0 && selected === 'option1') ||
                      (index === 1 && selected === 'option2') ||
                      (index === 2 && selected === 'option3');
    
    button.setStyle({
      backgroundColor: isSelected ? 'green' : 'gray',
      color: isSelected ? 'white' : 'black'
    });
  });
}

// Example 6: Button with keyboard shortcuts
const keyboardButton = new Button({
  label: 'Press Enter or Click',
  onClick: () => {
    console.log('Button activated!');
  }
});

// Add keyboard event listener
keyboardButton.on('keypress', (event) => {
  if (event.key === 'Enter' || event.key === ' ') {
    keyboardButton.emit('click');
  }
});

// Example 7: Conditional button rendering
function createConditionalButton(condition) {
  if (!condition) return null;
  
  return new Button({
    label: 'Conditional Button',
    onClick: () => {
      console.log('Conditional button clicked!');
    }
  });
}

// Example 8: Button with confirmation dialog
const confirmButton = new Button({
  label: 'Delete Item',
  style: {
    backgroundColor: 'red',
    color: 'white'
  },
  onClick: async (button) => {
    const confirmed = await showConfirmationDialog('Are you sure you want to delete?');
    if (confirmed) {
      console.log('Item deleted!');
      button.setLabel('Deleted');
      button.setDisabled(true);
    }
  }
});

async function showConfirmationDialog(message) {
  // This would typically use Telium's dialog system
  // For now, we'll simulate it
  return new Promise((resolve) => {
    console.log(message + ' (y/n)');
    // In a real implementation, you'd show a proper dialog
    resolve(true); // Simulate user clicking "yes"
  });
}

// Example 9: Button with progress indicator
const progressButton = new Button({
  label: 'Start Process',
  onClick: async (button) => {
    const steps = ['Initializing...', 'Processing...', 'Finalizing...', 'Complete!'];
    
    for (let i = 0; i < steps.length; i++) {
      button.setLabel(steps[i]);
      await new Promise(resolve => setTimeout(resolve, 1000));
    }
    
    button.setStyle({ backgroundColor: 'green' });
  }
});

// Example 10: Button with tooltip
const tooltipButton = new Button({
  label: 'Hover Me',
  tooltip: 'This button has a helpful tooltip!',
  onClick: () => {
    console.log('Tooltip button clicked!');
  }
});

// Export examples for use in other files
module.exports = {
  basicButton,
  statefulButton,
  asyncButton,
  styledButton,
  buttonGroup,
  keyboardButton,
  createConditionalButton,
  confirmButton,
  progressButton,
  tooltipButton
};