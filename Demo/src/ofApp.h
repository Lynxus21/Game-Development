#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    std::string directionIndicator; // "Left", "Right", or "Forward"

    // Define the block properties
    ofBoxPrimitive block; // 3D block
    glm::vec3 blockPosition;

    // Easy camera for 3D navigation
    ofEasyCam camera;

     std::string lastTurnDirection;

    // For displaying text
    ofTrueTypeFont font;

    // For displaying tokens and counting
    ofSpherePrimitive token; // Token object
    glm::vec3 tokenPosition;

     bool isPizzaDeliveryActive = false;

    // Pizza acquired token
    ofSpherePrimitive pizzaAcquiredToken; // Pizza acquired token object
    glm::vec3 pizzaAcquiredTokenPosition;

    // Pizza delivery location token
    ofSpherePrimitive pizzaDeliveryLocation; // Pizza delivery location token object
    glm::vec3 pizzaDeliveryLocationPosition;

    //GPS System variables
    float pizzaDeliveryDirectionAngle; // Angle to the pizza delivery location
       glm::vec3 forwardDirection; // Direction in which the block is facing
       glm::vec3 toPizzaDelivery;  // Direction to the pizza delivery location

    int interactionCount = 0; // Count of interactions

    // Declaring blockWidth
    float blockWidth; // Width of the block

        std::string debugText;

    // Car variables
    float speed;
    float acceleration;  // Declare acceleration
    float deceleration;  // Declare deceleration
    float maxSpeed;      // Declare maxSpeed

    // Person variables
    float personSpeed;
    float personAcceleration;
    float personMaxSpeed;

    // Pizza pickup and delivery location variables
    glm::vec3 pizzaAcquiredPosition;
    glm::vec3 deliveryLocationPosition;

    // Vector to store "Person" blocks
    std::vector<ofBoxPrimitive> persons;

    // Camera control variables
    bool cameraFollowsCar;

    // Flag to check if interaction has occurred with the orange token
    bool orangeTokenInteractionOccurred = false;

    // Add this variable to keep track of token visibility
    bool isOrangeTokenVisible = true;

    // Flag to prevent continuous key press
    bool keyPressedFlag = false;

    // Variable to store the time of orange token interaction
    unsigned long long interactionStartTime;

    void drawSceneObjects();
    void drawHUD();

    // Function declarations for drawing car and person scenes
    void drawCarScene();
    void drawPersonScene();

    // Function declaration for random pizza location
    void generateRandomPizzaDeliveryLocation();

        std::vector<glm::vec3> fixedTokenPositions;
        std::vector<ofBoxPrimitive> houses; // Vector to store house objects
           std::vector<glm::vec3> housePositions; // Vector to store house positions

    // Event handlers
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
};
