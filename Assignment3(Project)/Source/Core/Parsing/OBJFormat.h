#pragma once

#include "..\Graphics\ModelData.h"

bool load_obj(std::vector<MeshData>& meshes, const std::string& filename, const std::string& filepath);
//bool load_mtl(std::vector<MaterialData>& materials, const std::string& filename, const std::string& filepath);