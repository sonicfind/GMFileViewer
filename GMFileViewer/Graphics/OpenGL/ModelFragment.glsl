R"(#version 330 core
out vec4 FragColor;

in VS_OUT
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoord;
	vec4 color;
} vs_in;

layout (std140) uniform Material
{
	int blendType;
	int shadingType;
	vec4 diffuse;
	vec4 specular;
	int flags;
	int textEnv;
	int uTile;
	int vTile;
};

uniform sampler2D tex;
uniform int useTexture;

const int MAX_LIGHTS = 4;

struct Light
{
	vec3 direction;
	vec3 diffuse;
	vec3 specular;
	float min;
	float coefficient;
	float max;
};

layout (std140) uniform Lights
{
	int doLights;
	int numLights;
	float unknown;
	int useGlobal;

	vec3 globalVertexColor;
	vec3 sceneAmbience;
	Light lights[MAX_LIGHTS];
};

layout (std140) uniform View
{
	mat4 view;
};

vec4 getBlendColor(const vec3 color);
vec4 applyShading(const vec4 baseColor);

void main()
{	
	vec4 result;
	if (useTexture == 1)
	{
		vec4 texColor = texture(tex, vs_in.texCoord);
		if (shadingType != 3)
			result = getBlendColor(texColor.rgb);
		else
			result = getBlendColor(texColor.rgb * vs_in.color.rgb);
		result.a *= texColor.a;
	}	
	else if (shadingType == 3)
		result = getBlendColor(globalVertexColor + vs_in.color.rgb);
	else if (useGlobal == 1)
		result = getBlendColor(globalVertexColor);
	else
		result = getBlendColor(diffuse.rgb);

	if (flags > 2)
		result.a *= 2;

	FragColor = applyShading(result);
}

vec4 getBlendColor(const vec3 color)
{
	switch (blendType)
	{
	case 3:
		return vec4(color.rgb, 1 - diffuse.a * (1 - color.r) * (1 - color.g) * (1 - color.b));
	case 4:
		return vec4(color.rgb, color.r);
	case 5:
		return vec4(color.rgb, diffuse.a);
	default:
		return vec4(color, 1);
	}
}

vec4 applyShading(const vec4 baseColor)
{
	if (doLights == 0 || shadingType == 0 || shadingType == 3)
		return baseColor;
	
	vec3 ambient = vec3(0);
	vec3 diffvec = vec3(0);
	vec3 specvec = vec3(0);
	vec3 viewDir = normalize(vec3(view[0][3], view[1][3], view[2][3]) - vs_in.fragPos);
	float recip = 1.0f / numLights;
	for (int i = 0; i < numLights; ++i)
	{
		ambient += lights[i].max * sceneAmbience * 2;
		// diffuse shading
		float diff = max(dot(vs_in.normal, -lights[i].direction), 0);
		if (shadingType < 3)
			diffvec += recip * diff * lights[i].diffuse;
		else
			diffvec += recip * diff * (lights[i].diffuse + globalVertexColor.rgb + vs_in.color.rgb);

		if (shadingType == 1 || shadingType == 4)
		{
			// specular shading
			vec3 reflectDir = reflect(lights[i].direction, vs_in.normal);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), specular.a);
			specvec += recip * spec * lights[i].specular;
		}
	}
	return vec4((ambient + diffvec + specvec * specular.rgb) * baseColor.rgb, baseColor.a);
};
)"
