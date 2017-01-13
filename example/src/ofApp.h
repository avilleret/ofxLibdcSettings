#pragma once

#include "ofMain.h"
#include "ofxLibdc.h"
#include "ofxLibdcSettings.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp
{
  public:
    void setup  ();
    void update ();
    void draw   ();
    void exit   ();
    void keyPressed(int key);

  private:
    // ofxLibdc is a namespace, and Camera is the object
    ofxLibdc::Camera camera;

    // ofImage is where we store the current frame we grabbed from the Camera
    ofImage curFrame;

    ofxLibdcSettings settings;

    ofxDatGui *gui;
};
