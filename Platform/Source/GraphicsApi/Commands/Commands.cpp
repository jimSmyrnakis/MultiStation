#include "Commands.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_STATIC
#include <GLFW/glfw3.h>
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

	GLenum TypeSizeToOpenGLType(uint32_t typeSize) {
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

	
	void Commands::Draw(DrawMode mode, DrawParams& params)
	{
		//GLCALL(glPolygonMode(GL_BACK, GL_));
		GLCALL(glCullFace(GL_BACK));
		GLCALL(glEnable(GL_CULL_FACE)); // αν το είχες ενεργό πριν
		GLCALL(glEnable(GL_DEPTH_TEST));
		GLenum glMode = ToOpenGLDrawMode(mode);
		ASSERT(params.VertexBuffers, "Nullptr for Vertex Buffer in Draw Call !");

		// --- Framebuffer bind (εσένα ΔΕΝ χρειάζεται να το ανοίγεις/κλείνεις κάθε φορά)
		params.FrameBuffer->Bind();

		for (int i = 0; i < params.VBCount; i++)
		{
			VBuffer& vb = params.VertexBuffers[i];
			vb.Bind();

			if (vb.HasIndexBuffer())
			{
				auto attrib = vb.GetIndexBufferAttribs();

				uint32_t typeSize = attrib.TypeSize;
				uint32_t count = attrib.Count;
				GLenum   glType = TypeSizeToOpenGLType(typeSize);

				// σωστό offset σε bytes
				uint32_t offsetBytes = params.Start * typeSize;

				glDrawElements(glMode, count, glType, (void*)offsetBytes);
			}
			else
			{
				glDrawArrays(glMode, params.Start, params.Count);
			}
		}

		params.FrameBuffer->Unbind();
	}

}
