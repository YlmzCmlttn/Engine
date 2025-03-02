#include "Loader/MeshLoader.h"
#include "Renderer/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{
    void MeshLoader::LoadMesh(const std::string& filePath){
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            throw std::runtime_error("Failed to load mesh: " + std::string(importer.GetErrorString()));
        }
        std::cout << "Scene meshes: " << scene->mNumMeshes << std::endl;
        std::cout << "Scene nodes: " << scene->mRootNode->mNumChildren << std::endl;
        std::cout << "Scene materials: " << scene->mNumMaterials << std::endl;
        std::cout << "Scene animations: " << scene->mNumAnimations << std::endl;
        std::cout << "Scene textures: " << scene->mNumTextures << std::endl;
        std::cout << "Scene lights: " << scene->mNumLights << std::endl;
        std::cout << "Scene cameras: " << scene->mNumCameras << std::endl;
        std::cout << "Scene metadata: " << scene->mMetaData << std::endl;
        
        
        processNode(scene->mRootNode, scene);
    }

    void MeshLoader::processNode(aiNode* node, const aiScene* scene){
        std::string nodeName = node->mName.C_Str();
        std::cout << "Node name: " << nodeName << std::endl;
        std::cout << "Node meshes: " << node->mNumMeshes << std::endl;
        std::cout << "Node children: " << node->mNumChildren << std::endl;

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {            
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    void MeshLoader::processMesh(aiMesh* mesh, const aiScene* scene){
        
        std::cout<<"Mesh name: "<<mesh->mName.C_Str()<<std::endl;
        std::cout<<"Mesh vertices: "<<mesh->mNumVertices<<std::endl;
        std::cout<<"Mesh faces: "<<mesh->mNumFaces<<std::endl;
        std::cout<<"Mesh material index: "<<mesh->mMaterialIndex<<std::endl;
        
    }
}