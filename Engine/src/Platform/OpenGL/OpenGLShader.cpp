#include "Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Core/Log.h"
#include "Core/Assert.h"
#include "Renderer/ShaderUniform.h"
#include <shaderc/shaderc.hpp>

namespace {

// Maps C++ types to our engine's ShaderUniformType.
template<typename T>
struct ShaderUniformTraits;
template<>
struct ShaderUniformTraits<void*> {
    static constexpr Engine::ShaderUniformType UniformType = Engine::ShaderUniformType::VoidPtr;
};

template<>
struct ShaderUniformTraits<float> {
    static constexpr Engine::ShaderUniformType UniformType = Engine::ShaderUniformType::Float;
};

template<>
struct ShaderUniformTraits<int> {
    static constexpr Engine::ShaderUniformType UniformType = Engine::ShaderUniformType::Int;
};

template<>
struct ShaderUniformTraits<glm::vec2> {
    static constexpr Engine::ShaderUniformType UniformType = Engine::ShaderUniformType::Vec2;
};

template<>
struct ShaderUniformTraits<glm::vec3> {
    static constexpr Engine::ShaderUniformType UniformType = Engine::ShaderUniformType::Vec3;
};

template<>
struct ShaderUniformTraits<glm::vec4> {
    static constexpr Engine::ShaderUniformType UniformType = Engine::ShaderUniformType::Vec4;
};

template<>
struct ShaderUniformTraits<glm::mat3> {
    static constexpr Engine::ShaderUniformType UniformType = Engine::ShaderUniformType::Mat3;
};

template<>
struct ShaderUniformTraits<glm::mat4> {
    static constexpr Engine::ShaderUniformType UniformType = Engine::ShaderUniformType::Mat4;
};

// Provides a pointer to the underlying data.
template<typename T>
struct UniformHelper {
    static const void* GetDataPtr(const T& value) {
        return static_cast<const void*>(&value);
    }
};

// Provides a pointer to the underlying data.
template<>
struct UniformHelper<void*> {
    static const void* GetDataPtr(const void* value) {
        return value;
    }
};

// Specializations for glm types to use glm::value_ptr.
template<>
struct UniformHelper<glm::vec2> {
    static const void* GetDataPtr(const glm::vec2& value) {
        return glm::value_ptr(value);
    }
};

template<>
struct UniformHelper<glm::vec3> {
    static const void* GetDataPtr(const glm::vec3& value) {
        return glm::value_ptr(value);
    }
};

template<>
struct UniformHelper<glm::vec4> {
    static const void* GetDataPtr(const glm::vec4& value) {
        return glm::value_ptr(value);
    }
};

template<>
struct UniformHelper<glm::mat3> {
    static const void* GetDataPtr(const glm::mat3& value) {
        return glm::value_ptr(value);
    }
};

template<>
struct UniformHelper<glm::mat4> {
    static const void* GetDataPtr(const glm::mat4& value) {
        return glm::value_ptr(value);
    }
};

// Helper function to find a uniform buffer by name (searching the m_UniformBuffers map).
static const Engine::ShaderUniformBuffer* FindUniformBufferByName(
    const std::string& bufferName,
    const std::unordered_map<uint32_t, Engine::ShaderUniformBuffer>& uniformBuffers)
{
    for (auto& [binding, ub] : uniformBuffers)
    {
        if (ub.name == bufferName){
            return &ub;

		}
    }
    return nullptr;
}

} // end anonymous namespace

namespace Engine {

	static GLenum ShaderTypeFromString(const std::string& type);
	static std::optional<std::vector<uint32_t>> readBinary(std::filesystem::path path);
	static void writeBinary(std::filesystem::path path, const std::vector<uint32_t>& binary);
	static ShaderUniformType SPIRTypeToShaderUniformType(spirv_cross::SPIRType type);
	static std::string getShaderFilePrefix(GLenum type);

