#pragma once

#include <Defs.hpp>
#include <glm/glm.hpp>

namespace Game{
    class Projection{

        public:
            virtual ~Projection(void) = default;

            virtual glm::mat4& GetProjection(void) const = 0;


    };

    class Perspective3D : public Projection{

        public:
            Perspective3D(void);
            Perspective3D(const Perspective3D& copy);
            // move constructor have no point here

            virtual ~Perspective3D(void) ;

            virtual glm::mat4& GetProjection(void) const override;

            void Set(float fov , float aspectRatio , float znear , float zfar );

            float GetFov(void)          const;
            float GetZFar(void)         const;
            float GetZNear(void)        const;
            float GetAspectRatio(void)  const;

        private:
            float               m_Fov           ;
            float               m_AspectRatio   ;
            float               m_ZNear         ;
            float               m_ZFar          ;
            mutable glm::mat4   m_Projection    ;
            mutable bool        m_Update        ; 
    };
}