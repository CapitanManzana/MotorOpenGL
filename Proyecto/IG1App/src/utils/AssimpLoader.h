#pragma once
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <core/Mesh.h>
#include <utils/logger.h>

namespace cme {
	class AssimpLoader {
	public:
		static std::vector<std::shared_ptr<Mesh>> loadModel(const std::string& path) {
			std::vector<std::shared_ptr<Mesh>> meshes;
			Assimp::Importer importer;

			const aiScene* scene = importer.ReadFile(path, 
				aiProcess_Triangulate | 
				aiProcess_JoinIdenticalVertices | 
				aiProcess_SortByPType | 
				aiProcess_FlipUVs |
				aiProcess_GenSmoothNormals |
				aiProcess_GlobalScale |
				aiProcess_PreTransformVertices);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				LOG_ERROR("Assimp Error: " + std::string(importer.GetErrorString()));
				return meshes;
			}

			// Generar colores aleatorios para cada malla
			// TODO: Quitar esto cuando se implementen texturas
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dis(0.3f, 1.0f);

			for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
				aiMesh* assimpMesh = scene->mMeshes[i];
				if (assimpMesh->mNumVertices == 0) continue;

				std::vector<glm::vec3> vertices;
				std::vector<glm::uvec3> indices;
				std::vector<glm::vec4> colors;

				glm::vec3 minBound(1e10), maxBound(-1e10);
				glm::vec4 meshColor(dis(gen), dis(gen), dis(gen), 1.0f);

				for (unsigned int j = 0; j < assimpMesh->mNumVertices; j++) {
					glm::vec3 pos(assimpMesh->mVertices[j].x, assimpMesh->mVertices[j].y, assimpMesh->mVertices[j].z);
					vertices.push_back(pos);

					minBound = glm::min(minBound, pos);
					maxBound = glm::max(maxBound, pos);
					
					if (assimpMesh->HasVertexColors(0)) {
						colors.emplace_back(assimpMesh->mColors[0][j].r, assimpMesh->mColors[0][j].g, assimpMesh->mColors[0][j].b, assimpMesh->mColors[0][j].a);
					} else {
						colors.push_back(meshColor);
					}
				}

				for (unsigned int j = 0; j < assimpMesh->mNumFaces; j++) {
					aiFace face = assimpMesh->mFaces[j];
					if (face.mNumIndices == 3) {
						indices.emplace_back(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
					}
				}

				auto mesh = std::make_shared<Mesh>();
				mesh->setMeshData(vertices, indices, colors);
				meshes.push_back(mesh);
			}

			LOG_INFO("Total mallas: " + std::to_string(meshes.size()));
			return meshes;
		}
	};
}
