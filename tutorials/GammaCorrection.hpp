/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef GAMMA_CORRECTION_CPP
#define GAMMA_CORRECTION_CPP

namespace GammaCorrection{
//
GLfloat planeVertices[] = {
    // Positions          // Normals         // Texture Coords
     8.0f, -0.5f,  8.0f,  0.0f, 1.0f, 0.0f,  5.0f, 0.0f,
    -8.0f, -0.5f,  8.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
    -8.0f, -0.5f, -8.0f,  0.0f, 1.0f, 0.0f,  0.0f, 5.0f,

     8.0f, -0.5f,  8.0f,  0.0f, 1.0f, 0.0f,  5.0f, 0.0f,
    -8.0f, -0.5f, -8.0f,  0.0f, 1.0f, 0.0f,  0.0f, 5.0f,
     8.0f, -0.5f, -8.0f,  0.0f, 1.0f, 0.0f,  5.0f, 5.0f
};
//
glm::vec3 lightPos = glm::vec3(0.0, 0.0, 0.0);
bool blinn = true;
bool lock = false;
bool gamma = true;
int attenIndex = 1;
//
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
    CameraController::keyCallback(window, key, scancode, action, mode);
    if(key == GLFW_KEY_B && action == GLFW_PRESS){
        //切换Shading模式
        blinn = !blinn;
        cout<<"Switched to "<<(blinn?"Blin Phong":"Pure Phong")<<" Mode"<<endl;
    }
    if(key == GLFW_KEY_L && action == GLFW_PRESS){
        //锁定视角方向
        lock = !lock;
        if(lock){
            glfwSetCursorPosCallback(window, nullptr);
        }else{
            glfwSetCursorPosCallback(window, CameraController::mouseCallback);
        }
    }
    if(key == GLFW_KEY_G && action == GLFW_PRESS){
        //开启/关闭 gamma矫正
        gamma = !gamma;
        cout<<"Gamma Correction : "<<(gamma?"ON":"OFF")<<endl;
    }
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        //
        attenIndex = (attenIndex == 1 ? 2 : 1);
        cout<<"Attenuation Function: "<<(attenIndex == 1 ? "Linear" : "Quadratic")<<endl;
    }
}
void tutorial(){
    GLFWwindow *window = initWindow("GammaCorrection", 800, 600);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, CameraController::mouseCallback);
    showEnviroment();
    Camera* pCamera = CameraController::getCamera();
    pCamera->moveto(glm::vec3(0.0f, 1.0f, 3.0f));
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    //
    Shader shader("Resources/Shaders/GammaCorrection/plane.vs", "Resources/Shaders/GammaCorrection/plane.frag");
    shader.use();
    glUniform3f(glGetUniformLocation(shader.programID, "vLight.position"), lightPos.x, lightPos.y, lightPos.z);
    //
    CoordinateAxes ca(pCamera);
    //
    Shape plane(planeVertices, 6, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    //
    Texture tex0("Resources/Textures/wood.jpg", GL_BGR, GL_SRGB);
    Texture tex1("Resources/Textures/wood.jpg", GL_BGR, GL_RGB);
    //
    cout<<"Switched to "<<(blinn?"Blin Phong":"Pure Phong")<<" Mode"<<endl;
    cout<<"Gamma Correction : "<<(gamma?"ON":"OFF")<<endl;
    cout<<"Attenuation Function: "<<(attenIndex == 1 ? "Linear" : "Quadratic")<<endl;
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        if(gamma){
            tex0.use();
        }else{
            tex1.use();
        }
        //
        shader.use();
        glUniform1i(glGetUniformLocation(shader.programID, "blinn"), blinn);
        glUniform1i(glGetUniformLocation(shader.programID, "attenIndex"), attenIndex);
        if(gamma){
            glUniform1f(glGetUniformLocation(shader.programID, "gamma"), 2.2f);
        }else{
            glUniform1f(glGetUniformLocation(shader.programID, "gamma"), 1.0f);
        }
        plane.draw(&shader, pCamera);
        //
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

};
#endif