	OpenGLShader::OpenGLShader(const std::string& filepath):m_FilePath(filepath)
	{
		size_t found = filepath.find_last_of("/\\");
		m_Name = found != std::string::npos ? filepath.substr(found + 1) : filepath;
		found = m_Name.find_last_of(".");
		m_Name = found != std::string::npos ? m_Name.substr(0, found) : m_Name;

		reload();
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& shaderSource):m_Name(name)
	{
		load(shaderSource);
	}



	void OpenGLShader::bind()
	{
		Renderer::Submit([this]() {
			glUseProgram(m_RendererID);
		});
	}

	void OpenGLShader::unbind()
	{
		Renderer::Submit([this]() {
			glUseProgram(0);
		});
	}

	void OpenGLShader::reload()
	{
		
		std::string source = readShaderFromFile(m_FilePath);
		load(source);
	}

	void OpenGLShader::load(const std::string& shaderSource,bool forceCompile)
	{
		auto shaderSources = preProcess(shaderSource);
		if(!m_IsCompute)
		{
			Renderer::Submit([this,forceCompile,shaderSources]() {				
				this->loadShaders(shaderSources,forceCompile);
				for (auto& callback : m_ShaderReloadedCallbacks)
					callback();

			});
		}
		else
		{
			Renderer::Submit([this]()
			{
				std::cout<<"Compute shader"<<std::endl;
			});
		}
	}

	std::string OpenGLShader::readShaderFromFile(const std::string& filepath) const
	{
		std::ifstream in(filepath, std::ios::ate | std::ios::binary);
		std::string msg = "Could not load shader file: " + filepath;
		ENGINE_CORE_ASSERT(in.is_open(), msg);
		
		// Get file size and allocate string buffer
		std::streamsize size = in.tellg();
		in.seekg(0, std::ios::beg);
		std::string buffer(size, '\0');
		
		if (!in.read(&buffer[0], size))
		{
			ENGINE_CORE_ERROR("Failed to read shader file: {0}", filepath);
			ENGINE_CORE_ASSERT(false);
		}
		return buffer;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			ENGINE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			ENGINE_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel" || type == "compute", "Invalid shader type specified");
			

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			auto shaderType = ShaderTypeFromString(type);
			if(shaderSources.find(shaderType) != shaderSources.end())
			{
				ENGINE_CORE_ERROR("Shader type {0} already defined", type);
				ENGINE_CORE_ASSERT(false);
			}
			shaderSources[shaderType] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
			
			if (shaderType == GL_COMPUTE_SHADER)
			{
				m_IsCompute = true;
				break;
			}
		}

