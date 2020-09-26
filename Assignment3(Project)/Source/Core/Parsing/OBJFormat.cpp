#include "OBJFormat.h"
#include "..\Utilities\Utilities.h"
#include "..\Utilities\Clock.h"
#include "..\Utilities\StringHelper.h"
#include <iostream>
#include <unordered_map>

//Uses C-code since it seems to be faster in benchmarking
#define OBJ_ERROR_SUCCESS 0
#define OBJ_ERROR_CORRUPT_FILE 1

struct OBJVertex
{
	int position = -1;
	int normal = -1;
	int uv = -1;

	inline bool operator==(const OBJVertex& other) const
	{
		return (position == other.position) && (normal == other.normal) && (uv == other.uv);
	}

	inline bool operator!=(const OBJVertex& other) const
	{
		return !(*this == other);
	}
};

struct OBJMesh
{
	std::vector<OBJVertex> vertices;
	std::vector<uint32> indices;
};


void print_error(int error);
void skip_line(const char** const iter);
void parse_obj_vertex(OBJVertex& vertex, const char** const buffer);
void parse_vector2(Math::Vector2& vector, const char** const iter);
void parse_vector3(Math::Vector3& vector, const char** const iter);
void add_unique_obj_vertex(const OBJVertex& vertex, OBJMesh& mesh);

void skip_line(const char** const iter)
{
	//Uses space since all other characters aren't relevant (See ASCII-table)
	while ((**iter) >= ' ')
		(*iter)++;
}

//Parse a vector2
void parse_vector2(Math::Vector2& vector, const char** const iter)
{
	int length = 0;
	vector.x = (float)fast_atof(*iter, length);

	(*iter) += length + 1;

	vector.y = (float)fast_atof(*iter, length);

	(*iter) += length;
}

//Parse a vector3
void parse_vector3(Math::Vector3& vector, const char** const iter)
{
	int length = 0;
	vector.x = (float)fast_atof(*iter, length);

	(*iter) += length + 1;

	vector.y = (float)fast_atof(*iter, length);

	(*iter) += length + 1;

	vector.z = (float)fast_atof(*iter, length);

	(*iter) += length;
}

void add_unique_obj_vertex(const OBJVertex& vertex, OBJMesh& mesh)
{
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		if (mesh.vertices[i] == vertex)
		{
			mesh.indices.emplace_back(i);
			return;
		}
	}

	mesh.vertices.emplace_back(vertex);
	mesh.indices.emplace_back((uint32)mesh.vertices.size() - 1);
}

//Parse a vertes
void parse_obj_vertex(OBJVertex& vertex, const char** const buffer)
{
	int length = 0;
	
	vertex.position = fast_atoi((*buffer), length);
	(*buffer) += (length + 1);

	vertex.uv = fast_atoi((*buffer), length);
	(*buffer) += (length + 1);

	vertex.normal = fast_atoi((*buffer), length);
	(*buffer) += length;
}

//error printing function
void print_error(int error)
{
	switch (error)
	{
	case OBJ_ERROR_CORRUPT_FILE:
		std::cout << "ERROR LOADING OBJ: Corrupt file\n";
		break;
	case OBJ_ERROR_SUCCESS:
	default:
		std::cout << "No error loading obj\n"; 
		break;
	}
}


