#pragma once
#include "Scene/Entity.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

namespace Engine
{
    class MeshLoader
    {
    public:
        void LoadMesh(const std::string& filePath);
        static MeshLoader& GetInstance()
        {
            static MeshLoader instance;
            return instance;
        }
    private:
        MeshLoader() = default;
        MeshLoader(const MeshLoader&) = delete;
        MeshLoader& operator=(const MeshLoader&) = delete;
        void processNode(aiNode* node, const aiScene* scene);
        void processMesh(aiMesh* mesh, const aiScene* scene);
    };
}
