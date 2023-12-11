#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
    // Setup the GL context
    // Can be OF_WINDOW or OF_FULLSCREEN
    // Pass in width and height too
    ofSetupOpenGL(1024, 768, OF_WINDOW);

    // This kicks off the running of my app
    // Can be OF_WINDOW or OF_FULLSCREEN
    // Pass in width, height, and mode (OF_WINDOW or OF_FULLSCREEN)
    ofRunApp(new ofApp());
}

