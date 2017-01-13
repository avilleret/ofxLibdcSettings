#include "ofApp.h"

void ofApp::setup()
{
    camera.setup();

    // settings.setup() should be called after camera.setup()
    settings.setup(camera.getLibdcCamera());

    // setup a gui to hold the settings folder
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    gui->addHeader("voxelstrack");
    gui->addFolder(settings.parameters);
}

void ofApp::update()
{
    if(camera.grabVideo(curFrame)) {
        curFrame.update();
    }
}

void ofApp::draw()
{
    if(camera.isReady()) {
        curFrame.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
}

void ofApp::exit()
{
}

// define some keyboard shortcuts :
// ESC : exit
// h : show/hide GUI
// l : load settings
// s : save settings
void ofApp::keyPressed(int key){
  switch (key){
    case 27:
      ofExit();
      break;
    case 'h':
      gui->setAutoDraw(!gui->getAutoDraw());
      break;
    case 's':
      settings.save();
      break;
    case 'l':
      settings.load();
      break;
    default:
      break;
  }
}
