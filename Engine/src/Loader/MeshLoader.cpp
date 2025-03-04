#include "Loader/MeshLoader.h"
#include "Renderer/Mesh.h"
#include "Scene/Systems.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Loader/TextureLoader.h"
namespace Engine
{

    glm::mat4 Mat4FromAssimpMat4(const aiMatrix4x4& matrix)
    {
        glm::mat4 result;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
        result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
        result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
        result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
        return result;
    }
    static void DecomposeTransform(const glm::mat4& transform, glm::vec3& position, glm::quat& rotation, glm::vec3& scale) {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform, scale, rotation, position, skew, perspective);
    }


    Entity MeshLoader::LoadMeshToScene(const std::string& filePath, Ref<Scene> scene) {
        LoadModel(filePath);
        auto model = m_Models[filePath];
        return processNode(model->rootNode, scene->getSceneEntity(),model->materials);
    }

    Entity MeshLoader::processNode(std::shared_ptr<NodeData> node, Entity parent,std::vector<Ref<Material>>& materials) {
        std::string nodeName = node->name;
        Entity nodeEntity = parent.getScene()->createEntity(nodeName);
        Systems::SetParent(nodeEntity, parent);
        
        nodeEntity.setTransform(node->position, node->rotation, node->scale);
        for (unsigned int i = 0; i < node->meshes.size(); i++)
        {
            std::cout << node->meshes[i]->mesh->name << std::endl;
            nodeEntity.addComponent<MeshComponent>().mesh = node->meshes[i]->mesh;
            nodeEntity.addComponent<MeshRendererComponent>(materials[node->meshes[i]->materialIndex]);
            if (!node->meshes[i]->mesh->isUploadedToGPU()) { node->meshes[i]->mesh->uploadToGPU(); }
        }
        for (unsigned int i = 0; i < node->childNodes.size(); i++)
        {
            processNode(node->childNodes[i], nodeEntity, materials);
        }
        return nodeEntity;
    }

    void MeshLoader::LoadModel(const std::string& filePath) {
        if (m_Models.find(filePath) != m_Models.end()) {
            std::cout << "Model is already loaded" << std::endl;
            return;
        }


        static const uint32_t s_MeshImportFlags =
            aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
            aiProcess_Triangulate |             // Make sure we're triangles
            aiProcess_SortByPType |             // Split meshes by primitive type
            aiProcess_GenNormals |              // Make sure we have legit normals
            aiProcess_GenUVCoords |             // Convert UVs if required 
            aiProcess_OptimizeMeshes |          // Batch draws where possible
            aiProcess_ValidateDataStructure;    // Validation
        //Get filename from path;
        Assimp::Importer importer;
        const aiScene* aiScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!aiScene || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode)
        {
            throw std::runtime_error("Failed to load mesh: " + std::string(importer.GetErrorString()));
        }

        std::shared_ptr<Model> newModel = std::make_shared<Model>();
        newModel->filePath = filePath;
        newModel->rootNode = std::make_shared<NodeData>();

        std::cout<<"Number of materials: "<<aiScene->mNumMaterials<<std::endl;
        for(uint32_t i=0;i<aiScene->mNumMaterials;i++){
            auto material = aiScene->mMaterials[i];
            auto newMaterial = Material::Create(m_Shader);
            newModel->materials.push_back(newMaterial);
        }
        

        m_Models[filePath] = newModel;
        std::unordered_map<unsigned int, std::shared_ptr<MeshData>> meshStorage;
        processNode(aiScene->mRootNode, aiScene, newModel->rootNode,meshStorage,newModel->materials,filePath);
    }

    void MeshLoader::processNode(aiNode* node, const aiScene* aiScene, std::shared_ptr<NodeData> nodeData, std::unordered_map<unsigned int, std::shared_ptr<MeshData>>& meshStorage,std::vector<Ref<Material>>& materials,const std::string& filePath) {
        nodeData->name = node->mName.C_Str();
        nodeData->childNodes.resize(node->mNumChildren);
        nodeData->meshes.resize(node->mNumMeshes);

        glm::mat4 nodeTransform = Mat4FromAssimpMat4(node->mTransformation);

        glm::vec3 position, scale;
        glm::quat rotation;
        DecomposeTransform(nodeTransform, position, rotation, scale);

        nodeData->position = position;
        nodeData->rotation = glm::eulerAngles(rotation);
        nodeData->scale = scale;
        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            if (meshStorage.find(node->mMeshes[i]) != meshStorage.end()) {
                nodeData->meshes[i] = meshStorage[node->mMeshes[i]];
                continue;
            }
            auto newMeshData = std::make_shared<MeshData>();
            auto newMesh = std::make_shared<Mesh>();

            newMeshData->uuid = UUID::Generate();
            newMesh->name = nodeData->name;
            newMeshData->mesh = newMesh;
            m_MeshLibrary[newMeshData->uuid] = newMesh;
            newMeshData->materialIndex = aiScene->mMeshes[node->mMeshes[i]]->mMaterialIndex;
            aiMesh* mesh = aiScene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, aiScene,newMesh,materials,filePath);
            nodeData->meshes[i] = newMeshData;
            meshStorage[node->mMeshes[i]] = newMeshData;
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++)
        {
            nodeData->childNodes[i] = std::make_shared<NodeData>();
            processNode(node->mChildren[i], aiScene, nodeData->childNodes[i], meshStorage,materials,filePath);
        }
    }

    void MeshLoader::processMesh(aiMesh* aiMesh, const aiScene* scene,std::shared_ptr<Mesh> mesh,std::vector<Ref<Material>>& materials,const std::string& filePath) {
        auto& vertices = mesh->getVertices();
        auto& indices = mesh->getIndices();
        vertices.resize(aiMesh->mNumVertices);
        indices.resize(aiMesh->mNumFaces*3);

        for (uint32_t i = 0; i < aiMesh->mNumVertices; i++) {
            Mesh::Vertex vertex;
            vertex.position.x = aiMesh->mVertices[i].x;
            vertex.position.y = aiMesh->mVertices[i].y;
            vertex.position.z = aiMesh->mVertices[i].z;

            vertex.uv.x = aiMesh->mTextureCoords[0][i].x;
            vertex.uv.y = aiMesh->mTextureCoords[0][i].y;

            vertices[i] = vertex;
            //vertex.uv later
        }
        for (uint32_t i = 0; i < aiMesh->mNumFaces; i++) {
            aiFace face = aiMesh->mFaces[i];
            indices[i * 3 + 0] = face.mIndices[0];
            indices[i * 3 + 1] = face.mIndices[1];
            indices[i * 3 + 2] = face.mIndices[2];
        }

        if(aiMesh->mMaterialIndex >= 0){
            aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];

            if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0){
                aiString path;
                material->GetTexture(aiTextureType_DIFFUSE,0,&path);
                std::string texturePath = path.C_Str();
                std::cout<<"Texture path: "<<texturePath<<std::endl;
                //Check that the file exists
                if(std::filesystem::exists(texturePath)){
                    std::cout<<"Texture found in path"<<std::endl;
                    //Ref<Texture2D> texture = Texture2D::Create(texturePath);
                    Ref<Texture2D> texture = TextureLoader::GetInstance().LoadTexture(texturePath);
                    Ref<Material> material = materials[aiMesh->mMaterialIndex];
                    Renderer::Submit([material,texture]() {                        
                        material->set("u_Texture",texture);
                    });

                }else{
                    std::cout<<"Texture not found"<<std::endl;
                    //Check inside model path
                    std::cout<<"File path: "<<filePath<<std::endl;
                    std::string modelPath = std::filesystem::path(filePath).parent_path().string();
                    std::cout<<"Model path: "<<modelPath<<std::endl;
                    std::string texturePathBasedOnModelPath = modelPath + "/" + texturePath;
                    if(std::filesystem::exists(texturePathBasedOnModelPath)){
                        std::cout<<"Texture found inside model path"<<std::endl;
                        //Ref<Texture2D> texture = Texture2D::Create(texturePathBasedOnModelPath);
                        Ref<Texture2D> texture = TextureLoader::GetInstance().LoadTexture(texturePathBasedOnModelPath);
                        Ref<Material> material = materials[aiMesh->mMaterialIndex];
                        Renderer::Submit([material,texture]() {                        
                            material->set("u_Texture",texture);
                        });
                    }else{
                        std::cout<<"Texture not found in model path:"<<texturePathBasedOnModelPath<<std::endl;
                    }
                }
            }else{
                std::cout<<"No texture found"<<std::endl;
            }
        }else{
            std::cout<<"No material found"<<std::endl;
        }
        

    }

}