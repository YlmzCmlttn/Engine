#include "Renderer/Mesh.h"

#include "Core/Log.h"
#include "Core/Assert.h"
#include "Renderer/Renderer.h"

#include <glad/gl.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>


namespace Engine
{
    namespace{
        const unsigned int ImportFlags =
            aiProcess_Triangulate           | // Converts all primitives (quads, polygons) into triangles.
                                         // Commonly used in modern rendering pipelines as GPUs expect triangles.

            aiProcess_CalcTangentSpace      | // Calculates tangent and bitangent vectors for each vertex.
                                            // Needed for normal mapping. Might be unnecessary if your model already has tangents.

            aiProcess_SortByPType           | // Splits meshes by primitive type (point, line, triangle).
                                            // Rarely needed if you only care about triangles. It can help discard or handle lines/points.

            aiProcess_PreTransformVertices  | // Bakes all node transformations (from the scene hierarchy) into the vertices.
                                            // This flattens the model to a single coordinate space (no local transforms).
                                            // Useful if you don't want to deal with a node hierarchy or bone transforms.

            aiProcess_FlipUVs              | // Flips all UV coordinates along the Y-axis.
                                            // Helpful if the model’s UVs appear upside-down. 
                                            // (Often used when switching between coordinate systems like DirectX vs OpenGL.)

            aiProcess_GenSmoothNormals      | // Generates smooth normals for each vertex if normals are missing OR forcibly recalculates them.
                                            // This can overwrite existing normals, so be cautious. 
                                            // Use only if you want newly computed smooth normals.

            // aiProcess_GenNormals        | // Similar to GenSmoothNormals, but produces 'hard' (flat) normals.
                                            // Typically not used if you're already generating smooth normals.
                                            // (Commented out in your snippet.)

            aiProcess_OptimizeMeshes        | // Merges small meshes to reduce the overall draw calls.
                                            // Potentially useful in reducing CPU overhead during rendering,
                                            // but can break certain material/texture setups if not used carefully.

            // aiProcess_OptimizeGraph     | // Reorders and optimizes the scene graph to reduce the number of nodes.
                                            // Potentially merges or removes redundant nodes.
                                            // This can make the scene simpler, but it might break references to nodes for animation/bones.
                                            // (You have it uncommented later in the snippet.)

            // aiProcess_JoinIdenticalVertices | 
            //   Merges identical vertices within each mesh to reduce vertex count.
            //   Often used to optimize geometry. Might break if you rely on duplicate vertices (e.g., discontinuous UV seams).

            // aiProcess_ImproveCacheLocality |
            //   Reorders vertices for better GPU cache usage. Usually beneficial, but modifies vertex ordering.

            // aiProcess_RemoveRedundantMaterials |
            //   Merges materials that are visually identical. Could break specialized material setups.

            aiProcess_Debone               | // Removes bones that have minimal or no influence on vertices.
                                            // This can simplify skeletal models but may remove slight deformations.

            aiProcess_FindInvalidData      | // Searches for invalid values (e.g., NaNs) in meshes and corrects or removes them.
                                            // Useful for sanitizing data from poorly exported models.

            aiProcess_FindDegenerates      | // Detects and (optionally) removes degenerate primitives (e.g., triangles with zero area).
                                            // Use if you suspect your model has degenerate faces that cause artifacts.

            aiProcess_FindInstances        | // Searches for duplicate mesh data and replaces it with references to a single mesh.
                                            // Useful for memory savings if the same geometry is repeated.

            aiProcess_OptimizeMeshes       | // Listed again in your snippet. The same as the earlier usage—merges small meshes.
                                            // Repeated flags will have no additional effect; it’s the same operation.

            aiProcess_OptimizeGraph        | // Reorders and optimizes the node hierarchy (the scene graph).
                                            // Potentially merges or removes redundant nodes to reduce overhead.

            aiProcess_ValidateDataStructure | // Validates the imported data structure for correctness.
                                            // Helps identify import issues but can slow down import. Good for debugging.

            aiProcess_RemoveComponent;       // Removes specific data components (normals, tangents, colors, etc.) if you don't need them.
                                            // Use with caution; you might lose data you need for rendering or animation.
    }

    struct LogStream : public Assimp::LogStream
	{
		static void Initialize()
		{
			if (Assimp::DefaultLogger::isNullLogger())
			{
				Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
				Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
			}
		}

		void write(const char* message) override
		{
			ENGINE_CORE_ERROR("Assimp error: {0}", message);
		}
	};

    Mesh::Mesh(const std::string& path)
    : m_FilePath(path)
    {
        LogStream::Initialize();
        ENGINE_CORE_INFO("Importing mesh from {0}", path);

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, ImportFlags);
        if(!scene || scene->HasMeshes())
        {
            ENGINE_CORE_ERROR("Failed to load mesh from {0}", path);
            return;
        }

