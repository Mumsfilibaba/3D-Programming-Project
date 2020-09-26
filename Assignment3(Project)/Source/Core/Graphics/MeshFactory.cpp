#include "MeshFactory.h"
#include "..\Math\MathUtilities.h"

namespace Graphics
{
	GeometryData Meshfactory::CreateCube()
	{
		using namespace Math;

		GeometryData data;
		data.vertices.resize(24);
		data.indices.resize(36);

		//FRONT FACE
		data.vertices[0].uv = Vector2(0.0f, 0.0f);
		data.vertices[0].position = Vector3(-0.5f, 0.5f, -0.5f);
		data.vertices[1].uv = Vector2(1.0f, 0.0f);
		data.vertices[1].position = Vector3(0.5f, 0.5f, -0.5f);
		data.vertices[2].uv = Vector2(0.0f, 1.0f);
		data.vertices[2].position = Vector3(-0.5f, -0.5f, -0.5f);
		data.vertices[3].uv = Vector2(1.0f, 1.0f);
		data.vertices[3].position = Vector3(0.5f, -0.5f, -0.5f);
		data.vertices[0].normal = data.vertices[1].normal =
			data.vertices[2].normal = data.vertices[3].normal = Vector3(0.0f, 0.0f, -1.0f);
		data.vertices[0].tangent = data.vertices[1].tangent =
			data.vertices[2].tangent = data.vertices[3].tangent = Vector3(1.0f, 0.0f, 0.0f);

		//BACK FACE
		data.vertices[4].uv = Vector2(0.0f, 0.0f);
		data.vertices[4].position = Vector3(0.5f, 0.5f, 0.5f);
		data.vertices[5].uv = Vector2(1.0f, 0.0f);
		data.vertices[5].position = Vector3(-0.5f, 0.5f, 0.5f);
		data.vertices[6].uv = Vector2(0.0f, 1.0f);
		data.vertices[6].position = Vector3(0.5f, -0.5f, 0.5f);
		data.vertices[7].uv = Vector2(1.0f, 1.0f);
		data.vertices[7].position = Vector3(-0.5f, -0.5f, 0.5f);
		data.vertices[4].normal = data.vertices[5].normal =
			data.vertices[6].normal = data.vertices[7].normal = Vector3(0.0f, 0.0f, 1.0f);
		data.vertices[4].tangent = data.vertices[5].tangent =
			data.vertices[6].tangent = data.vertices[7].tangent = Vector3(-1.0f, 0.0f, 0.0f);

		//RIGHT FACE (Seen from FRONT FACE)
		data.vertices[8].uv = Vector2(0.0f, 0.0f);
		data.vertices[8].position = Vector3(0.5f, 0.5f, -0.5f);
		data.vertices[9].uv = Vector2(1.0f, 0.0f);
		data.vertices[9].position = Vector3(0.5f, 0.5f, 0.5f);
		data.vertices[10].uv = Vector2(0.0f, 1.0f);
		data.vertices[10].position = Vector3(0.5f, -0.5f, -0.5f);
		data.vertices[11].uv = Vector2(1.0f, 1.0f);
		data.vertices[11].position = Vector3(0.5f, -0.5f, 0.5f);
		data.vertices[8].normal = data.vertices[9].normal =
			data.vertices[10].normal = data.vertices[11].normal = Vector3(1.0f, 0.0f, 0.0f);
		data.vertices[8].tangent = data.vertices[9].tangent =
			data.vertices[10].tangent = data.vertices[11].tangent = Vector3(0.0f, 0.0f, 1.0f);

		//LEFT FACE (Seen from FRONT FACE)
		data.vertices[12].uv = Vector2(0.0f, 0.0f);
		data.vertices[12].position = Vector3(-0.5f, 0.5f, 0.5f);
		data.vertices[13].uv = Vector2(1.0f, 0.0f);
		data.vertices[13].position = Vector3(-0.5f, 0.5f, -0.5f);
		data.vertices[14].uv = Vector2(0.0f, 1.0f);
		data.vertices[14].position = Vector3(-0.5f, -0.5f, 0.5f);
		data.vertices[15].uv = Vector2(1.0f, 1.0f);
		data.vertices[15].position = Vector3(-0.5f, -0.5f, -0.5f);
		data.vertices[12].normal = data.vertices[13].normal =
			data.vertices[14].normal = data.vertices[15].normal = Vector3(-1.0f, 0.0f, 0.0f);
		data.vertices[12].tangent = data.vertices[13].tangent =
			data.vertices[14].tangent = data.vertices[15].tangent = Vector3(0.0f, 0.0f, -1.0f);

		//ROOF FACE (Seen from FRONT FACE)
		data.vertices[16].uv = Vector2(0.0f, 0.0f);
		data.vertices[16].position = Vector3(-0.5f, 0.5f, 0.5f);
		data.vertices[17].uv = Vector2(1.0f, 0.0f);
		data.vertices[17].position = Vector3(0.5f, 0.5f, 0.5f);
		data.vertices[18].uv = Vector2(0.0f, 1.0f);
		data.vertices[18].position = Vector3(-0.5f, 0.5f, -0.5f);
		data.vertices[19].uv = Vector2(1.0f, 1.0f);
		data.vertices[19].position = Vector3(0.5f, 0.5f, -0.5f);
		data.vertices[16].normal = data.vertices[17].normal =
			data.vertices[18].normal = data.vertices[19].normal = Vector3(0.0f, 1.0f, 0.0f);
		data.vertices[16].tangent = data.vertices[17].tangent =
			data.vertices[18].tangent = data.vertices[19].tangent = Vector3(1.0f, 0.0f, 0.0f);

		//FLOOR FACE (Seen from FRONT FACE)
		data.vertices[20].uv = Vector2(0.0f, 0.0f);
		data.vertices[20].position = Vector3(-0.5f, -0.5f, -0.5f);
		data.vertices[21].uv = Vector2(1.0f, 0.0f);
		data.vertices[21].position = Vector3(0.5f, -0.5f, -0.5f);
		data.vertices[22].uv = Vector2(0.0f, 1.0f);
		data.vertices[22].position = Vector3(-0.5f, -0.5f, 0.5f);
		data.vertices[23].uv = Vector2(1.0f, 1.0f);
		data.vertices[23].position = Vector3(0.5f, -0.5f, 0.5f);
		data.vertices[20].normal = data.vertices[21].normal =
			data.vertices[22].normal = data.vertices[23].normal = Vector3(0.0f, -1.0f, 0.0f);
		data.vertices[20].tangent = data.vertices[21].tangent =
			data.vertices[22].tangent = data.vertices[23].tangent = Vector3(1.0f, 0.0f, 0.0f);

		//FRONT FACE
		data.indices[0] = 0;
		data.indices[1] = 1;
		data.indices[2] = 2;
		data.indices[3] = 1;
		data.indices[4] = 3;
		data.indices[5] = 2;

		//BACK FACE
		data.indices[6] = 4;
		data.indices[7] = 5;
		data.indices[8] = 6;
		data.indices[9] = 5;
		data.indices[10] = 7;
		data.indices[11] = 6;

		//RIGHT FACE (Seen from FRONT FACE)
		data.indices[12] = 8;
		data.indices[13] = 9;
		data.indices[14] = 10;
		data.indices[15] = 9;
		data.indices[16] = 11;
		data.indices[17] = 10;

		//LEFT FACE (Seen from FRONT FACE)
		data.indices[18] = 12;
		data.indices[19] = 13;
		data.indices[20] = 14;
		data.indices[21] = 13;
		data.indices[22] = 15;
		data.indices[23] = 14;

		//ROOF FACE (Seen from FRONT FACE)
		data.indices[24] = 16;
		data.indices[25] = 17;
		data.indices[26] = 18;
		data.indices[27] = 17;
		data.indices[28] = 19;
		data.indices[29] = 18;

		//FLOOR FACE (Seen from FRONT FACE)
		data.indices[30] = 20;
		data.indices[31] = 21;
		data.indices[32] = 22;
		data.indices[33] = 21;
		data.indices[34] = 23;
		data.indices[35] = 22;

		return data;
	}

