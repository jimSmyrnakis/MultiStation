#include "Draw.hpp"
#include <GL/glew.h>
#include "../Init.hpp"
namespace MultiStation {
	


	uint32_t ToOpenGLDrawMode(DrawMode mode) {
		switch (mode) {
			case DrawMode::POINTS:    return GL_POINTS;
			case DrawMode::LINES:     return GL_LINES;
			case DrawMode::TRIANGLES: return GL_TRIANGLES;
			default:                   return GL_TRIANGLES;
		}

		return GL_TRIANGLES;
	}
	DrawMode FromOpenGLDrawMode(uint32_t glMode) {
		switch (glMode) {
			case GL_POINTS:    return DrawMode::POINTS;
			case GL_LINES:     return DrawMode::LINES;
			case GL_TRIANGLES: return DrawMode::TRIANGLES;
			default:            return DrawMode::TRIANGLES;
		}

		return DrawMode::TRIANGLES;
	}

	GLenum TypeSizeToOpenGLTypeSize(uint32_t typeSize) {
		switch (typeSize) {
		case 1: return GL_UNSIGNED_BYTE;
		case 2: return GL_UNSIGNED_SHORT;
		case 4: return GL_UNSIGNED_INT;
		default:
			ASSERT(0, "Unknown Type Size for Index Buffer !");
			return GL_UNSIGNED_INT;
		}

		return GL_UNSIGNED_INT;
	}

	void DrawCommands::Draw(DrawMode mode, DrawParams& params, PipelineSettings settings) {
		GLenum glMode = ToOpenGLDrawMode(mode);
		ASSERT(params.ShaderProgram , "Nullptr for Shader Program in Draw Call !");
		ASSERT(params.VertexBuffer, "Nullptr for Vertex Buffer in Draw Call !");
		ASSERT(params.Count , "0 count for Draw Call !");

		//Set Frame Buffer
		if (params.FrameBuffer) {
			params.FrameBuffer->Bind();
		}
		else {
			GLCALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
		}

		//Set Shader Program
		params.ShaderProgram->Bind();
		//Set Vertex Buffer
		params.VertexBuffer->Bind();

		//Set Depth
		if (settings.DepthTestEnabled) {
			GLCALL(glEnable(GL_DEPTH_TEST));
		}
		else {
			GLCALL(glDisable(GL_DEPTH_TEST));
		}

		//Set Blending
		if (settings.BlendEnabled) {
			GLCALL(glEnable(GL_BLEND));
			GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		}
		else {
			GLCALL(glDisable(GL_BLEND));
		}

		//Set Culling
		if (settings.Cull.CullFaceEnabled) {
			GLCALL(glEnable(GL_CULL_FACE));
			if (settings.Cull.CullFront) {
				GLCALL(glCullFace(GL_FRONT));
			}
			else {
				GLCALL(glCullFace(GL_BACK));
			}
		}
		else {
			GLCALL(glDisable(GL_CULL_FACE));
		}

		//Draw Call
		//check if is index draw or not
		if (params.VertexBuffer->HasIndexBuffer()) {
			// check sign of index buffer
			GLenum Type = TypeSizeToOpenGLTypeSize( params.VertexBuffer->GetIndexBufferAttribs().TypeSize );
			GLCALL(glDrawElements(glMode, params.Count,
				Type, (void*)params.Start ));
		}
		else {
			GLCALL(glDrawArrays(glMode, params.Start, params.Count));
		}

		//Unbind Vertex Buffer
		params.VertexBuffer->Unbind();
		//Unbind Shader Program
		params.ShaderProgram->Unbind();
		//Unbind Frame Buffer
		if (params.FrameBuffer) {
			params.FrameBuffer->Unbind();
		}
	}

}
