#pragma once

#include "../../../Defs.hpp"
#include "../FBuffer/FBuffer.hpp"
#include "../VBuffer/VBuffer.hpp"
#include "../Shader/Shader.hpp"
#include "../UBuffer/UBuffer.hpp"
#include "../Texture/Texture2D.hpp"

namespace MultiStation{

	enum class DrawMode {
		
			POINTS,
				LINES,
				TRIANGLES
		

		
	};

	uint32_t ToOpenGLDrawMode(DrawMode mode);
	DrawMode FromOpenGLDrawMode(uint32_t glMode);


	struct DrawParams {
		DrawMode    Mode;
		uint32_t    Count;
		int32_t     Start;
		FBuffer* FrameBuffer;
		Shader* ShaderProgram;
		VBuffer* VertexBuffer;
		bool DrawToSurface;
	};

	struct Culling {
		bool CullFaceEnabled;
		bool CullFront;
	};
	struct PipelineSettings {
		bool DepthTestEnabled;
		bool BlendEnabled;
		Culling Cull;
	};

	class DrawCommands {

	public:
		static void Draw(DrawMode mode, DrawParams& params , PipelineSettings settings );
		



	};
}