	GeometryData Meshfactory::CreatePlane(uint8 width, uint8 height)
	{
		using namespace Math;
		using namespace std;

		GeometryData data;
		if (width < 1 || height < 1)
			return data;

		data.vertices.resize((width + 1) * (height + 1));
		data.indices.resize((width * height) * 6);

		//Size of each quad, size of the plane will always be between -0.5 and 0.5
		Vector2 quadSize = Vector2((float)1 / (float)width, (float)1 / (float)height);
		Vector2 uvQuadSize = Vector2((float)1 / (float)width, (float)1 / (float)height);

		//Initialize vertices
		for (uint32 x = 0; x <= width; x++)
		{
			for (uint32 y = 0; y <= height; y++)
			{
				int v = ((1 + height) * x) + y;
				data.vertices[v].position = Vector3(0.5f - (quadSize.x * x), 0.5f - (quadSize.y * y), 0.0f);
				data.vertices[v].normal = Vector3(0.0f, 0.0f, -1.0f);
				data.vertices[v].tangent = Vector3(1.0f - (2.0f * (x & 1)), 0.0f, 0.0f);
				data.vertices[v].uv = Vector2(0.0f + (uvQuadSize.x * x), 0.0f + (uvQuadSize.y * y));
			}
		}

		//Initialize indices
		for (unsigned char x = 0; x < width; x++)
		{
			for (unsigned char y = 0; y < height; y++)
			{
				int quad = (height * x) + y;
				data.indices[quad * 6] = (x * (1 + height)) + y + 1;
				data.indices[(quad * 6) + 1] = (data.indices[quad * 6] + 2 + (height - 1));
				data.indices[(quad * 6) + 2] = data.indices[quad * 6] - 1;
				data.indices[(quad * 6) + 3] = data.indices[(quad * 6) + 1];
				data.indices[(quad * 6) + 4] = data.indices[(quad * 6) + 1] - 1;
				data.indices[(quad * 6) + 5] = data.indices[(quad * 6) + 2];
			}
		}

		data.vertices.shrink_to_fit();
		data.indices.shrink_to_fit();
		return data;
	}

