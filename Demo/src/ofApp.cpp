#include "ofApp.h"

// Function definition
void ofApp::generateRandomPizzaDeliveryLocation() {
    pizzaDeliveryLocationPosition = glm::vec3(ofRandom(-2000, 2000), 0, ofRandom(-50, 50));
    cout << "Yellow token position: " << pizzaDeliveryLocationPosition << endl;

    // Set the y-coordinate of the acquired token to be equal to the floor's y-coordinate
    pizzaAcquiredTokenPosition.y = 0;
    // Set the y-coordinate of the delivery location token to be equal to the floor's y-coordinate
    pizzaDeliveryLocationPosition.y = 0;

}

//--------------------------------------------------------------
void ofApp::setup() {
    // Initialize block properties
       blockWidth = 50;
       // Set the initial position of the block away from (0, 0, 0)
       blockPosition = glm::vec3(ofRandom(-200, 200), 0, ofRandom(-200, 200));

    // Initialize with a default position
        pizzaDeliveryLocationPosition = glm::vec3(-1000, 100, -1000); // Some off-screen or default position


    // Initialize car controls
    speed = 0;
    acceleration = 0.1;
    deceleration = 0.2;
    maxSpeed = 5;

    // Set the initial position of the block away from (0, 0, 0)
    blockPosition = glm::vec3(ofRandom(-200, 200), 0, ofRandom(-200, 200));

    // Initialize person controls
    personSpeed = 0;
    personAcceleration = 0.1;
    personMaxSpeed = 2;

    // Test positions for known angle calculation
       blockPosition = glm::vec3(0, 0, 0); // Set block at origin
       pizzaDeliveryLocationPosition = glm::vec3(100, 0, 100); // Place pizza location at a known position

    // Set up the camera
    camera.setAutoDistance(false);
    camera.disableMouseInput();

    // Initialize token properties
       token.setRadius(20);
       tokenPosition = glm::vec3(100, 0, 100);
       token.setPosition(tokenPosition.x, tokenPosition.y, tokenPosition.z);

       // Initialize pizzaAcquiredToken properties
       pizzaAcquiredToken.setRadius(20);
       // Set a fixed position for pizzaAcquiredToken far from the block's initial position
       pizzaAcquiredTokenPosition = glm::vec3(500, 0, 500);

       // Initialize pizzaDeliveryLocation
       pizzaDeliveryLocation.setRadius(20);
       // Initialize with a default position off-screen
       pizzaDeliveryLocationPosition = glm::vec3(-1000, 0, -1000);

       // Set initial state for the orange token interaction
       orangeTokenInteractionOccurred = false;

    // Start by following the car
    cameraFollowsCar = true;
}


