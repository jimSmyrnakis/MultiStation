#include "RenderCommand.hpp"
#include <glad/gl.h>

namespace Game{
    static char vsource[] = 
    "#version 330 core\n"
    "layout (location = 0) in vec2 in_position;\n"
    "layout (location = 0) in vec2 in_text_coord;\n"
    "out vec2 v_text_coord;\n"
    "void main(void){ \n"
    "v_text_coord = in_text_coord;\n"
    "gl_Position = vec4(in_position, 1.0, 1.0);}\n"
    ;
    static char fsource[] = 
    "#version 330 core\n"
    "layout (location = 0) out vec4 out_Color;\n"
    "uniform sampler2D u_texture;\n"
    "in vec2 v_text_coord;\n"
    "void main(void){ \n"
    "out_Color = texture(u_texture , v_text_coord );}\n"
    ;

    static Shader*  s_shad;
    static VABuffer*   s_vao;
    static float s_Vertex[] = {
        -1 , -1 ,   0 , 1 ,
        -1 , +1 ,   0 , 0 ,
        +1 , -1 ,   1 , 1 ,
        +1 , +1 ,   1 , 0
    };
    static u32  s_Indeces[] = {
        0 , 1 , 2 ,
        1 , 2 , 3
    };
    


    static void initLastFrameDraw(void){
        static bool s_Initiallized = false;
        if (s_Initiallized)
            return;

        static VShader s_vs(vsource);
        static FShader s_fs(fsource);
        static Shader  s_shadIn(s_vs , s_fs);
        static VBuffer vb;
        static VertexLayout layout;
        static VertexAttribute a_xy(ShaderDataType::VEC2F , true);
        static VertexAttribute a_st(ShaderDataType::VEC2F , true);
        layout.AddAttribute(a_xy);
        layout.AddAttribute(a_st);
        vb.SetLayout(layout);
        vb.SetData(s_Vertex , sizeof(s_Vertex) );

        static IBuffer ib;
        ib.SetData(s_Indeces , sizeof(s_Indeces)/ sizeof(u32));



        s_vao = new VABuffer();
        s_vao->AddVertexBuffer(vb);
        s_vao->SetIndexBuffer(ib);
        s_shad = &s_shadIn;

        s_Initiallized  =true;
        

        
    }

    RenderCommand::RenderCommand(const Texture2DResolution& maxResolution){
        initLastFrameDraw();
        m_ClearColor = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
        m_ClearDepth = 1.0f;
        m_ViewPort   = glm::vec4(0,0,maxResolution.GetWidth(), maxResolution.GetHeight());
        m_FrameBuffer = new FBuffer(maxResolution , DepthFormat::DEPTH32 );
        m_FrameBuffer->Bind();
    }
     
    void RenderCommand::Draw(Material& material , const VABuffer& va){
        m_FrameBuffer->Bind();
        va.Bind();
        material.Bind();
        
        GLCALL( glDrawElements(GL_TRIANGLES , va.GetIndexBuffer().GetCount() , GL_UNSIGNED_INT , nullptr) );
        
    }

    void RenderCommand::ClearColor(const glm::vec4& rgba ){
        m_ClearColor = rgba;
    }

    void RenderCommand::ClearDepth(float clampValue){
        m_ClearDepth = clampValue;
    }

    void RenderCommand::Clear(bool ClearDepth ){
        m_FrameBuffer->ClearColorBuffer(0 , m_ClearColor);
        if (ClearDepth){
            m_FrameBuffer->ClearDepthBuffer(m_ClearDepth);
        }
    }

    void RenderCommand::ViewPort(const glm::vec4& viewport){
        m_ViewPort = viewport;
    }

    void RenderCommand::FinalDraw(void){
        m_FrameBuffer->Unbind();
        s_vao->Bind();
        m_FrameBuffer->GetColorBuffer(0)->Bind();
        u32* slot = (u32*)s_shad->GetUniforms()->GetUniformPointerByName("u_texture");
        *slot = 0;
        glViewport(m_ViewPort.x , m_ViewPort.y , m_ViewPort.z , m_ViewPort.w);
        GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , 0) );
        GLCALL( glClearColor(m_ClearColor.r , m_ClearColor.g , m_ClearColor.b , m_ClearColor.a ) );
        GLCALL( glClear(GL_COLOR_BUFFER_BIT) );
        GLCALL( glDrawElements(GL_TRIANGLES , s_vao->GetIndexBuffer().GetCount() , GL_UNSIGNED_INT , nullptr) );

        s_vao->Unbind();
        m_FrameBuffer->Bind();

    }

    void RenderCommand::ClearBackground(void){
        // Take old one frame buffer (Basicly after this function end the previus frame buffer must be already bind)
        i32 OldFB ;
        GLCALL( glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &OldFB) );

        //bind this frame buffer and clear the specific active color buffer 
        GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , 0) );
        GLCALL( glClearColor(m_ClearColor.r , m_ClearColor.g , m_ClearColor.b , m_ClearColor.a ) );
        GLCALL( glClear(GL_COLOR_BUFFER_BIT) );

        //Bind the old one frame buffer 
        GLCALL( glBindFramebuffer(GL_DRAW_FRAMEBUFFER , OldFB) );
    }
}