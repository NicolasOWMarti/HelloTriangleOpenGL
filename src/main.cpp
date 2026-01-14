#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>

#include <filesystem>

std::string loadShaderSource(const char* filepath){ // funksjon som importerer text filer som en streng
    std::ifstream file(filepath); // åpner textfilen og lagrer den i en var som heter file
    if(!file.is_open()){ // sjekker om textfilen ble åpnet
        std::cerr << "ERROR: failed to open shader file: " << filepath << std::endl;
        return "";
    }

    // retunerer file som buffer
    std::stringstream buffer; 
    buffer << file.rdbuf();
    return buffer.str();
}

void framebuffer_size_callback(GLFWwindow* window, int wigth, int height){
    glViewport(0, 0, wigth, height);
}

void processInput(GLFWwindow *window){ // tar inn hvilken vindu som parameter
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ // sjekker om esc tasten er trykket
        glfwSetWindowShouldClose(window, true); // gjør som at vinduet skal lukkes
    }
}


int main() {
    std::cout << "working directory:" << std::filesystem::current_path() << std::endl;

    if (!glfwInit()) { // starter opp GLFW
    std::cout << "Failed to initialize GLFW" << std::endl;
    return -1;
    }
    
    // setter GLFW setings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "HelloTriangle", NULL, NULL); // lager et window objekt
    if (window == NULL){ // sjekker om noe gikk galt med GLFW
        std::cout << "failed to make GLFW window" << std::endl; // er det samme som print() i .py og console.log i .js
        glfwTerminate(); // skrur av GLFW
        return -1; // fortellet pc-en at noe har gått galt
    }
    glfwMakeContextCurrent(window); // forteller openGL hvilket vindu den skal tegne til.

    glewExperimental = GL_TRUE; // loader alle mulige GLEW funksjoner
    if (glewInit() != GLEW_OK){ // starte opp GLEW og sjekker at det funker
        std::cout << "failed to initialize GLEW" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600); // for teller openGL hvor stort vinduet den skal rendre er

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "max vertex attribs: " << nrAttributes << std::endl;


    //-----compiler Vertex Shader-----
    std::cout << "does shader.vert exist?" << std::filesystem::exists("../src/shaders/shader.vert") << std::endl;
    std::cout << "does shader.frag exist?" << std::filesystem::exists("../src/shaders/shader.frag") << std::endl;

    std::string vertexShaderCode = loadShaderSource("../src/shaders/shader.vert"); // henter vertex shader koden som streng fra "shader/shader.vert"
    const char* vertexShaderSource = vertexShaderCode.c_str(); // endrer vertex shader koden fra streng type til char type

    unsigned int vertexShader; // lager en var som skal bevare vertex shaderen
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // gjør som at variabelen kan bevare vertex shaderen
    glShaderSource(vertexShader, 1, &vertexShaderSource , NULL); // fester shader koden til shader objektet
    glCompileShader(vertexShader);

    int success; // lager en var som skal sjekke om shaderen ble compilet riktig
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success); // fetcher compile status

    if(!success){ // printer ut feil komentar hvis noe gikk galt
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //-----compiler Fragment shader-----
    std::string fragmentShaderCode = loadShaderSource("../src/shaders/shader.frag"); // henter fragment shader koden som streng fra "shader/shader.frag"
    const char* fragmentShaderSource = fragmentShaderCode.c_str(); // endrer fragment shader koden fra streng type til char type

    unsigned int fragmentShader; // lager en var som skal bevare vertex shaderen
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // gjør som at variabelen kan bevare vertex shaderen
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // fester shader koden til shader objektet
    glCompileShader(fragmentShader);

     glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // fetcher compile status

    if (!success){ // printer ut feil komentar hvis noe gikk galt
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    if (vertexShaderCode.empty() || fragmentShaderCode.empty()){
        std::cout << "shader source missing. quiting code." << std::endl;
        return -1;
    }
    //-----kombinerer shaders til et program-----
    unsigned int shaderProgram; // laget en variabel for hele shader programmet
    shaderProgram = glCreateProgram();  // gjør variablet shader klar

    glAttachShader(shaderProgram, vertexShader); // legger til vertex shader
    glAttachShader(shaderProgram, fragmentShader); // legger til fragment shader
    glLinkProgram(shaderProgram); // ferdig gjør shader programmet

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // fetcher link status
    if(!success){ // printer en feil melding om noe er galt
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // sletter vertex shader og fragment shader for å spare plass
    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader);  


    float vertices[] = { // definerer vertex data som normalized device coordinates (NDC).
    //positions             //color
    0.0f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,   // top
    0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f    // bottom left
    };
    unsigned int indices[] = { // definerer hvilket vertex skal bli koblet til hvilket vertex
        0, 1, 2,
    };


    // VBOs (vertex buffer objects) er en liten del av GPU minne som oppbevarer data i float nummere.
    // for at openGL skal kunne bruke denne dataen så må du fortelle openGL hvordan man leser dataen og hvordan man bruker dataen. 
    // dette bruker man VAOs (vertex array objects) for.

    // ----- lager VAO og VBO: -----
    unsigned int VAO; // definerer en vertex array object (VAO) som bevarer informasjon om hvordam vertexene skal settes sammen.
    unsigned int VBO; // definerer en vertex buffer object (VBO) som bevarer vertex data på GPU memory.
    unsigned int EBO; // definerer en ellement buffer object (EBO) som forteller GPUen hvordan man setter sammen vertex dataen

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); 
    glGenBuffers(1, &EBO);

    // ----- binder VAO og VBO: -----
    glBindVertexArray(VAO); // binde VAO som at jeg kan skrive til VAOen
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //binder VBOen som en GL_ARRAY_BUFFER objekt. 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // binder EBOen som en GL_ELEMENT_ARRAY_BUFFER objekt.
    
    // -----skriver til VBO og VAO: -----
    glBufferData( // uploader vertex dataen til VBOen.
        GL_ARRAY_BUFFER,    
        sizeof(vertices),   // data size
        vertices,           // data
        GL_STATIC_DRAW 
    );   

    glVertexAttribPointer( // uploader info om hvordan man leser VBOen til VAOen.
        0,                  // attribute location
        3,                  // 3 floats per vertex
        GL_FLOAT,           // type
        GL_FALSE,           // normalization
        6 * sizeof(float),  // stride
        (void*)0 
    );         // offset

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3* sizeof(float))
    );
    
    glBufferData(   // uploader data til EBOen.
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );
    
    // ----- starter opp VBO(0): -----
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // render loop:
    while (!glfwWindowShouldClose(window)){ // fortsetter så lenge vinduet er opent
        // input:
        processInput(window);
        
        // rendering commands:
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO); // denne binder VAO en og VBOen OG EBOen!!!!!!
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // check and call events and swap the buffers
        glfwSwapBuffers(window); // bytter frame buffers
        glfwPollEvents(); // hondterer input events og vindu størrelses endringer
    }

    // sletter alt før man slutter programmet for å unngå noen problemer
    std::cout << "yey, everything works" << std::endl;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}