//--------------------------------------------------------------
void ofApp::update() {
    glm::vec3 oldPosition = blockPosition;
    float distanceBehind = 300.0; // Adjust this value as needed
    float distanceAbove = 150.0;   // Adjust this value as needed

    if (cameraFollowsCar) {
        if (ofGetKeyPressed(OF_KEY_LEFT)) {
            block.rotateDeg(1, glm::vec3(0, 1, 0));
        }

        if (ofGetKeyPressed(OF_KEY_RIGHT)) {
            block.rotateDeg(-1, glm::vec3(0, 1, 0));
        }

        glm::vec3 forwardDirection = block.getLookAtDir();

        if (ofGetKeyPressed(OF_KEY_UP)) {
            speed += acceleration;
            speed = ofClamp(speed, 0, maxSpeed);
            blockPosition += forwardDirection * speed;
        } else if (ofGetKeyPressed(OF_KEY_DOWN)) {
            speed -= acceleration;
            speed = ofClamp(speed, -maxSpeed, maxSpeed);
            blockPosition += forwardDirection * speed;
        } else {
            speed *= 0.95;
            blockPosition += forwardDirection * speed;
        }

        if (abs(speed) < 0.05) {
            speed = 0;
        }

        block.setPosition(blockPosition.x, blockPosition.y, blockPosition.z);

        // Check interaction with orange token first
        if (!orangeTokenInteractionOccurred && glm::distance(blockPosition, pizzaAcquiredTokenPosition) < (blockWidth / 2 + pizzaAcquiredToken.getRadius())) {
            orangeTokenInteractionOccurred = true;
            interactionStartTime = ofGetElapsedTimeMillis();
            pizzaAcquiredTokenPosition = glm::vec3(0, -1000, 0);
            pizzaDeliveryLocationPosition.y = blockPosition.y;
            generateRandomPizzaDeliveryLocation();
            isPizzaDeliveryActive = true;
        }

        // Check interaction with yellow token next
        if (orangeTokenInteractionOccurred && glm::distance(blockPosition, pizzaDeliveryLocationPosition) < (blockWidth / 2 + pizzaDeliveryLocation.getRadius())) {
            interactionCount++;
            pizzaDeliveryLocationPosition = glm::vec3(0, -1000, 0);
            pizzaAcquiredTokenPosition = glm::vec3(0, blockPosition.y, 0);
            orangeTokenInteractionOccurred = false;
            generateRandomPizzaDeliveryLocation();
            isPizzaDeliveryActive = false;
        }

        // Update the target position to follow the car
        glm::vec3 targetPosition = blockPosition - block.getLookAtDir() * distanceBehind + glm::vec3(0, distanceAbove, 0);
        camera.setPosition(targetPosition.x, targetPosition.y, targetPosition.z);
        camera.lookAt(blockPosition);
    } else {
        for (auto& person : persons) {
            if (ofGetKeyPressed(OF_KEY_LEFT)) {
                person.rotateDeg(1, glm::vec3(0, 1, 0));
            }

            if (ofGetKeyPressed(OF_KEY_RIGHT)) {
                person.rotateDeg(-1, glm::vec3(0, 1, 0));
            }

            glm::vec3 forwardDirection = person.getLookAtDir();

            if (ofGetKeyPressed(OF_KEY_UP)) {
                personSpeed += personAcceleration;
                personSpeed = ofClamp(personSpeed, 0, personMaxSpeed);
                person.setPosition(person.getPosition() + forwardDirection * personSpeed);
            } else if (ofGetKeyPressed(OF_KEY_DOWN)) {
                personSpeed -= personAcceleration;
                personSpeed = ofClamp(personSpeed, -personMaxSpeed, personMaxSpeed);
                person.setPosition(person.getPosition() + forwardDirection * personSpeed);
            }
        }

        // Reset the orange token interaction flag after a certain duration
        if (orangeTokenInteractionOccurred && ofGetElapsedTimeMillis() - interactionStartTime >= 2000) {
            orangeTokenInteractionOccurred = false;
        }
    }
    // Member variables for hysteresis and smoothing
    float lastDirectionChangeTime = 0;
    float directionChangeDelay = 1.0; // delay in seconds
    std::string lastDirectionIndicator = "";

    if (isPizzaDeliveryActive) {
            glm::vec3 toPizzaDelivery = glm::normalize(pizzaDeliveryLocationPosition - blockPosition);
            glm::vec3 forwardDirection = glm::normalize(block.getLookAtDir());

            // Calculate the angle in degrees between forward direction and toPizzaDelivery
            float angle = glm::degrees(acos(glm::dot(forwardDirection, toPizzaDelivery)));
            // Determine the sign of the angle
            float crossProductY = glm::cross(forwardDirection, toPizzaDelivery).y;

            // Use angular thresholds to determine direction
            if (angle < 15) {  // Small angle: Go forward
                directionIndicator = "Forward";
            } else if (angle > 165) {  // Nearly opposite: Turn around
                directionIndicator = "Turn Around";
            } else {
                // Check the sign for left or right
                if (crossProductY > 0) {
                    directionIndicator = "Left";
                } else {
                    directionIndicator = "Right";
                }
            }

            // Implementing hysteresis and smoothing
            float currentTime = ofGetElapsedTimef();
            if (directionIndicator != lastDirectionIndicator) {
                if (currentTime - lastDirectionChangeTime > directionChangeDelay) {
                    lastDirectionChangeTime = currentTime;
                    lastDirectionIndicator = directionIndicator;
                } else {
                    directionIndicator = lastDirectionIndicator;
                }
            }
        }
}

