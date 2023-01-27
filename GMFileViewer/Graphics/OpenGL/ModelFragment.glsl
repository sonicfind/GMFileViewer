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
	vec3 ambient;
	vec3 diffuse;
	float min;
	float coefficient;
	float max;
};

layout (std140) uniform Lights
{
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
	vec4 result = vec4(0);
	if (useTexture == 1)
	{
		vec4 texColor = texture(tex, vs_in.texCoord);
		result = getBlendColor(texColor.rgb);
		result.a *= texColor.a;
	}
	else if (shadingType >= 3)
		result = vec4(1);

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
	if (shadingType < 3)
		return baseColor;
	else
		return baseColor * vs_in.color;

	//if (shadingType == 0)
		//return baseColor;
	//else if (shadingType == 3)
		//return baseColor * vs_in.color;

	
	vec3 viewDir = normalize(vec3(view[0][3], view[1][3], view[2][3]) - vs_in.fragPos);
	vec3 result = vec3(0);
	for (int i = 0; i < numLights; ++i)
	{
		vec3 lighting = vec3(0);
		if (shadingType >= 3)
			lighting = lights[i].ambient * vs_in.color.rgb;

		if (shadingType != 3)
		{
			float dp = dot(vs_in.normal, lights[i].direction);
			if (dp > 0)
			{
				lighting += (sceneAmbience + (lights[i].diffuse * diffuse.rgb)) * dp;

				vec3 reflectDir = reflect(-lights[i].direction, vs_in.normal);
				float specdot = 0;
				if (shadingType == 1 || shadingType == 4)
				{
					float specdot = dot(viewDir, reflectDir);
					if (specdot > 0)
						lighting += specular.rgb * diffuse.a * pow(specdot, specular.a);
				}					
			}
		}
		result += lighting;
	}

	float val = 1 - unknown;
	result = result * val + unknown;
	return vec4(result * baseColor.rgb, baseColor.a);
};
)"
