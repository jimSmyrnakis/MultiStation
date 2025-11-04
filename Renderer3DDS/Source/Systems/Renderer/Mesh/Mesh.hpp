#pragma once 

#include <Defs.hpp>
#include "Material.hpp"
#include "../GraphicsApi/VABuffer/VABuffer.hpp"
#include "../GraphicsApi/IBuffer/IBuffer.hpp"
#include "../GraphicsApi/VBuffer/VBuffer.hpp"

namespace Game{
    class Mesh3D{
        public:
            

        private:
            VBuffer         m_VBO;
            IBuffer         m_IBO;
            VABuffer        m_VABO;
            
    };
}