        // if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        // {
        //     ENGINE_CORE_ERROR("Failed to load mesh from {0}", path);
        //     return;
        // }

        aiMesh* mesh = scene->mMeshes[0];
        ENGINE_CORE_ASSERT(mesh->HasPositions(), "Mesh requires positions");

        ENGINE_CORE_ASSERT(mesh->HasNormals(), "Mesh requires normals");

        //ENGINE_CORE_ASSERT(mesh->HasTextureCoords(0), "Mesh requires texture coordinates");

        //ENGINE_CORE_ASSERT(mesh->HasFaces(), "Mesh requires faces");

        m_Vertices.resize(mesh->mNumVertices);

        for(size_t i = 0; i < m_Vertices.capacity(); i++){
            Vertex vertex;
            vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            if(mesh->HasTangentsAndBitangents())
            {
                vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
                vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            }
            if(mesh->HasTextureCoords(0))
            {
                vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            m_Vertices.push_back(vertex);
        }

        m_VertexBuffer.reset(VertexBuffer::Create());
        m_VertexBuffer->setData(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));

        m_Indices.reserve(mesh->mNumFaces);
        for(size_t i = 0; i < m_Indices.capacity(); i++){
            ENGINE_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Mesh requires triangles");
            m_Indices.push_back({mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]});
        }

        m_IndexBuffer.reset(IndexBuffer::Create());
        m_IndexBuffer->setData(m_Indices.data(), m_Indices.size() * sizeof(uint32_t));
    }

    Mesh::~Mesh(){}

    void Mesh::render()
    {
        m_VertexBuffer->bind();
        m_IndexBuffer->bind();
        Renderer::Submit([this](){
            glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, tangent));

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, bitangent));

			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));
        });
        Renderer::DrawIndexed(m_IndexBuffer->getCount());
    }
} // namespace Engine


