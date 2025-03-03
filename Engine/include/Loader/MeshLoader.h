#pragma once
#include "Scene/Entity.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"

namespace Engine
{
    class MeshLoader
    {

        struct MeshData {
            UUID uuid;
            Ref<Mesh> mesh;
        };
        struct NodeData {
            std::string name;
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;
            std::vector<Ref<MeshData>> meshes;
            std::vector<Ref<NodeData>> childNodes;
        };
        struct Model {
            std::string filePath;
            std::shared_ptr<NodeData> rootNode;
        };
    public:
        void LoadModel(const std::string& filePath);
        Entity LoadMeshToScene(const std::string& filePath,Ref<Scene> scene);
        static MeshLoader& GetInstance()
        {
            static MeshLoader instance;
            instance.m_Shader = Shader::CreateFromFile("BasicS", "C:/Users/ylmzc/Desktop/Engine/Editor/assets/shaders/basic.glsl");
            instance.m_Material = Material::Create(instance.m_Shader);
            return instance;
        }
    private:
        MeshLoader() = default;
        MeshLoader(const MeshLoader&) = delete;
        MeshLoader& operator=(const MeshLoader&) = delete;
        Entity processNode(std::shared_ptr<NodeData> node, Entity parent);
        void processNode(aiNode* node, const aiScene* aiScene, std::shared_ptr<NodeData> nodeData, std::unordered_map<unsigned int, std::shared_ptr<MeshData>>& meshStorage);
        void processMesh(aiMesh* aiMesh, const aiScene* scene, std::shared_ptr<Mesh> mesh);
        std::unordered_map<UUID, std::shared_ptr<Mesh>> m_MeshLibrary;
        std::unordered_map<std::string, std::shared_ptr<Model>> m_Models;
        Ref<Shader> m_Shader;
        Ref<Material> m_Material;
    };
}
