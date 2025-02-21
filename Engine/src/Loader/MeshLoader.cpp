// #include "Loader/MeshLoader.h"
// #include "Renderer/Mesh.h"
// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>

// namespace Engine
// {
//     Entity MeshLoader::LoadMesh(const std::string& filePath){
//         Entity entityRoot = Entity();
        
//         Assimp::Importer importer;
//         const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
//         if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//         {
//             throw std::runtime_error("Failed to load mesh: " + std::string(importer.GetErrorString()));
//         }
//         ProcessNode(entityRoot,scene->mRootNode, scene);
//         return entityRoot;
//     }

//     void MeshLoader::ProcessNode(Entity rootEntity,aiNode* node, const aiScene* scene){
//         for (unsigned int i = 0; i < node->mNumMeshes; i++)
//         {
//             std::string nodeName = node->mName.C_Str();
//             std::cout << "Node name: " << nodeName << std::endl;
//             aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//             ProcessMesh(mesh, scene);
//         }
//         for (unsigned int i = 0; i < node->mNumChildren; i++)
//         {
//             std::string childName = node->mChildren[i]->mName.C_Str();
//             std::cout << "Child name: " << childName << std::endl;
//             ProcessNode(node->mChildren[i], scene);
//         }
//     }

//     void MeshLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene){
//         std::vector<Mesh::Vertex> vertices;
//         std::vector<unsigned int> indices;

//         for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//         {
//             Mesh::Vertex vertex;
//             vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
//             vertices.push_back(vertex);
//         }

//         for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//         {
//             aiFace face = mesh->mFaces[i];
//             for (unsigned int j = 0; j < face.mNumIndices; j++)
//             {
//                 indices.push_back(face.mIndices[j]);
//             }
//         }

//         std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertices, indices);
//     }
// }