	GeometryData Meshfactory::CreateSphere(uint8 subdivisions)
	{
		using namespace Math;

		GeometryData data;
		data.indices.resize(60);
		data.vertices.resize(12);

		//VERTICES
		data.vertices[0].position = Vector3(-0.525731f, 0.0f, 0.850651f);
		data.vertices[1].position = Vector3(0.525731f, 0.0f, 0.850651f);
		data.vertices[2].position = Vector3(-0.525731f, 0.0f, -0.850651f);
		data.vertices[3].position = Vector3(0.525731f, 0.0f, -0.850651f);
		data.vertices[4].position = Vector3(0.0f, 0.850651f, 0.525731f);
		data.vertices[5].position = Vector3(0.0f, 0.850651f, -0.525731f);
		data.vertices[6].position = Vector3(0.0f, -0.850651f, 0.525731f);
		data.vertices[7].position = Vector3(0.0f, -0.850651f, -0.525731f);
		data.vertices[8].position = Vector3(0.850651f, 0.525731f, 0.0f);
		data.vertices[9].position = Vector3(-0.850651f, 0.525731f, 0.0f);
		data.vertices[10].position = Vector3(0.850651f, -0.525731f, 0.0f);
		data.vertices[11].position = Vector3(-0.850651f, -0.525731f, 0.0f);

		//INDICIES
		data.indices[0] = 1;
		data.indices[1] = 4;
		data.indices[2] = 0;

		data.indices[3] = 4;
		data.indices[4] = 9;
		data.indices[5] = 0;

		data.indices[6] = 4;
		data.indices[7] = 5;
		data.indices[8] = 9;

		data.indices[9] = 8;
		data.indices[10] = 5;
		data.indices[11] = 4;

		data.indices[12] = 1;
		data.indices[13] = 8;
		data.indices[14] = 4;

		data.indices[15] = 1;
		data.indices[16] = 10;
		data.indices[17] = 8;

		data.indices[18] = 10;
		data.indices[19] = 3;
		data.indices[20] = 8;

		data.indices[21] = 8;
		data.indices[22] = 3;
		data.indices[23] = 5;

		data.indices[24] = 3;
		data.indices[25] = 2;
		data.indices[26] = 5;

		data.indices[27] = 3;
		data.indices[28] = 7;
		data.indices[29] = 2;

		data.indices[30] = 3;
		data.indices[31] = 10;
		data.indices[32] = 7;

		data.indices[33] = 10;
		data.indices[34] = 6;
		data.indices[35] = 7;

		data.indices[36] = 6;
		data.indices[37] = 11;
		data.indices[38] = 7;

		data.indices[39] = 6;
		data.indices[40] = 0;
		data.indices[41] = 11;

		data.indices[42] = 6;
		data.indices[43] = 1;
		data.indices[44] = 0;

		data.indices[45] = 10;
		data.indices[46] = 1;
		data.indices[47] = 6;

		data.indices[48] = 11;
		data.indices[49] = 0;
		data.indices[50] = 9;

		data.indices[51] = 2;
		data.indices[52] = 11;
		data.indices[53] = 9;

		data.indices[54] = 5;
		data.indices[55] = 2;
		data.indices[56] = 9;

		data.indices[57] = 11;
		data.indices[58] = 2;
		data.indices[59] = 7;

		if (subdivisions > 0)
			Subdivide(data, subdivisions);

		for (uint32 i = 0; i < data.vertices.size(); i++)
		{
			data.vertices[i].normal = data.vertices[i].position.Normalize();
			data.vertices[i].position *= 0.5f;
		}

		data.indices.shrink_to_fit();
		data.vertices.shrink_to_fit();

		return data;
	}

