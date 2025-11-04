#include "Projection.hpp"

/*==================================================================================
/*==================================================================================*/
/*==================================================================================*/
/*==================================================================================
/*==================================================================================*/
/*==================================================================================*/
/*==================================================================================
/*==================================================================================*/
/*==================================================================================*/
/*==================================================================================
/*==================================================================================*/
/*==================================================================================*/

#include <glm/gtc/matrix_transform.hpp>

namespace Game{
    Perspective3D::Perspective3D(void){
        m_Fov = 60;
        m_AspectRatio = 1.0f;
        m_ZNear = 0.1f;
        m_ZFar  = 100.0f;
        m_Update = true;
    }
    Perspective3D::Perspective3D(const Perspective3D& copy){
        m_Fov           = copy.m_Fov        ;
        m_AspectRatio   = copy.m_AspectRatio;
        m_ZNear         = copy.m_ZNear      ;
        m_ZFar          = copy.m_ZFar       ;
        m_Update        = copy.m_Update     ;
        m_Projection    = copy.m_Projection ;
    }
            

    Perspective3D::~Perspective3D(void) {}

    glm::mat4& Perspective3D::GetProjection(void) const {
        if (!m_Update)
            return m_Projection;

        m_Update = false;
        m_Projection = glm::perspective(glm::radians(m_Fov) , m_AspectRatio , m_ZNear , m_ZFar);
        return m_Projection;
    }

    void Perspective3D::Set(float fov , float aspectRatio , float znear , float zfar ){
        ASSERT(aspectRatio > 0 , "Aspect ratio is <= 0 ");
        m_Fov           = fov           ;
        m_AspectRatio   = aspectRatio   ;
        m_ZNear         = znear         ;
        m_ZFar          = zfar          ;
        m_Update        = true          ;
        
    }

    float Perspective3D::GetFov(void)           const { return m_Fov            ; }
    float Perspective3D::GetZFar(void)          const { return m_ZFar           ; }
    float Perspective3D::GetZNear(void)         const { return m_ZNear          ; }
    float Perspective3D::GetAspectRatio(void)   const { return m_AspectRatio    ; }
}