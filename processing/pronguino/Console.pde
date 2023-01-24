/**
 * Console
 *
 * Uses factory pattern to create different types of console, one for PC(Internal), one for UNO(External)    
 *
 * @author  Neil Deighan
 */

// TODO: Needs a bit of refactoring

/**
 * Interface that defines what methods should be included for console
 */
interface Console {
  Speaker speaker();  
  void inputEvent(Object input);
  void write(int data);
  Controller addController(Player player);
}

/**
 * Abstract class for console 
 */
abstract class AbstractConsole implements Console {
  // as console is declared with interface Console
  // in main sketch, this property is effectively
  // private, and only exposed using speaker() method
  Speaker speaker;

  // Return speaker
  Speaker speaker() {
    return this.speaker;
  }
  
  // Methods that need to be implemented ...
  abstract void write(int data); 
  abstract void inputEvent(Object input);
  abstract Controller addController(Player player);
}

/**
 * Factory class that creates and returns instance of 
 * relevant console class  
 */
class ConsoleFactory {
  
  ConsoleFactory() {
  }
  
  // Create console depending on port connection ..
  Console createConsole(PApplet parent, Serial port) {
    if(port == null) { // .. not connected ...
      return new InternalConsole(parent);
    } else { // .. connected ..
      return new ExternalConsole(port);     
    }
  } 
}

/**
 * Concrete class for internal console i.e. computer/Processing app
 */
class InternalConsole extends AbstractConsole {
 
  /**
   * Constrctor
   *
   * @param  parent  PApplet needed for minim sound library
   */
  InternalConsole(PApplet parent) {
    this.speaker = new InternalSpeaker(parent);
  }
  
  void inputEvent(Object input) {
    //TODO: Send in key here on Keypressed ?
  }
  
  void write(int data) {
    //TODO: Nothing to do here at moment
  }

  // Just return null for controllers each, not really relevant at the moment
  Controller addController(Player player) {
    return null;
  }  
}

/**
 * Concrete class for external console i.e. Arduino UNO
 */
class ExternalConsole extends AbstractConsole {

 ArrayList<Controller> controllers = new ArrayList<Controller>();
 boolean connected;
 Serial port;
 
  /**
   * Constructor
   *
   * @param  port  connected serial port
   */
  ExternalConsole(Serial port) {
    this.port = port;
    this.speaker = new ExternalSpeaker(this);
    this.connected = true;
  }
  
  /**
   * Called from serialEvent in main sketch
   *
   * @param  input  used Object type, currently will send Serial, 
   *                but may use "key" if implemented in internal console
   */
  void inputEvent(Object input) {

    // Is there any data available ?
    while (this.port.available() > 0) {
      // Get the data from port
      byte data = (byte)this.port.read();
  
      // Split data into two nibbles .. one for each players controller    
      int highNibble = Functions.getHighNibble(data);
      int lowNibble = Functions.getLowNibble(data);
  
      this.setController(Constants.PLAYER_ONE, highNibble);
      this.setController(Constants.PLAYER_TWO, lowNibble);
  
      // Small delay before getting any further data, 
      // this is quite a crucial delay, see what happens 
      // when reduced to say 10 ... really threw me when I 
      // was trying to get paddle to move with controller! 
      delay(100);
    } 

    // Call no Serial Event
    noSerialEvent();
  }

  /**
   * Writes data to serial port 
   */
  void write(int data) {
    this.port.write(data);
    // Small delay to allow processing
    delay(10);
  }

  /**
   * Adds controller to console
   *
   * @param    player  player associated with controller
   * 
   * @returns  new controller
   */
  Controller addController(Player player) {
    Controller controller = new Controller(player);
    this.controllers.add(controller);
    return controller;
  } //<>//
 
  /**
   * set value of controller from serial read (inputEvent)
   */
  void setController(int playerId, int nibble) {
    // Set the value of each controller (first 3 bits of nibble ... 0-7)
    players[playerId].controller.setValue(nibble & Constants.DATA_BITS_VALUE);    
  
    // Set the state of each controllers button (4th or most significant bit of each nibble ... 0-1)
    players[playerId].controller.button.setState((nibble & Constants.DATA_BITS_STATE) >> 3); 
  }
  
}