	GeometryData Meshfactory::CreatePyramid()
	{
		using namespace Math;

		GeometryData data;
		data.vertices.resize(16);
		data.indices.resize(18);

		//FLOOR FACE (Seen from FRONT FACE)
		data.vertices[0].uv = Vector2(0.33f, 0.33f);
		data.vertices[0].position = Vector3(-0.5f, -0.5f, -0.5f);
		data.vertices[1].uv = Vector2(0.66f, 0.33f);
		data.vertices[1].position = Vector3(0.5f, -0.5f, -0.5f);
		data.vertices[2].uv = Vector2(0.33f, 0.66f);
		data.vertices[2].position = Vector3(-0.5f, -0.5f, 0.5f);
		data.vertices[3].uv = Vector2(0.66f, 0.66f);
		data.vertices[3].position = Vector3(0.5f, -0.5f, 0.5f);

		//TOP VERTICES
		data.vertices[4].position = data.vertices[5].position = data.vertices[6].position = data.vertices[7].position = Vector3(0.0f, 0.5f, 0.0f);
		data.vertices[4].uv = Vector2(0.495f, 0.0f);
		data.vertices[5].uv = Vector2(0.0f, 0.495f);
		data.vertices[6].uv = Vector2(0.495f, 0.99f);
		data.vertices[7].uv = Vector2(0.99f, 0.495f);

		//BACK
		data.vertices[8].uv = Vector2(0.33f, 0.33f);
		data.vertices[8].position = Vector3(-0.5f, -0.5f, -0.5f);
		data.vertices[9].uv = Vector2(0.66f, 0.33f);
		data.vertices[9].position = Vector3(0.5f, -0.5f, -0.5f);

		//FRONT
		data.vertices[10].uv = Vector2(0.33f, 0.66f);
		data.vertices[10].position = Vector3(-0.5f, -0.5f, 0.5f);
		data.vertices[11].uv = Vector2(0.66f, 0.66f);
		data.vertices[11].position = Vector3(0.5f, -0.5f, 0.5f);

		//LEFT
		data.vertices[12].uv = Vector2(0.33f, 0.33f);
		data.vertices[12].position = Vector3(-0.5f, -0.5f, -0.5f);
		data.vertices[13].uv = Vector2(0.33f, 0.66f);
		data.vertices[13].position = Vector3(-0.5f, -0.5f, 0.5f);

		//RIGHT
		data.vertices[14].uv = Vector2(0.66f, 0.33f);
		data.vertices[14].position = Vector3(0.5f, -0.5f, -0.5f);
		data.vertices[15].uv = Vector2(0.66f, 0.66f);
		data.vertices[15].position = Vector3(0.5f, -0.5f, 0.5f);

		//FLOOR FACE
		data.indices[0] = 0;
		data.indices[1] = 1;
		data.indices[2] = 2;
		data.indices[3] = 1;
		data.indices[4] = 3;
		data.indices[5] = 2;

		//BACK FACE
		data.indices[6] = 4;
		data.indices[7] = 9;
		data.indices[8] = 8;

		//LEFT FACE
		data.indices[9] = 5;
		data.indices[10] = 12;
		data.indices[11] = 13;

		//FRONT FACE
		data.indices[12] = 6;
		data.indices[13] = 10;
		data.indices[14] = 11;

		//RIGHT FACE
		data.indices[15] = 7;
		data.indices[16] = 15;
		data.indices[17] = 14;

		data.indices.shrink_to_fit();
		data.vertices.shrink_to_fit();

		CalculateNormalsHardened(data);
		return data;
	}

