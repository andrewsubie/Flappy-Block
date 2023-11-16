#include "engine.h"


enum state {start, play, over};
state screen = start;
// colors
const color skyBlue(77/255.0, 213/255.0, 240/255.0);
const color grassGreen(26/255.0, 176/255.0, 56/255.0);
const color white(1, 1, 1);
const color darkBlue(1/255.0, 110/255.0, 214/255.0);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color magenta(1, 0, 1);
const color cyan (0, 1, 1);

Engine::Engine() : keys() {
    this->initWindow();
    this->initShaders();
    this->initShapes();
}

Engine::~Engine() {}

unsigned int Engine::initWindow(bool debug) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(width, height, "engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    return 0;
}

void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert", "../res/shaders/shape.frag",  nullptr, "shape");

    textShader = shaderManager->loadShader("../res/shaders/text.vert", "../res/shaders/text.frag",nullptr, "text");
    fontRenderer = make_unique<FontRenderer>(shaderManager->getShader("text"), "../res/fonts/MxPlus_IBM_BIOS.ttf", 24);

    // Set uniforms that never change
    shapeShader.use();
    shapeShader.setMatrix4("projection", this->PROJECTION);

   
}

void Engine::initShapes() {
    // init user
    user = make_unique<Rect>(shapeShader, vec2(width/2, 450), vec2(20, 20), white);

    // Init grass
    grass = make_unique<Rect>(shapeShader, vec2(width/2, 50), vec2(width, height / 3), grassGreen);




    int totalRecWidth = 0;
    vec2 buildingSize;

    // Populate top rectangles
    totalRecWidth = 0;
    while (totalRecWidth < width + 100) {
        buildingSize.y = rand() % 51 + 150;
        buildingSize.x = rand() % 51 + 50;
        topRec.push_back(make_unique<Rect>(shapeShader,
                                               vec2(totalRecWidth + (buildingSize.x / 2.0) + 5,
                                                    ((buildingSize.y / 2.0) + 50)),
                                               buildingSize, darkBlue));
        totalRecWidth += buildingSize.x + 5;
    }
    // Populate bottom rectangles
    totalRecWidth = 0;
    while (totalRecWidth < width + 200) {
        buildingSize.y = rand() % 301 + 200;
        buildingSize.x = rand() % 101 + 50;
        bottomRec.push_back(make_unique<Rect>(shapeShader,
                                               vec2(totalRecWidth + (buildingSize.x / 2.0) + 5,
                                                    (height)),
                                               buildingSize, purple));
        totalRecWidth += buildingSize.x + 5;
    }
}

void Engine::processInput() {
    glfwPollEvents();

    // Set keys to true if pressed, false if released
    for (int key = 0; key < 1024; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            keys[key] = true;
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
            keys[key] = false;
    }

    // Close window if escape key is pressed
    // Mouse position saved to check for collisions
    glfwGetCursorPos(window, &MouseX, &MouseY);

    // Update mouse rect to follow mouse
    MouseY = height - MouseY; // make sure mouse y-axis isn't flipped

    user->setPosX(width / 2);
    if (keys[GLFW_KEY_SPACE])
        user->moveY(10.0f);
    if (keys[GLFW_KEY_S] && screen == start)
        screen = play;
    if(keys[GLFW_KEY_R] && screen == over)
        screen = start;


    // for loops to change color of rectangles pass, increase score
    for (const unique_ptr<Rect> &r: topRec) {
        if (r->getRight() < user->getLeft() && screen == play) {
            r->setColor(cyan);
            score++;
        } else {
            r->setColor(darkBlue);
        }
    }
    for (const unique_ptr<Rect> &r: bottomRec) {
        if (r->getRight() < user->getLeft() && screen == play) {
            r->setColor(magenta);
            score++;
        } else {
            r->setColor(purple);
        }
    }
    // for loops to end game if user collides with the rectangles
    for (const unique_ptr<Rect> & r: bottomRec){
        if (screen == play){
            if (r->isOverlapping(*user)){
                screen = over;
                user->setPos(vec2(width/2, 450));
            }
        }
    }
    for (const unique_ptr<Rect> & r: topRec){
        if (screen == play){
            if (r->isOverlapping(*user)){
                screen = over;
                user->setPos(vec2(width/2, 350));
            }
        }
    }


}