//--------------------------------------------------------------
void ofApp::draw() {
    camera.begin(); // Begin 3D camera

    // Draw a grid on the floor
    ofPushMatrix();
    ofRotateXDeg(0); // Rotate to lay flat
    ofDrawGrid(500, 10, false, false, true, false); // Draw a 500x500 grid
    ofPopMatrix();

    // Draw the block
    ofSetColor(ofColor::blue);
    block.drawWireframe(); // Draw the block as a wireframe

    // Draw the token
    ofSetColor(ofColor::green);
    token.draw();

    // Draw the pizzaAcquiredToken
    ofSetColor(ofColor::orange);
    if (!orangeTokenInteractionOccurred) {
        pizzaAcquiredToken.setPosition(pizzaAcquiredTokenPosition.x, pizzaAcquiredTokenPosition.y, pizzaAcquiredTokenPosition.z);
        pizzaAcquiredToken.draw();
    }

    // Draw the pizzaDeliveryLocation
    ofSetColor(ofColor::yellow);
    if (orangeTokenInteractionOccurred) {
        pizzaDeliveryLocation.setPosition(pizzaDeliveryLocationPosition.x, pizzaDeliveryLocationPosition.y, pizzaDeliveryLocationPosition.z);
        pizzaDeliveryLocation.draw();
    }

    glm::vec3 forwardVec = block.getLookAtDir() * 100.0f; // Increase length
      glm::vec3 lineStart = blockPosition + block.getLookAtDir() * blockWidth; // Offset by block's width
      glm::vec3 lineEnd = lineStart + forwardVec;
      ofDrawLine(lineStart, lineEnd);

    // Draw the person block
    ofSetColor(ofColor::red);
    for (auto& person : persons) {
        person.drawWireframe();
    }

    camera.end(); // End 3D camera

    // Draw the GPS Arrow with adjusted angle for 2D overlay
      int boxSize = 50;
      int boxX = ofGetWidth() - boxSize - 20;
      int boxY = 20;

      if (isPizzaDeliveryActive) {
             // Draw the direction indicator
             ofDrawBitmapString("Direction to Delivery: " + directionIndicator, 10, 40);
         }

      ofSetColor(0, 0, 0, 128);
      ofDrawRectangle(boxX, boxY, boxSize, boxSize);

      ofPushMatrix();
      ofTranslate(boxX + boxSize / 2, boxY + boxSize / 2);

      // Adjust the angle for 2D overlay
      float adjustedAngle = -pizzaDeliveryDirectionAngle + 90; // Invert and shift by 90 degrees
      // Normalize the angle to [0, 360] range
      adjustedAngle = fmod(adjustedAngle, 360.0f);
      if (adjustedAngle < 0) adjustedAngle += 360;

      ofRotateDeg(adjustedAngle); // Apply the adjusted angle
      ofSetColor(255, 255, 255);
      ofDrawLine(0, 0, 10, 0); // Arrow shaft
      ofDrawLine(10, 0, 5, -5); // Arrowhead left
      ofDrawLine(10, 0, 5, 5); // Arrowhead right
      ofPopMatrix();

    // Display message when pizza is picked up
    if (orangeTokenInteractionOccurred) {
        std::string pizzaPickedUpText = "Pizza Picked Up!";
        ofSetColor(ofColor::orange);
        int textWidth = font.stringWidth(pizzaPickedUpText);
        ofDrawBitmapString(pizzaPickedUpText, ofGetWidth() / 2 - textWidth / 2, ofGetHeight() / 2);

        // Draw the debug text
            ofDrawBitmapString(debugText,ofGetWidth() / 2 - textWidth / 2, ofGetHeight() / 2);
    }
}

//--------------------------------------------------------------

void ofApp::keyPressed(int key) {
    if (key == 'E' || key == 'e') {
        cout << "E key pressed" << endl;

        if (cameraFollowsCar) {
            // Switch to person controls and camera
            cameraFollowsCar = false;

            if (persons.empty()) {
                // Spawn the person if not already spawned
                glm::vec3 personPosition = blockPosition;
                personPosition.x += 30.0;

                ofBoxPrimitive person;
                person.set(20);
                person.setPosition(personPosition.x, personPosition.y, personPosition.z);

                persons.push_back(person);
            }
        } else {
            // Switch back to car controls and camera
            cameraFollowsCar = true;

            // Despawn the person
            persons.clear();
        }
    }

    if (cameraFollowsCar) {
        if (key == OF_KEY_LEFT) {
            block.rotateDeg(1, glm::vec3(0, 1, 0));
        } else if (key == OF_KEY_RIGHT) {
            block.rotateDeg(-1, glm::vec3(0, 1, 0));
        } else if (key == OF_KEY_UP) {
            speed += acceleration;
            speed = ofClamp(speed, 0, maxSpeed);
            blockPosition += block.getLookAtDir() * speed;
        } else if (key == OF_KEY_DOWN) {
            speed -= acceleration;
            speed = ofClamp(speed, -maxSpeed, maxSpeed);
            blockPosition += block.getLookAtDir() * speed;
        }
    } else {
        if (key == OF_KEY_LEFT) {
            for (auto& person : persons) {
                person.rotateDeg(1, glm::vec3(0, 1, 0));
            }
        } else if (key == OF_KEY_RIGHT) {
            for (auto& person : persons) {
                person.rotateDeg(-1, glm::vec3(0, 1, 0));
            }
        } else if (key == OF_KEY_UP) {
            for (auto& person : persons) {
                personSpeed += personAcceleration;
                personSpeed = ofClamp(personSpeed, 0, personMaxSpeed);
                person.setPosition(person.getPosition() + person.getLookAtDir() * personSpeed);
            }
        } else if (key == OF_KEY_DOWN) {
            for (auto& person : persons) {
                personSpeed -= personAcceleration;
                personSpeed = ofClamp(personSpeed, -personMaxSpeed, personMaxSpeed);
                person.setPosition(person.getPosition() + person.getLookAtDir() * personSpeed);
            }
        }
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    if (key == 'e' || key == 'E') {
        keyPressedFlag = false;
    }
}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
