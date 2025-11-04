#pragma once

#include "Model.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Game{
    Model3D::Model3D(void): m_Position(0.0f) , m_Rotation(0.0f) , m_Scale(1.0f,1.0f,1.0f) , m_HasChange(true) , m_ModelMatrix(glm::mat4(1.0f)) {

    }

    void Model3D::SetPosition(const glm::vec3& position){ m_Position = position; m_HasChange = true; }
    void Model3D::SetRotation(const glm::vec3& rotation){ m_Rotation = rotation; m_HasChange = true; }
    void Model3D::SetScale   (const glm::vec3& scale)   { m_Scale    = scale   ; m_HasChange = true; }

    glm::vec3 Model3D::GetPosition(void) { return m_Position; }
    glm::vec3 Model3D::GetRotation(void) { return m_Rotation; }
    glm::vec3 Model3D::GetScale   (void) { return m_Scale   ; }

    glm::mat4& Model3D::GetModelMatrix(void) {
        CalculateMatrix();
        return m_ModelMatrix;
    }

    void Model3D::CalculateMatrix(void){
        if (!m_HasChange)
            return;
        
        glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f) , glm::radians( m_Rotation.x ) , glm::vec3(1.0f , 0.0f , 0.0f) );
        glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f) , glm::radians( m_Rotation.y ) , glm::vec3(0.0f , 1.0f , 0.0f) );
        glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f) , glm::radians( m_Rotation.z ) , glm::vec3(0.0f , 0.0f , 1.0f) );
        glm::mat4 transl  = glm::translate(glm::mat4(1.0f) , m_Position);
        glm::mat4 scale   = glm::scale(glm::mat4(1.0f) , m_Scale);
        glm::mat4 rotate  = rotateZ * rotateY * rotateX ;
        m_ModelMatrix = transl * rotate * scale; 
        
    }
}