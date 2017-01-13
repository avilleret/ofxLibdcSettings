#include "ofApp.h"

int main()
{
  ofGLFWWindowSettings settings;
  settings.width = 1280;
  settings.height = 960;
  settings.title = "ofxLibdcSettings example";

  auto window = ofCreateWindow(settings);
  auto app = make_shared<ofApp>();
  ofRunApp(window, app);

  return ofRunMainLoop();
}
