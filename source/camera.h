#include "main.h"

#ifndef CAMERA_H
#define CAMERA_H


class Camera
{
    public:
    glm::vec3 Eye;
    glm::vec3 Target;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    int rot_speed = 1;

    Camera()
    {
        Eye =  glm::vec3(0.0f, 0.0f, 10.0f);
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        Up = WorldUp;
        updateCamera(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    void updateCamera(glm::vec3 target)
    {
        Target = target;
        glm::vec3 Front = Target - Eye;
        Right = glm::normalize(glm::cross(Front, Up));
        Up    = glm::normalize(glm::cross(Right, Front));
    }

    void posEye(int type)
    {
        switch (type)
        {
        case 1:
            Eye = glm::vec3(10.0f, 0.0f, 0.0f);
            break;
        case 2:
            Eye = glm::vec3(1.0f, 10.0f, 1.0f);
            break;
        case 3:
            Eye = glm::vec3(0.0f, 0.0f, 10.0f);
            break;
        default:
            std::cout<<"Unknown type\n";
        }
    }

    void moveCamera(int pn, int dir) {
        
        glm::vec3 move_upd = glm::vec3(0.0f, 0.0f, 0.0f);

        if(dir == 0)
        {
            if(pn>0)
                move_upd = glm::normalize(Right);
            else
                move_upd = -glm::normalize(Right);
        }
        else if(dir == 1)
        {
            if(pn>0)
                move_upd = glm::normalize(Up);
            else
                move_upd = -glm::normalize(Up);
        }
        else if(dir == 2)
        {
            if(pn>0)
                move_upd = -glm::normalize(Target - Eye);
            else
                move_upd = glm::normalize(Target - Eye);
        }

        move_upd = move_upd * 0.05f;

        Target += move_upd;
        Eye += move_upd;

        updateCamera(Target);
    }

    void rotateCamera()
    {
        glm::mat4 rotationMat(1);
        rotationMat = glm::rotate(rotationMat, 0.05f, Up);

        glm::vec3 Front = Target - Eye;
            
        Front = glm::vec3(rotationMat * glm::vec4(Front, 1.0));

        Eye = Target - Front;
        Right = glm::normalize(glm::cross(Front, Up));
    }

    glm::mat4 getViewMatrix()
    {
        glm::mat4 mat = glm::lookAt(this->Eye, this->Target, this->Up);        
        return mat;
    }

    // void aboutMe()
    // {
    //     std::cout<<"Cam:\n";
    //     std::cout<<"\n";
    // }

    void printVec(glm::vec3 vec, char* about)
    {
        std::cout<<about<<" "<<vec.x<<" "<<vec.y<<" "<<vec.z<<"\n";
    }


};


#endif