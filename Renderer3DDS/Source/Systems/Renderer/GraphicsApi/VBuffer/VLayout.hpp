#pragma once 

#include <Defs.hpp>
#include <Core.hpp>
#include <vector>
#include "../Format/Format.hpp"

namespace Game{


    struct VertexAttribute{
        ShaderDataType  Type;
        u32             Size;
        u32             Offs;
        bool            Normallized;

        VertexAttribute( ShaderDataType type , bool Norm = true);
        VertexAttribute(void) = default;

        u32 GetComponentCount(void) const;
    };

    class VertexLayout{
        public:
            
            VertexLayout(void);

            void AddAttribute(const VertexAttribute& attribute);

            const std::vector<VertexAttribute>& GetAttributes(void) const ;

            u32 GetStride(void) const ;
        private:
            std::vector<VertexAttribute> m_Attributes;
            u32                          m_Stride  ;
        private:
            void CalculateOffsetsAndStride(void);
    };
}