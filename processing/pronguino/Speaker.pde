/**
 * Speaker
 *
 * Define classes for internal (PC) and external (Arduino UNO/Piezo) speakers
 *
 * @author  Neil Deighan
 */

 // Import minim sound library
import ddf.minim.*;

/**
 * Interface that defines required methods
 */
interface Speaker {
   void playWallSound();
   void playPaddleSound();
   void playPointSound();
}

/**
 * Implmentation for external speaker  
 */
class ExternalSpeaker implements Speaker {
   
  Console parent;
  /**
   * Class Constructor
   */
  ExternalSpeaker(Console parent) {
    this.parent = parent;
  }
  
  void playWallSound() {
    this.parent.write(Constants.DATA_BIT_WALL_SOUND);
  }
  
  void playPaddleSound() {
    this.parent.write(Constants.DATA_BIT_PADDLE_SOUND);
  }
  
  void playPointSound() {
    this.parent.write(Constants.DATA_BIT_POINT_SOUND);
  } 
}

/**
 * Implmentation for internal speaker  
 */
class InternalSpeaker implements Speaker {

  Minim minim;
  AudioPlayer wallSound;
  AudioPlayer paddleSound;
  AudioPlayer pointSound;

  InternalSpeaker(PApplet parent) {
    this.minim = new Minim(parent);
    this.wallSound = minim.loadFile("WallSound.wav");
    this.paddleSound = minim.loadFile("PaddleSound.wav");
    this.pointSound = minim.loadFile("PointSound.wav");    
  }
   
  void play(AudioPlayer audioPlayer) {
    audioPlayer.rewind();
    audioPlayer.play();
  }
   
  void playWallSound() {
    this.play(wallSound);
  }
   
  void playPaddleSound() {
    this.play(paddleSound);
  }
  
  void playPointSound() {
    this.play(pointSound);
  } 
}
 
 
