#pragma once
#include <Defs.hpp>
#include <glm/glm.hpp>

namespace Game{
    class Camera{
        public:
            virtual ~Camera(void);

            virtual glm::mat4& GetView(void) = 0;

    };

    class UVRCamera: public Camera{

        public:
            UVRCamera(const glm::vec3& position = glm::vec3(0,0,0.1f));
            
            virtual glm::mat4& GetView(void) override;

            void SetPosition(const glm::vec3& position);

            void MoveUp(float step);
            void MoveDown(float step);

            void MoveRight(float step);
            void MoveLeft(float step);

            void MoveFront(float step);
            void MoveBack(float step);

            void RotateRight(float degrees);
            void RotateLeft(float degrees);
            
            void RotateUp(float degrees);
            void RotateDown(float degrees);

            
        private:
            glm::vec3   m_ViewNormal;
            glm::vec3   m_UpNormal  ;
            glm::mat4   m_ViewMat   ;
            bool        m_Update    ; 
    };
}