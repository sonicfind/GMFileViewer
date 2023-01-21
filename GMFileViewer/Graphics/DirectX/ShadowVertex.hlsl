cbuffer Model
{
	matrix model;
};

struct VSOut
{
	float4 screenPos : SV_POSITION;
	float3 fragPos   : VERT_POS;
	float3 normal    : VERT_NORM;
	float2 texCoord  : VERT_TEXCOORD;
	float4 color     : VERT_COLOR;
};

VSOut main(float4 pos : POSITION, float3 norm : NORMAL, float4 color : COLOR, float2 texCoord : TEXCOORD, uint envelopeIndex : ENVELOPE, float4 weights : WEIGHTS, uint instanceID : SV_InstanceID)
{
	const matrix model = models[instanceID];
	const matrix weightedMatrix = weights[0] * envelopes[envelopeIndex].bones[0];

	const float4 finalPos = mul(weightedMatrix, float4(pos.xyz, 1));
	const float4 finalNorm = mul(weightedMatrix, float4(norm.xyz, 0));

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