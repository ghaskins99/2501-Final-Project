// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

out vec4 FragColor;

uniform sampler2D onetex;
uniform vec3 colorMod;

void main()
{
	vec4 color = texture2D(onetex, uv_interp);
	FragColor = vec4(color.r,color.g,color.b,color.a);
	FragColor += vec4(colorMod, 0.0);					//the fragment color is modified by the 'color' uniform
    if((color.r + color.g + color.b) / 3.0 > 0.95)
	{
		discard;
	}
//	 FragColor = color_interp;
}