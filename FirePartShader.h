#pragma once

#include <GL/glew.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileUtils.h"
#include "Shader.h"

class FirePartShader : public Shader {
public:
	FirePartShader();
	~FirePartShader();

	virtual void attributeBinding();

	// Getters
	inline GLuint getShaderID() { return shaderProgram; }
private:
	GLuint shaderProgram;
	// Source code of vertex shader for particle system
	const char *source_vpart = "#version 130\n\
	\n\
	// Vertex buffer\n\
	in vec2 vertex;\n\
	in vec2 dir;\n\
	in float t;\n\
	in vec2 uv;\n\
	out vec2 uv_interp;\n\
	\n\
	// Uniform (global) buffer\n\
	uniform mat4 x;\n\
	uniform float time;\n\
	\n\
	// Attributes forwarded to the fragment shader\n\
	out vec4 color_interp;\n\
	out float colourTime; \n\
	out float fadeTime;\n\
	\n\
	\n\
	void main()\n\
	{\n\
		vec4 ppos;\n\
		float acttime;\n\
		float speed = 9.0;\n\
		float gravity = -9.8;\n\
		acttime = mod(time + t*2.0, 1.0);\n\
		colourTime = mod(time, 4);\n\
		fadeTime = acttime;\n\
		//colourTime = max(colourTime, 1);\n\
		ppos = vec4(vertex.x+dir.x*acttime*speed , vertex.y+dir.y*acttime*speed + -0.5*gravity*acttime*acttime, 0.0, 1.0);\n\
		gl_Position = x*ppos;\n\
		\n\
	//    color_interp = vec4(0.5+0.5*cos(4*acttime),0.5*sin(4*acttime)+0.5,0.5, 1.0);\n\
		color_interp = vec4(uv,0.5,1.0);\n\
		uv_interp = uv;\n\
	}";


	// Source code of fragment shader
	const char *source_fp = "#version 130\n\
	\n\
	// Attributes passed from the vertex shader\n\
	in vec4 color_interp;\n\
	in vec2 uv_interp;\n\
	in float colourTime;\n\
	in float fadeTime;\n\
	\n\
	uniform sampler2D onetex;\n\
	\n\
	\n\
	void main()\n\
	{\n\
		vec4 color = texture2D(onetex, uv_interp);\n\
		gl_FragColor = vec4(color.r, color.g, color.b * colourTime * 1.75, color.a * (1 - fadeTime));\n\
		if(color.a < 0.5)\n\
		{\n\
	//		gl_FragColor.a = 0.0;\n\
			discard;\n\
		}\n\
	//	 gl_FragColor = color_interp;\n\
	//gl_FragColor = vec4(1,1,1,1);\n\
	}";
};