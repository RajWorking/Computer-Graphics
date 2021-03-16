#include "ball.h"
#include "main.h"
#include <vector>
using namespace std;

Ball::Ball(float x, float y, int type)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    int numVertices, numTriangles;
    vector<GLfloat> vertices;
    vector<unsigned int> triangles;
    switch (type)
    {
    case 1:
        vertices = {
            0.0f,
            0.0f,
            1.0f,
            0.866f,
            0.5f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            -0.866f,
            0.5f,
            0.0f,
            -0.866f,
            -0.5f,
            0.0f,
            0.0f,
            -1.0f,
            0.0f,
            0.866f,
            -0.5f,
            0.0f,
            0.0f,
            0.0f,
            -1.0f,
        };
        triangles = {
            1,
            2,
            0,
            2,
            3,
            0,
            3,
            4,
            0,
            4,
            5,
            0,
            5,
            6,
            0,
            6,
            1,
            0,
            1,
            2,
            7,
            2,
            3,
            7,
            3,
            4,
            7,
            4,
            5,
            7,
            5,
            6,
            7,
            6,
            1,
            7,
        };
        numVertices = 8, numTriangles = 12;
        break;

    case 2:
        vertices = {
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            0.0f,
            0.0f,
            0.841254f,
            0.540641f,
            0.0f,
            0.415415f,
            0.909632f,
            0.0f,
            -0.142315f,
            0.989821f,
            0.0f,
            -0.654861f,
            0.75575f,
            0.0f,
            -0.959493f,
            0.281733f,
            0.0f,
            -0.959493f,
            -0.281733f,
            0.0f,
            -0.654861f,
            -0.75575f,
            0.0f,
            -0.142315f,
            -0.989821f,
            0.0f,
            0.415415f,
            -0.909632f,
            0.0f,
            0.841254f,
            -0.540641f,
            0.0f,
        };
        triangles = {
            1,
            2,
            0,
            2,
            3,
            0,
            3,
            4,
            0,
            4,
            5,
            0,
            5,
            6,
            0,
            6,
            7,
            0,
            7,
            8,
            0,
            8,
            9,
            0,
            9,
            10,
            0,
            10,
            11,
            0,
            11,
            1,
            0,
            2,
            3,
            1,
            3,
            4,
            1,
            4,
            5,
            1,
            5,
            6,
            1,
            6,
            7,
            1,
            7,
            8,
            1,
            8,
            9,
            1,
            9,
            10,
            1,
            10,
            11,
            1,
        };
        numVertices = 12, numTriangles = 20;
        break;

    case 3:
        vertices = {
            1.0f,
            0.0f,
            0.5f,
            0.309017f,
            0.951057f,
            0.5f,
            -0.809017f,
            0.587785f,
            0.5f,
            -0.809017f,
            -0.587785f,
            0.5f,
            0.309017f,
            -0.951057f,
            0.5f,
            0.809017f,
            0.587785f,
            -0.5f,
            -0.309017f,
            0.951057f,
            -0.5f,
            -1.0f,
            0.0f,
            -0.5f,
            -0.309017f,
            -0.951057f,
            -0.5f,
            0.809017f,
            -0.587785f,
            -0.5f,
        };
        triangles = {
            1,
            2,
            0,
            2,
            3,
            0,
            3,
            4,
            0,
            6,
            7,
            5,
            7,
            8,
            5,
            8,
            9,
            5,
            0,
            1,
            5,
            1,
            2,
            6,
            2,
            3,
            7,
            3,
            4,
            8,
            4,
            5,
            9,
            5,
            6,
            1,
            6,
            7,
            2,
            7,
            8,
            3,
            8,
            9,
            4,
            9,
            5,
            0,
        };
        numVertices = 10, numTriangles = 16;
        break;

    default:
        vertices = {
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
        };
        triangles = {
            0,
            1,
            2,
        };
        numVertices = 3, numTriangles = 1;
    }

    GLfloat vertex_buffer_data[vertices.size()];
    copy(vertices.begin(), vertices.end(), vertex_buffer_data);

    unsigned int indices[triangles.size()];
    copy(triangles.begin(), triangles.end(), indices);

    GLfloat *color_buffer_data = new GLfloat[3 * numVertices];
    for (int i = 0; i < numVertices; i++)
    {
        color_buffer_data[3 * i] = rand() / double(RAND_MAX);
        color_buffer_data[3 * i + 1] = rand() / double(RAND_MAX);
        color_buffer_data[3 * i + 2] = rand() / double(RAND_MAX);
    }

    color_buffer_data[0] = 255;
    color_buffer_data[1] = 0;
    color_buffer_data[2] = 0;
    color_buffer_data[3] = 0;
    color_buffer_data[4] = 0;
    color_buffer_data[5] = 255;


    this->object = create3DObject(GL_TRIANGLES, numVertices, vertex_buffer_data, numTriangles, indices, color_buffer_data, GL_FILL);
}

void Ball::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
}

void Ball::move(int pn, int dir)
{
    this->position[dir] += pn * 0.05;
}

void Ball::tick()
{
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