//"Main" for loading .obj-files
bool load_obj(std::vector<MeshData>& meshes, const std::string& filename, const std::string& filepath)
{
	//TODO: Load mtl
	//TODO: Load multple meshes from 1 file
	//TODO: Benchmark
	//TODO: Remove strings

	using namespace Math;

	const char* buffer = nullptr;

	//Read in the full textfile into a buffer
	uint32 bytes = read_textfile(filepath + filename, &buffer);
	if (bytes == 0)
		return false;

	//VertexData and OBJMeshes
	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> textureCoords;
	std::vector<OBJMesh> objMeshes;

	//Declare variables here to save on allocations
	const char* iter;
	Vector3 vec3;
	Vector2 vec2;
	OBJVertex vertex;
	int32 i = 0;
	uint32 currentMesh = 0;

	//Initialize variables
	objMeshes.resize(1);
	currentMesh = 0;
	iter = buffer;

	while (*iter != '\0')
	{
		switch (*iter)
		{
		case '#':
			//Comment
			skip_line(&iter);
			break;
		case 'v':
			//Some vertex attribute
			iter++;

			if (*iter == 't')
			{
				//TextureCoords
				iter++;
				if ((*iter) != ' ')
				{
					print_error(OBJ_ERROR_CORRUPT_FILE);
					
					ReleaseBuffer_S(buffer);
					
					return false;
				}

				//Parse UV
				iter++;
				parse_vector2(vec2, &iter);


				if ((*iter) != '\n')
				{
					print_error(OBJ_ERROR_CORRUPT_FILE);

					ReleaseBuffer_S(buffer);

					return false;
				}

				textureCoords.emplace_back(vec2);
				std::cout << "Found TexCoord: " << vec2.ToString() << '\n';
			}
			else if ((*iter) == 'n')
			{
				//Normals
				iter++;
				if ((*iter) != ' ')
				{
					print_error(OBJ_ERROR_CORRUPT_FILE);

					ReleaseBuffer_S(buffer);

					return false;
				}

				//Parse Normal
				iter++;
				parse_vector3(vec3, &iter);


				if ((*iter) != '\n')
				{
					print_error(OBJ_ERROR_CORRUPT_FILE);

					ReleaseBuffer_S(buffer);

					return false;
				}

				normals.emplace_back(vec3);
				std::cout << "Found Normal: " << vec3.ToString() << '\n';
			}
			else if ((*iter) == ' ')
			{
				//Parse Normal
				iter++;
				parse_vector3(vec3, &iter);


				if ((*iter) != '\n')
				{
					print_error(OBJ_ERROR_CORRUPT_FILE);

					ReleaseBuffer_S(buffer);

					return false;
				}

				positions.emplace_back(vec3);
				std::cout << "Found Position: " << vec3.ToString() << '\n';
			}
			else
			{
				print_error(OBJ_ERROR_CORRUPT_FILE);

				ReleaseBuffer_S(buffer);

				return false;
			}

			break;
		case 'f':
			std::cout << "Found face: ";
			
			{
				int len = 0;

				iter++;
				if ((*iter) != ' ')
				{
					print_error(OBJ_ERROR_CORRUPT_FILE);

					ReleaseBuffer_S(buffer);

					return false;
				}

				for (i = 0; i < 3; i++)
				{
					iter++;
					parse_obj_vertex(vertex, &iter);					
					add_unique_obj_vertex(vertex, objMeshes[currentMesh]);

					std::cout << vertex.position << '/' << vertex.uv << '/' << vertex.normal << ' ';
				}

				if ((*iter) != '\n' && (*iter) != '\0')
				{
					iter++;
					parse_obj_vertex(vertex, &iter);

					uint32 size = (uint32)objMeshes[currentMesh].vertices.size();
					objMeshes[currentMesh].indices.emplace_back(size - 1);
					objMeshes[currentMesh].indices.emplace_back(size - 2);

					add_unique_obj_vertex(vertex, objMeshes[currentMesh]);

					std::cout << vertex.position << '/' << vertex.uv << '/' << vertex.normal << ' ';
				}

				std::cout << '\n';
			}

			break;
		case 'g':
			std::cout << "Found group\n";
			break;
		default:
			skip_line(&iter);
			break;
		}

		iter++;
	}

	std::cout << "Vertices:\n";
	for (int i = 0; i < objMeshes[0].vertices.size(); i++)
	{
		std::cout << objMeshes[0].vertices[i].position << '/' <<
			objMeshes[0].vertices[i].uv << '/' <<
			objMeshes[0].vertices[i].normal << '\n';
	}

	std::cout << "Indices:\n";
	for (int i = 0; i < objMeshes[0].indices.size(); i++)
	{
		std::cout << objMeshes[0].indices[i] << '\n';
	}

	ReleaseBuffer_S(buffer);
	return true;
}