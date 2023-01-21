R"(#version 430 core
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

struct Vertex
{
	vec4 position;
	vec3 normal;
	vec4 color;
	vec2 texCoord;
};

layout (packed) buffer OutputVerts
{
	Vertex outputVertices[];
};

layout (std430) buffer InputPositions
{
	vec4 inputPos[];
};

layout (std430) buffer Targets
{
	int targets[];
};

struct Weight
{
	float val[4];
};

layout (std430) buffer Weights
{
	Weight weights[];
};

uniform mat4 bones[4];
uniform uint startVertex;

void main()
{
	for (uint index = 0, targetIndex = 0; index < weights.length(); ++index, ++targetIndex)
	{
		mat4 matrix;
		float total = 0;
		for (uint i = 0; i < 4 && total < 1; ++i)
		{
			matrix += weights[index].val[i] * bones[i];
			total += weights[index].val[i];
		}

		const vec4 position = matrix * inputPos[startVertex + index];
		while (targets[targetIndex] != -1)
		{
			outputVertices[targets[targetIndex]].position = position;
			++targetIndex;
		}
	}
}
)"
