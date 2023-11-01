
cbuffer Material
{
	uint blendType;
	uint shadingType;
	float4 diffuse;
	float4 specular;
	uint flags;
	uint textEnv;
	uint uTile;
	uint vTile;
};

uniform texture2D tex;
SamplerState smplr
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

uniform int useTexture;

cbuffer GlobalShading
{
	uint numLights;
	float unknown;
	uint useTint;

	float3 tint;
	float3 sceneAmbience;
};

static const uint MAX_LIGHTS = 4;

struct Light
{
	float3 diffuse;
	float3 specular;
	float3 direction;
};

cbuffer Lights
{
	Light lights[MAX_LIGHTS];
};

cbuffer View
{
	matrix view;
};

float getAlpha(float4 color);
float3 getShading();


float4 main(float3 fragPos : VERT_POS, float3 normal : VERT_NORM, float2 texCoord : VERT_TEXCOORD, float4 color : VERT_COLOR) : SV_TARGET
{
	float4 result = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if (useTexture == 1)
	{
		float4 baseColor = tex.Sample(smplr, texCoord);
		result = float4(baseColor.rgb, getAlpha(baseColor));
	}
	else if (shadingType == 3)
		result.a = getAlpha(color);
	else if (shadingType < 3)
		return float4(0, 0, 0, 1);

	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

float getAlpha(float4 color)
{
	if ((flags % 2) == 1)
		return 2 * color.a;

	switch (blendType)
	{
	case 1:
		return 2 * diffuse.a * color.a;
	case 3:
		return diffuse.a * (1 - color.r) * (1 - color.g) * (1 - color.b);
	case 4:
		return color.r;
	case 5:
		return 2 * diffuse.a * color.a;
	default:
		return 1.0;
	}
}

float3 getShading(float3 fragPos, float3 normal, float4 color)
{
	switch (shadingType)
	{
	case 0:
		return diffuse.rgb;
	case 3:
		return color.rgb;
	case 4:
		return 2 * sceneAmbience / numLights;
	default:
	{
		float3 viewDir = normalize(float3(view[0][3], view[1][3], view[2][3]) - fragPos);
		float3 result = float3(0, 0, 0);
		for (uint i = 0; i < numLights; ++i)
		{
			float3 lighting = sceneAmbience;
			float dp = dot(normal, lights[i].direction);
			if (dp > 0)
			{
				lighting += (lights[i].diffuse * diffuse.rgb) * dp;

				float3 reflectDir = reflect(-lights[i].direction, normal);
				float specdot = 0;
			}

			result += lighting;
		}

		return 2 * result / numLights;
	}
	}
};


