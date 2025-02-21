// #pragma once
// #include "Scene/Entity.h"
// namespace Engine
// {
//     class MeshLoader
//     {
//     public:
//         Entity LoadMesh(const std::string& filePath);
//         std::shared_ptr<Mesh> GetMesh(const std::string& filePath);
//         static MeshLoader& GetInstance()
//         {
//             static MeshLoader instance;
//             return instance;
//         }
//     private:
//         MeshLoader() = default;
//         MeshLoader(const MeshLoader&) = delete;
//         MeshLoader& operator=(const MeshLoader&) = delete;
//         std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Meshes;
//     }
// }