/*
 
        aiProcess_Triangulate           | // Converts all primitives (quads, polygons) into triangles.
                                         // Commonly used in modern rendering pipelines as GPUs expect triangles.

        aiProcess_CalcTangentSpace      | // Calculates tangent and bitangent vectors for each vertex.
                                         // Needed for normal mapping. Might be unnecessary if your model already has tangents.

        aiProcess_SortByPType           | // Splits meshes by primitive type (point, line, triangle).
                                         // Rarely needed if you only care about triangles. It can help discard or handle lines/points.

        aiProcess_PreTransformVertices  | // Bakes all node transformations (from the scene hierarchy) into the vertices.
                                         // This flattens the model to a single coordinate space (no local transforms).
                                         // Useful if you don't want to deal with a node hierarchy or bone transforms.

        aiProcess_FlipUVs              | // Flips all UV coordinates along the Y-axis.
                                         // Helpful if the model’s UVs appear upside-down. 
                                         // (Often used when switching between coordinate systems like DirectX vs OpenGL.)

        aiProcess_GenSmoothNormals      | // Generates smooth normals for each vertex if normals are missing OR forcibly recalculates them.
                                         // This can overwrite existing normals, so be cautious. 
                                         // Use only if you want newly computed smooth normals.

        // aiProcess_GenNormals        | // Similar to GenSmoothNormals, but produces 'hard' (flat) normals.
                                         // Typically not used if you're already generating smooth normals.
                                         // (Commented out in your snippet.)

        aiProcess_OptimizeMeshes        | // Merges small meshes to reduce the overall draw calls.
                                         // Potentially useful in reducing CPU overhead during rendering,
                                         // but can break certain material/texture setups if not used carefully.

        // aiProcess_OptimizeGraph     | // Reorders and optimizes the scene graph to reduce the number of nodes.
                                         // Potentially merges or removes redundant nodes.
                                         // This can make the scene simpler, but it might break references to nodes for animation/bones.
                                         // (You have it uncommented later in the snippet.)

        // aiProcess_JoinIdenticalVertices | 
        //   Merges identical vertices within each mesh to reduce vertex count.
        //   Often used to optimize geometry. Might break if you rely on duplicate vertices (e.g., discontinuous UV seams).

        // aiProcess_ImproveCacheLocality |
        //   Reorders vertices for better GPU cache usage. Usually beneficial, but modifies vertex ordering.

        // aiProcess_RemoveRedundantMaterials |
        //   Merges materials that are visually identical. Could break specialized material setups.

        aiProcess_Debone               | // Removes bones that have minimal or no influence on vertices.
                                         // This can simplify skeletal models but may remove slight deformations.

        aiProcess_FindInvalidData      | // Searches for invalid values (e.g., NaNs) in meshes and corrects or removes them.
                                         // Useful for sanitizing data from poorly exported models.

        aiProcess_FindDegenerates      | // Detects and (optionally) removes degenerate primitives (e.g., triangles with zero area).
                                         // Use if you suspect your model has degenerate faces that cause artifacts.

        aiProcess_FindInstances        | // Searches for duplicate mesh data and replaces it with references to a single mesh.
                                         // Useful for memory savings if the same geometry is repeated.

        aiProcess_OptimizeMeshes       | // Listed again in your snippet. The same as the earlier usage—merges small meshes.
                                         // Repeated flags will have no additional effect; it’s the same operation.

        aiProcess_OptimizeGraph        | // Reorders and optimizes the node hierarchy (the scene graph).
                                         // Potentially merges or removes redundant nodes to reduce overhead.

        aiProcess_ValidateDataStructure | // Validates the imported data structure for correctness.
                                         // Helps identify import issues but can slow down import. Good for debugging.

        aiProcess_RemoveComponent;       // Removes specific data components (normals, tangents, colors, etc.) if you don't need them.
                                         // Use with caution; you might lose data you need for rendering or animation.

        // Makes all imported data left-handed by multiplying the x-axis by -1.
        // Use if your engine or rendering pipeline is strictly left-handed.
        const unsigned int MAKE_LEFT_HANDED         = aiProcess_MakeLeftHanded;

        // Flips face winding order from CCW to CW (or vice versa), typically used with MAKE_LEFT_HANDED.
        // Helps avoid inverted normals if you swap coordinate handedness.
        const unsigned int FLIP_WINDING_ORDER       = aiProcess_FlipWindingOrder;

        // If you want to do all the above in one step:
        // (MakeLeftHanded + FlipUVs + FlipWindingOrder).
        // Good if your entire engine uses left-handed coordinates.
        // Note: This is the combined, single-step approach.
        const unsigned int CONVERT_TO_LEFT_HANDED   = aiProcess_ConvertToLeftHanded;


        // 2) SPLIT FLAGS
        // Splitting large meshes is sometimes needed if you have GPU/hardware limits (e.g., max 65k vertices).

        // Splits large meshes into smaller submeshes if they exceed vertex limits.
        // Rarely needed on modern hardware, but can help with older limits or specific engine constraints.
        const unsigned int SPLIT_LARGE_MESHES       = aiProcess_SplitLargeMeshes;

        // Splits meshes if they exceed a certain number of bones (useful for skeletal animation on older engines).
        // Rarely needed unless your engine or GPU has strict bone-count limitations per draw call.
        const unsigned int SPLIT_BY_BONE_COUNT      = aiProcess_SplitByBoneCount;


        // 3) Vertex processing & optimization flags:

        // Joins identical vertices within each mesh to reduce vertex count.
        // Good for optimization, but can remove “seams” if your UVs or normals are discontinuous.
        // (You had it commented out, but it wasn’t active in your snippet.)
        const unsigned int JOIN_IDENTICAL_VERTICES  = aiProcess_JoinIdenticalVertices;

        // Attempts to improve GPU cache locality by reordering triangles and vertices.
        // Usually beneficial for performance, but modifies vertex ordering.
        const unsigned int IMPROVE_CACHE_LOCALITY   = aiProcess_ImproveCacheLocality;

        // Removes redundant (duplicate) materials. If multiple meshes reference the same
        // material settings, merges them into one. Potentially breaks unique material modifications.
        const unsigned int REMOVE_REDUNDANT_MATERIALS = aiProcess_RemoveRedundantMaterials;


        // 4) Bone weight / skeleton processing:

        // Limits the number of weights each vertex can have to a maximum (e.g., 4),
        // discarding any extra influences. Often needed if your GPU or engine
        // doesn’t support unlimited bone weights.
        const unsigned int LIMIT_BONE_WEIGHTS       = aiProcess_LimitBoneWeights;


        // 5) UV transformations:

        // Generates UV coordinates for primitive shapes if they are missing.
        // (E.g., parametric shapes might not have UVs.)
        // Not typically used for standard mesh imports that already have UVs.
        const unsigned int GEN_UV_COORDS            = aiProcess_GenUVCoords;

        // Applies specific transformations to UV coordinates, e.g., to handle texture mapping differently.
        // Rarely needed unless you have specialized UV transforms or a custom pipeline for them.
        const unsigned int TRANSFORM_UV_COORDS      = aiProcess_TransformUVCoords;


        // 6) Other specialized flags:

        // Force the generation of normals even if they already exist, but produce “hard/flat” normals.
        // Typically used when you want a faceted look or want to ignore the file’s normals.
        const unsigned int FORCE_GEN_NORMALS        = aiProcess_ForceGenNormals;

        // Embeds external textures (e.g., .png files) into the model data. 
        // Potentially useful if you want a single file containing both geometry and texture data.
        // Often not desired for typical real-time rendering pipelines.
        const unsigned int EMBED_TEXTURES           = aiProcess_EmbedTextures;

        // Applies a global uniform scale factor to the model.
        // Useful if your models come in at inconsistent scales.
        // But you can also handle this in-engine, so it’s not common for typical usage.
        const unsigned int GLOBAL_SCALE             = aiProcess_GlobalScale;
*/