	GeometryData Meshfactory::CreateCylinder(uint8 sides)
	{
		using namespace Math;
		GeometryData data;
		if (sides < 5)
		{
			return data;
		}

		data.vertices.resize(22);
		data.indices.resize(60);

		float angle = (PI_F * 2.0f) / 5.0f;
		//TOP CAP VERTICES
		data.vertices[0].position = Vector3(0.0f, 0.5f, 0.0f);
		data.vertices[1].position = Vector3(cosf(PI_F / 2.0f), 0.5f, sinf(PI_F / 2.0f)).Normalize() * 0.5f;
		data.vertices[2].position = Vector3(cosf((PI_F / 2.0f) + angle), 0.5f, sinf((PI_F / 2.0f) + angle)).Normalize() * 0.5f;
		data.vertices[3].position = Vector3(cosf((PI_F / 2.0f) + (angle * 2)), 0.5f, sinf((PI_F / 2.0f) + (angle * 2))).Normalize() * 0.5f;
		data.vertices[4].position = Vector3(cosf((PI_F / 2.0f) + (angle * 3)), 0.5f, sinf((PI_F / 2.0f) + (angle * 3))).Normalize() * 0.5f;
		data.vertices[5].position = Vector3(cosf((PI_F / 2.0f) + (angle * 4)), 0.5f, sinf((PI_F / 2.0f) + (angle * 4))).Normalize() * 0.5f;
		data.vertices[0].normal = Vector3(0.0f, -1.0f, 0.0f);
		data.vertices[1].position.y = 0.5f;
		data.vertices[1].normal = Vector3(0.0f, 1.0f, 0.0f);
		data.vertices[2].position.y = 0.5f;
		data.vertices[2].normal = Vector3(0.0f, -1.0f, 0.0f);
		data.vertices[3].position.y = 0.5f;
		data.vertices[3].normal = Vector3(0.0f, -1.0f, 0.0f);
		data.vertices[4].position.y = 0.5f;
		data.vertices[4].normal = Vector3(0.0f, -1.0f, 0.0f);
		data.vertices[5].position.y = 0.5f;
		data.vertices[5].normal = Vector3(0.0f, -1.0f, 0.0f);

		//BOTTOM CAP VERTICES
		data.vertices[6].position = data.vertices[0].position;
		data.vertices[7].position = data.vertices[1].position;
		data.vertices[8].position = data.vertices[2].position;
		data.vertices[9].position = data.vertices[3].position;
		data.vertices[10].position = data.vertices[4].position;
		data.vertices[11].position = data.vertices[5].position;
		data.vertices[6].position.y = -0.5f;
		data.vertices[6].normal = Vector3(0.0f, 1.0f, 0.0f);
		data.vertices[7].position.y = -0.5f;
		data.vertices[7].normal = Vector3(0.0f, 1.0f, 0.0f);
		data.vertices[8].position.y = -0.5f;
		data.vertices[8].normal = Vector3(0.0f, 1.0f, 0.0f);
		data.vertices[9].position.y = -0.5f;
		data.vertices[9].normal = Vector3(0.0f, 1.0f, 0.0f);
		data.vertices[10].position.y = -0.5f;
		data.vertices[10].normal = Vector3(0.0f, 1.0f, 0.0f);
		data.vertices[11].position.y = -0.5f;
		data.vertices[11].normal = Vector3(0.0f, 1.0f, 0.0f);

		//TOP SIDE VERTICES
		data.vertices[12].position = data.vertices[1].position;
		data.vertices[12].normal = data.vertices[12].position.UnitVector();
		data.vertices[13].position = data.vertices[2].position;
		data.vertices[13].normal = data.vertices[13].position.UnitVector();
		data.vertices[14].position = data.vertices[3].position;
		data.vertices[14].normal = data.vertices[14].position.UnitVector();
		data.vertices[15].position = data.vertices[4].position;
		data.vertices[15].normal = data.vertices[15].position.UnitVector();
		data.vertices[16].position = data.vertices[5].position;
		data.vertices[16].normal = data.vertices[16].position.UnitVector();

		//BOTTOM SIDE VERTICES
		data.vertices[17].position = data.vertices[7].position;
		data.vertices[17].normal = data.vertices[17].position.UnitVector();
		data.vertices[18].position = data.vertices[8].position;
		data.vertices[18].normal = data.vertices[18].position.UnitVector();
		data.vertices[19].position = data.vertices[9].position;
		data.vertices[19].normal = data.vertices[19].position.UnitVector();
		data.vertices[20].position = data.vertices[10].position;
		data.vertices[20].normal = data.vertices[20].position.UnitVector();
		data.vertices[21].position = data.vertices[11].position;
		data.vertices[21].normal = data.vertices[21].position.UnitVector();

		//TOP CAP INDICES
		data.indices[0] = 0;
		data.indices[1] = 2;
		data.indices[2] = 1;

		data.indices[3] = 0;
		data.indices[4] = 3;
		data.indices[5] = 2;

		data.indices[6] = 0;
		data.indices[7] = 4;
		data.indices[8] = 3;

		data.indices[9] = 0;
		data.indices[10] = 5;
		data.indices[11] = 4;

		data.indices[12] = 0;
		data.indices[13] = 1;
		data.indices[14] = 5;

		//BOTTOM CAP INDICES
		data.indices[15] = 6;
		data.indices[16] = 7;
		data.indices[17] = 8;

		data.indices[18] = 6;
		data.indices[19] = 8;
		data.indices[20] = 9;

		data.indices[21] = 6;
		data.indices[22] = 9;
		data.indices[23] = 10;

		data.indices[24] = 6;
		data.indices[25] = 10;
		data.indices[26] = 11;

		data.indices[27] = 6;
		data.indices[28] = 11;
		data.indices[29] = 7;

		//SIDES
		data.indices[30] = 17;
		data.indices[31] = 12;
		data.indices[32] = 13;
		data.indices[33] = 18;
		data.indices[34] = 17;
		data.indices[35] = 13;

		data.indices[36] = 18;
		data.indices[37] = 13;
		data.indices[38] = 14;
		data.indices[39] = 14;
		data.indices[40] = 19;
		data.indices[41] = 18;

		data.indices[42] = 19;
		data.indices[43] = 14;
		data.indices[44] = 15;
		data.indices[45] = 15;
		data.indices[46] = 20;
		data.indices[47] = 19;

		data.indices[48] = 20;
		data.indices[49] = 15;
		data.indices[50] = 16;
		data.indices[51] = 16;
		data.indices[52] = 21;
		data.indices[53] = 20;

		data.indices[54] = 21;
		data.indices[55] = 16;
		data.indices[56] = 17;
		data.indices[57] = 17;
		data.indices[58] = 16;
		data.indices[59] = 12;
		return data;
	}

