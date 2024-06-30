#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"
#include "camera_controller.hpp"
#include "ray_tracing.hpp"
#include "path_tracing.hpp"
#include <string>
#include <glut.h>
#include "NEE.hpp"
using namespace std;
// 本文件自行实现


SceneParser *sceneParser;
CameraController *cameraController;
int imgW, imgH;
string savePicturePath;

void screenCapture() {
    Image openglImg(imgW, imgH);
    auto *pixels = new unsigned char[3 * imgW * imgH];
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, imgW, imgH, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    for (int x = 0; x < imgW; ++x) {
        for (int y = 0; y < imgH; ++y) {
            Vector3f color(
                    pixels[3 * (y * imgW + x) + 0] / 255.0,
                    pixels[3 * (y * imgW + x) + 1] / 255.0,
                    pixels[3 * (y * imgW + x) + 2] / 255.0);
            openglImg.SetPixel(x, y, color);
        }
    }
    openglImg.SaveImage(savePicturePath.c_str());
    delete[] pixels;
    cout << "Current viewport captured at " << savePicturePath << "." << endl;
}

//  Called when mouse button is pressed.
void mouseFunc(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {

        switch (button) {
            case GLUT_LEFT_BUTTON:
                cameraController->mouseClick(CameraController::LEFT, x, y);
                break;
            case GLUT_MIDDLE_BUTTON:
                cameraController->mouseClick(CameraController::MIDDLE, x, y);
                break;
            case GLUT_RIGHT_BUTTON:
                cameraController->mouseClick(CameraController::RIGHT, x, y);
            default:
                break;
        }
    } else {
        cameraController->mouseRelease(x, y);
    }
    glutPostRedisplay();
}

// Called when mouse is moved while button pressed.
void motionFunc(int x, int y) {
    cameraController->mouseDrag(x, y);
    glutPostRedisplay();
}

// Called when the window is resized
void reshapeFunc(int w, int h) {
    sceneParser->getCamera()->resize(w, h);
    glViewport(0, 0, w, h);
    imgW = w;
    imgH = h;
}

// This function is responsible for displaying the object.
void drawScene() {
    Vector3f backGround = sceneParser->getBackgroundColor();
    glClearColor(backGround.x(), backGround.y(), backGround.z(), 1.0);

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setup MODELVIEW Matrix
    sceneParser->getCamera()->setupGLMatrix();

    // Turn On all lights.
    for (int li = 0; li < sceneParser->getNumLights(); ++li) {
        Light *light = sceneParser->getLight(li);
        light->turnOn(li);
    }
    // Draw elements.
    Group *baseGroup = sceneParser->getGroup();
    baseGroup->drawGL();

    // Dump the image to the screen.
    glutSwapBuffers();

    // Save if not in interactive mode.
    if (!savePicturePath.empty()) {
        screenCapture();
        exit(0);
    }
}

float getCenterDepth() {
    Camera *cam = sceneParser->getCamera();
    Ray centerRay = sceneParser->getCamera()->generateRay(
            Vector2f((float) cam->getWidth() / 2, (float) cam->getHeight() / 2));
    Hit hit;
    bool isHit = sceneParser->getGroup()->intersect(centerRay, hit, 0.0);
    return isHit ? hit.getT() : 10.0f;
}

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 4 && argc != 3) {
        cout << "Usage: PA2 <input scene file> [output image file] [mode: pathtracing / NEE]" << endl;
        cout << "  If the output path is not specified, program will enter interactive mode." << endl;
        return 1;
    }

    string inputFile = argv[1];
    string outputFile = argv[2];

    string code = argv[3];

    if(code == "pathtracing"){
        // std::cout << "alsdfkaslkfjasd" << std::endl;
        PT pathtracing(1000);
        pathtracing.render(inputFile, outputFile);
    }
    else if(code == "NEE"){
        // std::cout << "1235412351" << std::endl;
        NEE nee;
        nee.render(inputFile, outputFile);

    }

    return 0;

    
}