void Engine::update() {
    float linearMotion;
    if (screen == play){
        linearMotion = -2.0;
    }
    else{
        linearMotion = 0.0;
    }
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

// moving user to simulate gravity
    user->moveY(-3.0f);

// moving rectangles to simulate linear motion
    for (int i = 0; i < topRec.size(); ++i) {
        topRec[i]->moveX(linearMotion);
        if (topRec[i]->getPosX() < -(topRec[i]->getSize().x/2)) {
            int buildingOnLeft = (topRec[i] == topRec[0]) ? topRec.size()-1 : i - 1;
            topRec[i]->setPosX(topRec[buildingOnLeft]->getPosX() + topRec[buildingOnLeft]->getSize().x/2 + topRec[i]->getSize().x/2 + 5);
        }
    }
    for (int i = 0; i < bottomRec.size(); ++i) {
        bottomRec[i]->moveX(linearMotion);
        if (bottomRec[i]->getPosX() < -(bottomRec[i]->getSize().x/2)) {
            int buildingOnLeft = (bottomRec[i] == bottomRec[0]) ? bottomRec.size()-1 : i - 1;
            bottomRec[i]->setPosX(bottomRec[buildingOnLeft]->getPosX() + bottomRec[buildingOnLeft]->getSize().x/2 + bottomRec[i]->getSize().x/2 + 5);
        }
    }
}

void Engine::render() {
    glClearColor(skyBlue.red, skyBlue.green, skyBlue.blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shapeShader.use();
    switch (screen) {
        case start: {
            string message = "Welcome to Flappy Egg";
            string instructions = "Press S to start";
            string instructions1= "to turn them off";
            string instructions2 = "Release Version: 0.0.0";
            // (12 * message.length()) is the offset to center text.
            // 12 pixels is the width of each character scaled by 1.
            this->fontRenderer->renderText(message, 125 , 450, 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(instructions, 125 , 340, 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(instructions1, 125 , 300, 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(instructions2, 125 , 150, 1, vec3{1, 1, 1});
            break;

        }
        case play: {
            string display = "Score: " + std::to_string(score);
            grass->setUniforms();
            grass->draw();

            for (const unique_ptr<Rect>& b : bottomRec){
                b-> setUniforms();
                b->draw();
            }
            for (const unique_ptr<Rect>& b : topRec){
                b-> setUniforms();
                b->draw();
            }

            user -> setUniforms();
            user -> draw();
            this->fontRenderer->renderText(display,width-300, 25, 0.75, vec3{1,1,1});
            break;
        }

        case over: {
            string message = "Game Over!";
            string message2 = "Your score was: " + std::to_string(score);
            string message3 = "All-time high score: " + std::to_string(writeToFile());
            string message4 = "Press R to go home";

            this->fontRenderer->renderText(message, 125 , 450, 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(message2, 125 , 340, 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(message3, 125 , 300, 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(message4, 125 , 260, 1, vec3{1, 1, 1});
            break;
        }
    }
    glfwSwapBuffers(window);
}

unsigned int Engine::writeToFile() {
    string oldScore;
    std::ifstream inFile (R"(C:\Users\subac\CLionProjects\M4OEP-asubach\src\framework\data\scores.txt)");
    inFile >> oldScore;
    int oldScoreInt = std::stoi(oldScore);
    if (score > oldScoreInt){
        std::ofstream outFile (R"(C:\Users\subac\CLionProjects\M4OEP-asubach\src\framework\data\scores.txt)");
        outFile << std::to_string(score) + '\n' << std::endl;
        outFile.close();
        return score;
    }
    else{
        return oldScoreInt;
    }
}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}