	void Meshfactory::Subdivide(GeometryData& data, uint8 subdivisions)
	{
		using namespace Math;

		if (subdivisions < 1)
			return;

		Vertex v[3];
		int j = 0;
		int indexCount = 0;
		int vertexCount = 0;
		int oldVertexCount = 0;
		data.vertices.reserve((data.vertices.size() * (uint32)pow(2, subdivisions)));
		data.indices.reserve((data.indices.size() * (uint32)pow(4, subdivisions)));

		for (int i = 0; i < subdivisions; i++)
		{
			oldVertexCount = (int)data.vertices.size();
			indexCount = (int)data.indices.size();


			for (j = 0; j < indexCount; j += 3)
			{
				v[0].position = (data.vertices[data.indices[j]].position + data.vertices[data.indices[j + 1]].position) * 0.5f;
				v[1].position = (data.vertices[data.indices[j]].position + data.vertices[data.indices[j + 2]].position) * 0.5f;
				v[2].position = (data.vertices[data.indices[j + 1]].position + data.vertices[data.indices[j + 2]].position) * 0.5f;

				v[0].uv = (data.vertices[data.indices[j]].uv + data.vertices[data.indices[j + 1]].uv) * 0.5f;
				v[1].uv = (data.vertices[data.indices[j]].uv + data.vertices[data.indices[j + 2]].uv) * 0.5f;
				v[2].uv = (data.vertices[data.indices[j + 1]].uv + data.vertices[data.indices[j + 2]].uv) * 0.5f;

				v[0].normal = ((data.vertices[data.indices[j]].normal + data.vertices[data.indices[j + 1]].normal) * 0.5f).Normalize();
				v[1].normal = ((data.vertices[data.indices[j]].normal + data.vertices[data.indices[j + 2]].normal) * 0.5f).Normalize();
				v[2].normal = ((data.vertices[data.indices[j + 1]].normal + data.vertices[data.indices[j + 2]].normal) * 0.5f).Normalize();

				//Push the new vertices
				data.vertices.push_back(v[0]);
				data.vertices.push_back(v[1]);
				data.vertices.push_back(v[2]);


				//Push index of the new triangles
				vertexCount = (int)data.vertices.size();
				data.indices.push_back((vertexCount - 3));
				data.indices.push_back((vertexCount - 1));
				data.indices.push_back((vertexCount - 2));

				data.indices.push_back((vertexCount - 3));
				data.indices.push_back(data.indices[j + 1]);
				data.indices.push_back((vertexCount - 1));

				data.indices.push_back((vertexCount - 2));
				data.indices.push_back((vertexCount - 1));
				data.indices.push_back(data.indices[j + 2]);


				//Reassign the old indexes
				data.indices[j + 1] = ((vertexCount - 3));
				data.indices[j + 2] = ((vertexCount - 2));
			}

			Optimize(data, oldVertexCount);
		}

		data.vertices.shrink_to_fit();
		data.indices.shrink_to_fit();
	}

