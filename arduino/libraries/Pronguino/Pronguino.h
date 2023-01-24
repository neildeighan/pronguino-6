/*
   Basic.h - Library Header file Pronguino.

   @author Neil Deighan
*/

// Safe guards against including more than once
#ifndef PRONGUINO_H
#define PRONGUINO_H

// Include the Basic Library
#include "Component.h"

class Controller {
  public:
    Controller();  
    Controller(int potPin, int switchPin, int ledPin);
    int readValue();
    int readButtonState();
    void writeLedState(int state);
  private:
    Potentiometer _pot;
    Switch _switch;
    LED _led;
};

class Speaker {
  public:
    Speaker();
    Speaker(int piezoPin);
    void wallSound();
    void paddleSound();
    void pointSound();
  private:
    Piezo _piezo;
};

class GameConsole {
  public:
    GameConsole();
    GameConsole(int piezoPin);
    Controller* controllers();   
    Speaker speaker();
    void addController(int potPin, int switchPin, int ledPin);   // Return controller added ? 
  private:
    Speaker _speaker;
    int _controllerCount;
    Controller* _controllers;
};

#endif
