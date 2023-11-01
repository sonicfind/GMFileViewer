cbuffer View
{
	matrix view;
};

cbuffer Combo
{
	matrix combo;
};

cbuffer Model
{
	matrix model;
};

cbuffer Material
{
	int blendType;
	int shadingType;
	float4 diffuse;
	float4 specular;
	int flags;
	int textEnv;
	int uTile;
	int vTile;
};

struct VSOut
{
	float3 fragPos   : VERT_POS;
	float3 normal    : VERT_NORM;
	float2 texCoord  : VERT_TEXCOORD;
	float4 color     : VERT_COLOR;
	float4 screenPos : SV_POSITION;
};

VSOut main(float4 pos : POSITION, float3 norm : NORMAL, float4 color : COLOR, float2 texCoord : TEXCOORD)
{
	const float4 finalPos = float4(pos.xyz, 1);

	VSOut vs_out;
	vs_out.screenPos = mul(mul(combo, model), finalPos);
	vs_out.fragPos = mul(model, finalPos).xyz;
	vs_out.normal = normalize(mul(model, float4(norm, 0)).xyz);
	vs_out.color = color;

	if (textEnv == 0)
		vs_out.texCoord = texCoord;
	else
	{
		// Spherical texture environment
		float3 r = reflect(mul(view, float4(vs_out.fragPos, 1)), mul(view, float4(vs_out.normal, 0))).xyz;
		float m = 2 * sqrt(pow(r.x, 2) + pow(r.y, 2) + pow(r.z + 1, 2));
		vs_out.texCoord = r.xy / m + .5;
	}
	return vs_out;
}