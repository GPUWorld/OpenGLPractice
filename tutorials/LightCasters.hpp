/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef LIGHT_CASTERS_CPP
#define LIGHT_CASTERS_CPP

namespace LightCasters{
//顶点信息前置声明
extern GLfloat cubeVertices[36*8];
//位置信息前置声明
extern glm::vec3 cubePositions[10];
//光照颜色(光源颜色)
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
//光照的方向
glm::vec3 lightDir( -0.2f, -1.0f, -0.3f);

//教程1:实现 定向光(光线平行且在无限远处)
void tutorialDirectionalLight(){
    //创建窗口
    GLFWwindow *window = initWindow("LightCasters",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    Camera *pCamera = CameraController::getCamera();
    //着色器程序初始化
    Shader cubeShader("Resources/Shaders/LightCasters/cubeDirectionalLight.vs","Resources/Shaders/LightCasters/cubeDirectionalLight.frag");
    cubeShader.use();
    glUniform1f(glGetUniformLocation(cubeShader.programID, "material.shininess"), 32.0f);
    glUniform1i(glGetUniformLocation(cubeShader.programID, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(cubeShader.programID, "material.specular"),1);

    glm::vec3 diffuseColor = glm::vec3(0.5f); // Decrease the influence
    glm::vec3 ambientColor = glm::vec3(0.2f); // Low influence
    glm::vec3 specularColor = glm::vec3(1.0f); // Low influence
    glUniform3f(glGetUniformLocation(cubeShader.programID, "light.direction"), lightDir.x, lightDir.y, lightDir.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "light.specular"), specularColor.x, specularColor.y, specularColor.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
    //物体对象初始化
    vector<Shape> cubes;
    for(int i = 0; i < 10; ++i){
        cubes.push_back(Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES));
    }
    //绑定控制
    CameraController::bindControl(window);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(pCamera);
    ca.showGrid(false);
    //帧数计数器

    //载入纹理
    Texture tex0("Resources/Textures/container2.png", GL_BGRA, GL_RGBA);
    Texture tex1("Resources/Textures/container2_specular.png", GL_BGRA, GL_RGBA);
    //
    glm::mat4 tmpModel;
    GLfloat angle;
    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ca.draw();

        tex0.use(0);
        tex1.use(1);
        for(int i = 0; i < 10; ++i){
            tmpModel = glm::translate(glm::mat4(), cubePositions[i]);
            angle = 20.0f * i;
            tmpModel = glm::rotate(tmpModel, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            cubes[i].setModelMat(tmpModel);
            cubes[i].draw(&cubeShader, pCamera);
        }
        glfwSwapBuffers(window);

    }
    glfwDestroyWindow(window);
    glfwTerminate();
}


//教程2:实现 点光源
glm::vec3 lightPos(1.2f, 1.5f, 2.0f);
void tutorialPointLight(){
    //创建窗口
    GLFWwindow *window=initWindow("LightCasters",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0);
    Camera *pCamera = CameraController::getCamera();
    //着色器程序初始化
    Shader cubeShader("Resources/Shaders/LightCasters/cubePointLight.vs","Resources/Shaders/LightCasters/cubePointLight.frag");
    Shader lampShader("Resources/Shaders/LightCasters/lampPointLight.vs","Resources/Shaders/LightCasters/lampPointLight.frag");
    cubeShader.use();

    glUniform1f(glGetUniformLocation(cubeShader.programID, "material.shininess"), 32.0f);
    glUniform1i(glGetUniformLocation(cubeShader.programID, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(cubeShader.programID, "material.specular"),1);

    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Decrease the influence
    glm::vec3 ambientColor = lightColor * glm::vec3(0.2f); // Low influence
    glm::vec3 specularColor = lightColor * glm::vec3(1.0f); // Low influence
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vLight.position"), lightPos.x,lightPos.y,lightPos.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vLight.specular"), specularColor.x,specularColor.y,specularColor.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vLight.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "vLight.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
    glUniform1f(glGetUniformLocation(cubeShader.programID, "vLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(cubeShader.programID, "vLight.linear"), 0.09);
    glUniform1f(glGetUniformLocation(cubeShader.programID, "vLight.quadratic"), 0.032);


    lampShader.use();
    GLint lampColorLoc=glGetUniformLocation(lampShader.programID,"lampColor");
    glUniform3f(lampColorLoc,lightColor.x,lightColor.y,lightColor.z);
    //物体对象初始化
    vector<Shape> cubes;
    for(int i = 0; i < 10; ++i){
        cubes.push_back(Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES));
    }
    //
    Shape lamp(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    lamp.moveTo(lightPos);
    lamp.scaleTo(Vec3(0.4));
    //绑定控制
    CameraController::bindControl(window);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(pCamera);
    ca.showGrid(false);
    //帧数计数器

    //载入纹理
    Texture tex0("Resources/Textures/container2.png", GL_BGRA, GL_RGBA);
    Texture tex1("Resources/Textures/container2_specular.png", GL_BGRA, GL_RGBA);
    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ca.draw();

        lamp.draw(&lampShader, pCamera);

        tex0.use(0);
        tex1.use(1);
        for(int i = 0; i < 10; ++i){
            cubes[i].draw(&cubeShader, pCamera);
        }

        glfwSwapBuffers(window);

    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

//教程3:实现 聚光灯灯源(实现一个跟随玩家视觉移动的手电筒)
void tutorialSpotLight(){
    //创建窗口
    GLFWwindow *window=initWindow("LightCasters",800,600);
    showEnviroment();
    glEnable(GL_DEPTH_TEST);
    Camera *pCamera = CameraController::getCamera();
    //着色器程序初始化
    Shader cubeShader("Resources/Shaders/LightCasters/cubeSpotLight.vs","Resources/Shaders/LightCasters/cubeSpotLight.frag");
    cubeShader.use();
    glUniform1f(glGetUniformLocation(cubeShader.programID, "material.shininess"), 32.0f);
    glUniform1i(glGetUniformLocation(cubeShader.programID, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(cubeShader.programID, "material.specular"),1);

    glm::vec3 diffuseColor = glm::vec3(0.5f); // Decrease the influence
    glm::vec3 ambientColor = glm::vec3(0.2f); // Low influence
    glm::vec3 specularColor = glm::vec3(1.0f); // Low influence
    glUniform1f(glGetUniformLocation(cubeShader.programID, "light.innerCutOff"), glm::cos(glm::radians(12.5f)));
    glUniform1f(glGetUniformLocation(cubeShader.programID, "light.outerCutOff"), glm::cos(glm::radians(17.5f)));
    glUniform3f(glGetUniformLocation(cubeShader.programID, "light.specular"), specularColor.x, specularColor.y, specularColor.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
    glUniform3f(glGetUniformLocation(cubeShader.programID, "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
    glUniform1f(glGetUniformLocation(cubeShader.programID, "light.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(cubeShader.programID, "light.linear"), 0.09);
    glUniform1f(glGetUniformLocation(cubeShader.programID, "light.quadratic"), 0.032);
    //物体对象初始化
    vector<Shape> cubes;
    for(int i = 0; i < 10; ++i){
        cubes.push_back(Shape(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES));
    }
    //绑定控制
    CameraController::bindControl(window);
    //关闭鼠标显示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //显示坐标轴
    CoordinateAxes ca(pCamera);
    ca.showGrid(false);
    //帧数计数器

    //载入纹理
    Texture tex0("Resources/Textures/container2.png", GL_BGRA, GL_RGBA);
    Texture tex1("Resources/Textures/container2_specular.png", GL_BGRA, GL_RGBA);
    //
    glm::mat4 tmpModel;
    GLfloat angle;
    //主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        ca.draw();

        tex0.use(0);
        tex1.use(1);
        for(int i = 0; i < 10; ++i){
            tmpModel = glm::translate(glm::mat4(), cubePositions[i]);
            angle = 20.0f * i;
            tmpModel = glm::rotate(tmpModel, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            cubes[i].setModelMat(tmpModel);
            cubes[i].draw(&cubeShader, pCamera);
        }

        glfwSwapBuffers(window);

    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
//位置信息
glm::vec3 cubePositions[10] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

//顶点信息
GLfloat cubeVertices[36*8] = {
    // Positions           // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

}
#endif  //  LIGHT_CASTERS