		return shaderSources;
	}


	void OpenGLShader::addShaderReloadedCallback(const ShaderReloadedCallback& callback)
	{
		m_ShaderReloadedCallbacks.push_back(callback);
	}
	void OpenGLShader::createCacheFolder()
	{
		std::filesystem::path filePath{m_FilePath};
		if(!isCacheFolderExists()) std::filesystem::create_directories(filePath.parent_path()/"cached");
	}

	bool OpenGLShader::isCacheFolderExists()
	{
		std::filesystem::path filePath{m_FilePath};
		return std::filesystem::exists(filePath.parent_path()/"cached");
	}

	void OpenGLShader::loadShaders(const std::unordered_map<GLenum, std::string>& shaderSources,bool forceCompile)
	{
		if (!isCacheFolderExists()) {
			createCacheFolder();
			forceCompile = true;
		}
		
		createProgram();
		std::vector<GLuint> shaderRendererIDs;
		for (const auto& shaderSource : shaderSources) {
			// Retrieve the Vulkan binary and decide if recompilation is needed.
			auto vulkanBinary = readVulkanBinary(shaderSource.first);
			const bool recompile = (!vulkanBinary.has_value() || forceCompile);
			if (recompile) {
				vulkanBinary = compileGLGS2SPIRV(shaderSource);
				writeVulkanBinaryToFile(vulkanBinary.value(),shaderSource.first);
			}



			std::optional<std::vector<uint32_t>> openGLBinary;
			if (!recompile) {				
				openGLBinary = readOpenGLBinary(shaderSource.first);
			}
			if (recompile || !openGLBinary.has_value()) {
				std::string openGLSource = convertSPIRV2OpenGLSource(vulkanBinary.value());
				// printf("=========================================\n");
				// printf("Shaders:\n%s\n", openGLSource.c_str());
				// printf("=========================================\n");
				openGLBinary = compileOpenGLSource2SPIRV(openGLSource,shaderSource.first);
				reflect(openGLBinary.value());
				writeOpenGLBinaryToFile(openGLBinary.value(),shaderSource.first);
			}
			shaderRendererIDs.push_back(createShader(shaderSource.first, openGLBinary.value()));
		}
		linkShaders(shaderRendererIDs);
	}

	void OpenGLShader::reflect(const std::vector<uint32_t>& binary){
		spirv_cross::CompilerGLSL glsl(binary);
		spirv_cross::CompilerGLSL::Options opts = glsl.get_common_options();
		opts.version = 450;
		opts.es = false;
		glsl.set_common_options(opts);

		// std::cout<<"========================================="<<std::endl;
		// std::cout<<glsl.compile()<<std::endl;
		// std::cout<<"========================================="<<std::endl;

		auto resources = glsl.get_shader_resources();
		for(auto& resource : resources.uniform_buffers){
			auto& bufferType = glsl.get_type(resource.base_type_id);
			int memberCount = bufferType.member_types.size();
			int binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
			//std::cout<<"Buffer name: "<<resource.name<<std::endl;

			if(m_UniformBuffers.find(binding) == m_UniformBuffers.end()){
				ShaderUniformBuffer& buffer = m_UniformBuffers[binding];
				buffer.name = resource.name;
				buffer.bindingPoint = binding;
				buffer.size = glsl.get_declared_struct_size(bufferType);
				//std::cout<<"Buffer type: "<<bufferType.basetype<<std::endl;
				//std::cout<<"Buffer size: "<<buffer.size<<std::endl;
				//std::cout<<"Buffer name: "<<buffer.name<<std::endl;
				//std::cout<<"Buffer binding point: "<<buffer.bindingPoint<<std::endl;				

				for(int i = 0; i < memberCount; i++){
					auto& memberType = glsl.get_type(bufferType.member_types[i]);
					const auto& memberName = glsl.get_member_name(bufferType.self, i);
					auto size = glsl.get_declared_struct_member_size(bufferType, i);
					auto offset = glsl.type_struct_member_offset(bufferType, i);
					//std::cout<<"Member type: "<<memberType.basetype<<std::endl;
					//std::cout<<"Member name: "<<memberName<<std::endl;
					//std::cout<<"Member size: "<<size<<std::endl;
					//std::cout<<"Member offset: "<<offset<<std::endl;

					buffer.uniforms[memberName] = ShaderUniform(memberName, SPIRTypeToShaderUniformType(memberType), size, offset);
				}

				glCreateBuffers(1, &buffer.rendererID);
				glBindBuffer(GL_UNIFORM_BUFFER, buffer.rendererID);
				glBufferData(GL_UNIFORM_BUFFER, buffer.size, nullptr, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_UNIFORM_BUFFER, buffer.bindingPoint, buffer.rendererID);

				ENGINE_CORE_TRACE("Created Uniform Buffer at binding point {0} with name '{1}', size is {2} bytes", buffer.bindingPoint, buffer.name, buffer.size);				
			}
			
		}

		// for(auto& resource : resources.storage_buffers){
		// 	std::cout<<"Storage buffer: "<<resource.name<<std::endl;
		// }

		// for(auto& resource : resources.storage_images){
		// 	std::cout<<"Storage image: "<<resource.name<<std::endl;
		// }

		for(auto& resource : resources.sampled_images){
			auto& type = glsl.get_type(resource.base_type_id);
			auto binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
			const auto& name = resource.name;
			uint32_t dimension = type.image.dim;
			// std::cout<<"Sampled image: "<<resource.name<<std::endl;
			// std::cout<<"Dimension: "<<dimension<<std::endl;
			// std::cout<<"Binding: "<<binding<<std::endl;
			// std::cout<<"Name: "<<name<<std::endl;
			GLint location = glGetUniformLocation(m_RendererID, name.c_str());
			m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);
			glUniform1i(location, binding);
		}
		// for (const spirv_cross::Resource& resource : res.sampled_images)
		// {
		// 	auto& type = comp.get_type(resource.base_type_id);
		// 	auto binding = comp.get_decoration(resource.id, spv::DecorationBinding);
		// 	const auto& name = resource.name;
		// 	uint32_t dimension = type.image.dim;

		// 	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		// 	HZ_CORE_ASSERT(location != -1);
		// 	m_Resources[name] = ShaderResourceDeclaration(name, binding, 1);
		// 	glUniform1i(location, binding);
		// }
		//And others
	}

	void OpenGLShader::createProgram(){
		if(m_RendererID) glDeleteProgram(m_RendererID);
		m_RendererID = glCreateProgram();		
	}

	GLuint OpenGLShader::createShader(GLenum type, const std::vector<uint32_t>& binary){
		auto shaderID = glCreateShader(type);
		glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, binary.data(), binary.size() * sizeof(uint32_t));
		glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
		glAttachShader(m_RendererID, shaderID);
		return shaderID;
	}

	void OpenGLShader::linkShaders(std::vector<GLuint>& shaderRendererIDs){
		glLinkProgram(m_RendererID);
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
		if(isLinked == GL_FALSE){
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);
			ENGINE_CORE_ERROR("Shader compilation failed ({0}):\n{1}", m_FilePath, &infoLog[0]);

			glDeleteProgram(m_RendererID);

			for(auto& shaderID : shaderRendererIDs){
				glDeleteShader(shaderID);
			}

			ENGINE_CORE_ASSERT(false, "Shader linking failed");
		}

		for(auto& shaderID : shaderRendererIDs){
			glDetachShader(m_RendererID, shaderID);
		}
	}
	

	static shaderc_shader_kind getShadercKind(GLenum type){
		if(type == GL_VERTEX_SHADER) return shaderc_glsl_vertex_shader;
		if(type == GL_FRAGMENT_SHADER) return shaderc_glsl_fragment_shader;
		if(type == GL_COMPUTE_SHADER) return shaderc_glsl_compute_shader;
		ENGINE_CORE_ASSERT(false, "Invalid shader type");
		return shaderc_glsl_infer_from_source;
	}

	std::vector<uint32_t> OpenGLShader::compileOpenGLSource2SPIRV(const std::string& openGLSource,GLenum type){

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		options.SetAutoMapLocations(true);
		options.SetAutoBindUniforms(true);
		options.SetPreserveBindings(false);

		options.SetOptimizationLevel(shaderc_optimization_level_zero);
		//options.SetOptimizationLevel(shaderc_optimization_level_performance);
		shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(openGLSource, getShadercKind(type), m_FilePath.c_str(), options);
		if (module.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			ENGINE_CORE_ERROR(module.GetErrorMessage());
			ENGINE_CORE_ASSERT(false);
		}
		return std::vector<uint32_t>(module.cbegin(), module.cend());
	}
	
	std::string OpenGLShader::convertSPIRV2OpenGLSource(const std::vector<uint32_t>& vulkanBinaries){
		spirv_cross::CompilerGLSL glsl(vulkanBinaries);
		spirv_cross::CompilerGLSL::Options opts = glsl.get_common_options();
		
		opts.emit_push_constant_as_uniform_buffer = true;
		opts.version = 450;
		opts.es = false;
		glsl.set_common_options(opts);		
		return glsl.compile();
	}

	std::vector<uint32_t> OpenGLShader::compileGLGS2SPIRV(const std::pair<GLenum, std::string>& shaderSource){
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		//options.SetAutoMapLocations(true);
		//options.SetAutoBindUniforms(true);
		options.SetPreserveBindings(false);
		//options.SetOptimizationLevel(shaderc_optimization_level_performance);

		//options.SetOptimizationLevel(shaderc_optimization_level_zero);

		shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(shaderSource.second, getShadercKind(shaderSource.first), m_FilePath.c_str(), options);
		if (module.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			ENGINE_CORE_ERROR(module.GetErrorMessage());
			ENGINE_CORE_ASSERT(false);
		}
		return std::vector<uint32_t>(module.cbegin(), module.cend());			
	}






	std::optional<std::vector<uint32_t>> OpenGLShader::readVulkanBinary(GLenum type){
		std::filesystem::path p = m_FilePath;
		auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_vulkan." + getShaderFilePrefix(type));
		return readBinary(path);
	}

	std::optional<std::vector<uint32_t>> OpenGLShader::readOpenGLBinary(GLenum type){
		std::filesystem::path p = m_FilePath;
		auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_opengl." + getShaderFilePrefix(type));
		return readBinary(path);
	}

	void OpenGLShader::writeVulkanBinaryToFile(const std::vector<uint32_t>& binary, GLenum type){
		std::filesystem::path p = m_FilePath;
		auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_vulkan." + getShaderFilePrefix(type));
		writeBinary(path, binary);
	}

	void OpenGLShader::writeOpenGLBinaryToFile(const std::vector<uint32_t>& binary, GLenum type){
		std::filesystem::path p = m_FilePath;
		auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_opengl." + getShaderFilePrefix(type));
		writeBinary(path, binary);
	}

	const ShaderUniform* OpenGLShader::getUniform(const std::string& name) const{
		auto pos = name.find(".");
        ENGINE_CORE_ASSERT(pos != std::string::npos, "Invalid uniform name format. Expected BufferName.UniformName");
        std::string bufferName = name.substr(0, pos);
        std::string uniformName = name.substr(pos + 1);

        // Look up the uniform buffer by name.
        const ShaderUniformBuffer* uniformBuffer = FindUniformBufferByName(bufferName, m_UniformBuffers);
        ENGINE_CORE_ASSERT(uniformBuffer, "Uniform buffer "+bufferName+" not found");

        // Find the specific uniform.
        auto it = uniformBuffer->uniforms.find(uniformName);
        ENGINE_CORE_ASSERT(it != uniformBuffer->uniforms.end(), "Uniform "+ uniformName +" not found in uniform buffer"+ bufferName);
		return &it->second;
	}
	

	void OpenGLShader::setUniformBuffer(const std::string& name, const void* data, uint32_t size)
	{
		uint8_t* buffer = new uint8_t[size];
		memcpy(buffer, data, size);

		Renderer::Submit([this, name, size, buffer]()
		{
			ShaderUniformBuffer* uniformBuffer = nullptr;
			for (auto& [bindingPoint, ub] : m_UniformBuffers)
			{
				if (ub.name == name)
				{
					uniformBuffer = &ub;
					break;
				}
			}

			ENGINE_CORE_ASSERT(uniformBuffer);
			ENGINE_CORE_ASSERT(uniformBuffer->size >= size);
			glNamedBufferSubData(uniformBuffer->rendererID, 0, size, buffer);

			delete[] buffer;
		});
	}

	template<typename T>
    void OpenGLShader::setUniformT(const std::string& fullname, const T& value)
    {
		auto pos = fullname.find(".");
        ENGINE_CORE_ASSERT(pos != std::string::npos, "Invalid uniform name format. Expected BufferName.UniformName");
        std::string bufferName = fullname.substr(0, pos);
        std::string uniformName = fullname.substr(pos + 1);

        // Look up the uniform buffer by name.
        const ShaderUniformBuffer* uniformBuffer = FindUniformBufferByName(bufferName, m_UniformBuffers);
        ENGINE_CORE_ASSERT(uniformBuffer, "Uniform buffer "+ bufferName+" not found");

        // Find the specific uniform.
        auto it = uniformBuffer->uniforms.find(uniformName);
        ENGINE_CORE_ASSERT(it != uniformBuffer->uniforms.end(), "Uniform "+ uniformName +" not found in uniform buffer "+ bufferName);
        const ShaderUniform& uniform = it->second;

        // Check that the uniform type matches the expected type.
        constexpr ShaderUniformType expectedType = ShaderUniformTraits<T>::UniformType;
        ENGINE_CORE_ASSERT(expectedType == ShaderUniformType::VoidPtr || uniform.getType() == expectedType,
                           "Uniform "+ uniformName +" in buffer "+ bufferName +" does not match expected type");

        // Submit an update command to the renderer thread.
        Renderer::Submit([this, rendererID = uniformBuffer->rendererID,
                          offset = uniform.getOffset(),
                          size = uniform.getSize(),
                          value]() {
            glNamedBufferSubData(rendererID, offset, size, UniformHelper<T>::GetDataPtr(value));
        });
    }
	void OpenGLShader::setUniform(const std::string& fullname, void* value)
	{
		setUniformT<void*>(fullname, value);
	}
    // Public overloads that call the templated function.
    void OpenGLShader::setUniform(const std::string& fullname, float value)
    {
        setUniformT<float>(fullname, value);
    }

    void OpenGLShader::setUniform(const std::string& fullname, int value)
    {
        setUniformT<int>(fullname, value);
    }

    void OpenGLShader::setUniform(const std::string& fullname, const glm::vec2& value)
    {
        setUniformT<glm::vec2>(fullname, value);
    }

    void OpenGLShader::setUniform(const std::string& fullname, const glm::vec3& value)
    {
        setUniformT<glm::vec3>(fullname, value);
    }

    void OpenGLShader::setUniform(const std::string& fullname, const glm::vec4& value)
    {
        setUniformT<glm::vec4>(fullname, value);
    }

    void OpenGLShader::setUniform(const std::string& fullname, const glm::mat3& value)
    {
        setUniformT<glm::mat3>(fullname, value);
    }

    void OpenGLShader::setUniform(const std::string& fullname, const glm::mat4& value)
    {
        setUniformT<glm::mat4>(fullname, value);
    }


	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		if (type == "compute")
			return GL_COMPUTE_SHADER;

		return GL_NONE;
	}


	static std::optional<std::vector<uint32_t>> readBinary(std::filesystem::path path){
		if(!std::filesystem::exists(path)) return std::nullopt;
		std::ifstream in(path, std::ios::binary);
		if(!in) return std::nullopt;
		in.seekg(0, std::ios::end);
		auto fileSize = in.tellg();
		in.seekg(0, std::ios::beg);
		std::vector<uint32_t> binary(fileSize / sizeof(uint32_t));
		if(!in.read(reinterpret_cast<char*>(binary.data()), fileSize)) return std::nullopt;
		in.close();
		return binary;
	}

	static void writeBinary(std::filesystem::path path, const std::vector<uint32_t>& binary){
		std::ofstream out(path, std::ios::binary);
		out.write(reinterpret_cast<const char*>(binary.data()), binary.size() * sizeof(uint32_t));
		out.close();
	}



	static ShaderUniformType SPIRTypeToShaderUniformType(spirv_cross::SPIRType type)
	{
		ShaderUniformType result = ShaderUniformType::None;
		switch (type.basetype)
		{
		case spirv_cross::SPIRType::Boolean:  return ShaderUniformType::Bool;
		case spirv_cross::SPIRType::Int:      return ShaderUniformType::Int;
		case spirv_cross::SPIRType::Float:
			if (type.vecsize == 1)            result = ShaderUniformType::Float;
			if (type.vecsize == 2)            result = ShaderUniformType::Vec2;
			if (type.vecsize == 3)            result = ShaderUniformType::Vec3;
			if (type.vecsize == 4)            result = ShaderUniformType::Vec4;

			if (type.columns == 3)            result = ShaderUniformType::Mat3;
			if (type.columns == 4)            result = ShaderUniformType::Mat4;
			break;
		}
		ENGINE_CORE_ASSERT(result != ShaderUniformType::None, "Unknown type!");
		return result;
	}
	
	
	static std::string getShaderFilePrefix(GLenum type){
		if(type == GL_VERTEX_SHADER) return "vert";
		if(type == GL_FRAGMENT_SHADER) return "frag";
		if(type == GL_COMPUTE_SHADER) return "comp";
		return "";
	}
}