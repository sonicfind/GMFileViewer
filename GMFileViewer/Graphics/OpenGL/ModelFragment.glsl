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

layout (std140) uniform GlobalShading
{
	int numLights;
	float unknown;
	int useTint;

	vec3 tint;
	vec3 sceneAmbience;
};

const int MAX_LIGHTS = 4;

struct Light
{
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};

layout (std140) uniform Lights
{
	Light lights[MAX_LIGHTS];
};

layout (std140) uniform View
{
	mat4 view;
};

float getAlpha(vec4 color);
vec3 getShadeFactor();

void main()
{	
	vec4 base = vec4(1);
	if (useTexture == 1)
		base = texture(tex, vs_in.texCoord);
	else if (shadingType != 3)
	{
		FragColor = vec4(0,0,0,1);
		return;
	}

	float alpha;
	if ((flags & 1) == 0)
		alpha = getAlpha(base);
	else
		alpha = 2 * diffuse.a * base.a;
		
	FragColor = vec4(getShadeFactor() * base.rgb, alpha);
}

float getAlpha(vec4 color)
{
	switch (blendType)
	{
	case 1:
	case 5:
		return 2 * diffuse.a * color.a;
	case 4:
		return color.r;
	default:
		return 1.0;
	}
}

vec3 getShadeFactor()
{
	switch (shadingType)
	{
	case 0:
		return diffuse.rgb;
	case 3:
		return vs_in.color.rgb;
	case 4:
		return numLights * (vs_in.color.rgb + sceneAmbience);
	default:
	{
		vec3 viewDir = normalize(vec3(view[0][3], view[1][3], view[2][3]) - vs_in.fragPos);
		vec3 result = vec3(0);
		for (int i = 0; i < numLights; ++i)
		{
			vec3 lighting = sceneAmbience;
			float dp = dot(vs_in.normal, lights[i].direction);
			if (dp > 0)
			{
				if (shadingType < 3)
					lighting += (lights[i].diffuse * diffuse.rgb) * dp;

				vec3 reflectDir = reflect(-lights[i].direction, vs_in.normal);
				float specdot = 0;
				if (shadingType == 1 || shadingType == 4)
				{
					//float specdot = dot(viewDir, reflectDir);
					//if (specdot > 0)
						//lighting += specular.rgb * pow(specdot, specular.a);
				}
			}
		
			result += lighting;
		}

		return result;
	}
	}
};
)"
