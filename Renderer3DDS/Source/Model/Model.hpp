#pragma once
#include <Defs.hpp>
#include <glm/glm.hpp>


namespace Game{

    class Model{
        public:
            Model(void) = default;
            virtual ~Model(void) = default;

            virtual glm::mat4& GetModelMatrix(void) = 0;
    };

    class Model3D : Model{
        public:
            Model3D(void);
            virtual ~Model3D(void) = default;

            void SetPosition(const glm::vec3& position);
            void SetRotation(const glm::vec3& rotation);
            void SetScale   (const glm::vec3& scale)   ;

            glm::vec3 GetPosition(void) ;
            glm::vec3 GetRotation(void) ;
            glm::vec3 GetScale   (void) ;

            virtual glm::mat4& GetModelMatrix(void) override;

        private:
            glm::vec3 m_Position;
            glm::vec3 m_Rotation;
            glm::vec3 m_Scale;
            bool      m_HasChange;
            glm::mat4 m_ModelMatrix;   
        private:
            void CalculateMatrix(void); 
    };
}