	void Meshfactory::Optimize(GeometryData& data, uint32 startVertex)
	{
		using namespace std;

		uint32 vertexSize = (uint32)data.vertices.size();
		uint32 indexSize = (uint32)data.indices.size();

		assert(startVertex < (uint32)data.vertices.size());

		uint32 k = 0;
		uint32 j = 0;

		for (uint32 i = startVertex; i < vertexSize; i++)
		{
			for (j = 0; j < vertexSize; j++)
			{
				if (data.vertices[i] == data.vertices[j])
				{
					if (i != j)
					{
						data.vertices.erase(data.vertices.begin() + i);
						vertexSize--;
						j--;

						for (k = 0; k < indexSize; k++)
						{
							if (data.indices[k] == i)
								data.indices[k] = j;
							else if (data.indices[k] > i)
								data.indices[k]--;
						}

						i--;
						break;
					}
				}
			}
		}
	}

	void Meshfactory::CalculateNormalsHardened(GeometryData& data)
	{
		using namespace Math;
		Vector3 e1;
		Vector3 e2;
		Vector3 n;

		for (int i = 0; i < data.indices.size(); i += 3)
		{
			e1 = data.vertices[data.indices[i + 1]].position - data.vertices[data.indices[i]].position;
			e2 = data.vertices[data.indices[i + 2]].position - data.vertices[data.indices[i]].position;
			n = e1.Cross(e2);

			data.vertices[data.indices[i]].normal = n;
			data.vertices[data.indices[i + 1]].normal = n;
			data.vertices[data.indices[i + 2]].normal = n;
		}
	}
}