// Complete Telium Application with Button Examples

const { App, Box, Button, Text, Input } = require('telium');

class MyTeliumApp extends App {
  constructor() {
    super();
    this.counter = 0;
    this.isLoading = false;
  }

  render() {
    return new Box({
      direction: 'vertical',
      padding: 1,
      children: [
        new Text({
          content: 'Telium Button Examples',
          style: {
            color: 'cyan',
            bold: true,
            align: 'center'
          }
        }),
        
        // Basic counter button
        new Button({
          label: `Counter: ${this.counter}`,
          onClick: () => {
            this.counter++;
            this.update();
          }
        }),
        
        // Async loading button
        new Button({
          label: this.isLoading ? 'Loading...' : 'Load Data',
          disabled: this.isLoading,
          onClick: async () => {
            this.isLoading = true;
            this.update();
            
            try {
              await this.simulateDataLoad();
              console.log('Data loaded successfully!');
            } catch (error) {
              console.error('Error loading data:', error);
            } finally {
              this.isLoading = false;
              this.update();
            }
          }
        }),
        
        // Button with input interaction
        new Box({
          direction: 'horizontal',
          children: [
            new Input({
              placeholder: 'Enter text...',
              width: 20,
              onKeyPress: (input, event) => {
                if (event.key === 'Enter') {
                  this.handleInputSubmit(input.getValue());
                  input.setValue('');
                }
              }
            }),
            new Button({
              label: 'Submit',
              onClick: () => {
                const input = this.findChild('input');
                if (input) {
                  this.handleInputSubmit(input.getValue());
                  input.setValue('');
                }
              }
            })
          ]
        }),
        
        // Navigation buttons
        new Box({
          direction: 'horizontal',
          children: [
            new Button({
              label: 'Previous',
              onClick: () => {
                console.log('Previous clicked');
                this.navigate('prev');
              }
            }),
            new Button({
              label: 'Next',
              onClick: () => {
                console.log('Next clicked');
                this.navigate('next');
              }
            })
          ]
        }),
        
        // Styled action buttons
        new Box({
          direction: 'horizontal',
          children: [
            new Button({
              label: 'Save',
              style: {
                backgroundColor: 'green',
                color: 'white'
              },
              onClick: () => {
                console.log('Saving...');
                this.saveData();
              }
            }),
            new Button({
              label: 'Delete',
              style: {
                backgroundColor: 'red',
                color: 'white'
              },
              onClick: async () => {
                const confirmed = await this.showConfirmation('Are you sure you want to delete?');
                if (confirmed) {
                  console.log('Deleting...');
                  this.deleteData();
                }
              }
            })
          ]
        })
      ]
    });
  }

  async simulateDataLoad() {
    return new Promise((resolve) => {
      setTimeout(resolve, 2000);
    });
  }

  handleInputSubmit(value) {
    if (value.trim()) {
      console.log('Submitted:', value);
      // Process the input value
    }
  }

  navigate(direction) {
    console.log(`Navigating ${direction}`);
    // Handle navigation logic
  }

  saveData() {
    console.log('Data saved successfully!');
  }

  deleteData() {
    console.log('Data deleted successfully!');
  }

  async showConfirmation(message) {
    // In a real Telium app, you'd show a proper dialog
    console.log(message + ' (y/n)');
    return new Promise((resolve) => {
      // Simulate user confirmation
      resolve(true);
    });
  }

  update() {
    this.render();
  }
}

// Event-driven button handling
class EventDrivenApp extends App {
  constructor() {
    super();
    this.events = [];
  }

  render() {
    return new Box({
      direction: 'vertical',
      children: [
        new Text({
          content: 'Event-Driven Button Example',
          style: { bold: true, align: 'center' }
        }),
        
        new Button({
          label: 'Add Event',
          onClick: () => {
            this.addEvent('Button clicked at ' + new Date().toLocaleTimeString());
          }
        }),
        
        new Button({
          label: 'Clear Events',
          onClick: () => {
            this.clearEvents();
          }
        }),
        
        new Box({
          direction: 'vertical',
          children: this.events.map(event => 
            new Text({ content: event })
          )
        })
      ]
    });
  }

  addEvent(event) {
    this.events.push(event);
    this.update();
  }

  clearEvents() {
    this.events = [];
    this.update();
  }

  update() {
    this.render();
  }
}

// Usage example
if (require.main === module) {
  const app = new MyTeliumApp();
  app.run();
}

module.exports = {
  MyTeliumApp,
  EventDrivenApp
};