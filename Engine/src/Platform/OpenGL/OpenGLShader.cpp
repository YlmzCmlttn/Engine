#include "Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Core/Log.h"
#include "Core/Assert.h"

template<typename>
struct AlwaysFalse : std::false_type {};

namespace Engine {

	OpenGLShader::OpenGLShader(const std::string& filepath):m_FilePath(filepath)
	{
		size_t found = filepath.find_last_of("/\\");
		m_Name = found != std::string::npos ? filepath.substr(found + 1) : filepath;
		reload();
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& shaderSource):m_Name(name),m_ShaderSource(shaderSource)
	{
		compileAndUploadShader();
	}

	void OpenGLShader::reload()
	{
		
		Renderer::Submit([this]() {
			if (this->m_RendererID != 0)
				glDeleteProgram(this->m_RendererID);
			this->compileAndUploadShader();
		});
	}

	void OpenGLShader::bind()
	{
		Renderer::Submit([this]() {
			glUseProgram(this->m_RendererID);
		});
	}

	void OpenGLShader::unbind()
	{
		Renderer::Submit([this]() {
			glUseProgram(0);
		});
	}

	GLenum OpenGLShader::shaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		return GL_NONE;
	}

	void OpenGLShader::compileAndUploadShader()
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = m_ShaderSource.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = m_ShaderSource.find_first_of("\r\n", pos);
			ENGINE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = m_ShaderSource.substr(begin, eol - begin);
			ENGINE_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified");

			size_t nextLinePos = m_ShaderSource.find_first_not_of("\r\n", eol);
			pos = m_ShaderSource.find(typeToken, nextLinePos);
			shaderSources[shaderTypeFromString(type)] = m_ShaderSource.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? m_ShaderSource.size() - 1 : nextLinePos));
		}

		std::vector<GLuint> shaderRendererIDs;

		GLuint program = glCreateProgram();
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			std::string& source = kv.second;

			GLuint shaderRendererID = glCreateShader(type);
			const GLchar *sourceCstr = (const GLchar *)source.c_str();
			glShaderSource(shaderRendererID, 1, &sourceCstr, 0);

			glCompileShader(shaderRendererID);

			GLint isCompiled = 0;
			glGetShaderiv(shaderRendererID, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderRendererID, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaderRendererID, maxLength, &maxLength, &infoLog[0]);

				ENGINE_CORE_ERROR("Shader compilation failed:\n{0}", &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shaderRendererID);

				ENGINE_CORE_ASSERT(false, "Failed");
			}

			shaderRendererIDs.push_back(shaderRendererID);
			glAttachShader(program, shaderRendererID);
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			ENGINE_CORE_ERROR("Shader compilation failed:\n{0}", &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : shaderRendererIDs)
				glDeleteShader(id);
		}

		// Always detach shaders after a successful link.
		for (auto id : shaderRendererIDs)
			glDetachShader(program, id);

		m_RendererID = program;


		// //Need to delete later
		// // Bind default texture unit
		// uploadUniformInt("u_Texture", 0);

		// // PBR shader textures
		// uploadUniformInt("u_AlbedoTexture", 1);
		// uploadUniformInt("u_NormalTexture", 2);
		// uploadUniformInt("u_MetalnessTexture", 3);
		// uploadUniformInt("u_RoughnessTexture", 4);

		// uploadUniformInt("u_EnvRadianceTex", 10);
		// uploadUniformInt("u_EnvIrradianceTex", 11);

		// uploadUniformInt("u_BRDFLUTTexture", 15);
	}

	void OpenGLShader::uploadUniformBuffer(const UniformBufferBase& uniformBuffer)
	{
		for (unsigned int i = 0; i < uniformBuffer.getUniformCount(); i++)
		{
			const UniformDecl& uniform = uniformBuffer.getUniforms()[i];
			switch (uniform.type)
			{
				case UniformType::Float:
				{
					const float value = *(float*)(uniformBuffer.getBuffer() + uniform.offset);
					const std::string name = uniform.name;
					Renderer::Submit([this,name,value]() {						
						this->uploadUniformFloat(name, value);
					});
					break;
				}					
				case UniformType::Float2:
				{
					const glm::vec2 value = *(glm::vec2*)(uniformBuffer.getBuffer() + uniform.offset);
					const std::string name = uniform.name;
					Renderer::Submit([this,name,value]() {
						this->uploadUniformFloat2(name, value);
					});
					break;
				}
				case UniformType::Float3:
				{
					const glm::vec3 value = *(glm::vec3*)(uniformBuffer.getBuffer() + uniform.offset);
					const std::string name = uniform.name;
					Renderer::Submit([this,name,value]() {
						this->uploadUniformFloat3(name, value);
					});
					break;
				}
				case UniformType::Float4:
				{
					const glm::vec4 value = *(glm::vec4*)(uniformBuffer.getBuffer() + uniform.offset);
					const std::string name = uniform.name;
					Renderer::Submit([this,name,value]() {
						this->uploadUniformFloat4(name, value);
					});
					break;
				}
				case UniformType::Int32:
				{
					const int value = *(int*)(uniformBuffer.getBuffer() + uniform.offset);
					const std::string name = uniform.name;
					Renderer::Submit([this,name,value]() {
						this->uploadUniformInt(name, value);
					});
					break;
				}
				case UniformType::Mat4:
				{
					const glm::mat4 value = *(glm::mat4*)(uniformBuffer.getBuffer() + uniform.offset);
					const std::string name = uniform.name;
					Renderer::Submit([this,name,value]() {
						this->uploadUniformMat4(name, value);
					});
					break;
				}
				default:
					ENGINE_CORE_ASSERT(false, "Unsupported uniform type");
			}
		}
	}

	void OpenGLShader::setFloat(const std::string& name, const float& value)
	{
		Renderer::Submit([this,name,value]() {
			this->uploadUniformFloat(name, value);
		});
	}

	void OpenGLShader::setFloat2(const std::string& name, const glm::vec2& value)
	{
		Renderer::Submit([this,name,value]() {
			this->uploadUniformFloat2(name, value);
		});
	}

	void OpenGLShader::setFloat3(const std::string& name, const glm::vec3& value)
	{
		Renderer::Submit([this,name,value]() {
			this->uploadUniformFloat3(name, value);
		});
	}

	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& value)
	{
		Renderer::Submit([this,name,value]() {
			this->uploadUniformFloat4(name, value);
		});
	}

	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& value)
	{
		Renderer::Submit([this,name,value]() {
			this->uploadUniformMat4(name, value);
		});
	}

	void OpenGLShader::uploadUniformFloat(const std::string& name, const float& value)
	{
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniform1f(location, value);
		else
			ENGINE_CORE_WARN("Uniform {0} not found", name);
	}

	void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniform2f(location, value.x, value.y);
		else
			ENGINE_CORE_WARN("Uniform {0} not found", name);
	}

	void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniform3f(location, value.x, value.y, value.z);
		else
			ENGINE_CORE_WARN("Uniform {0} not found", name);
	}

	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniform4f(location, value.x, value.y, value.z, value.w);
		else
			ENGINE_CORE_WARN("Uniform {0} not found", name);
	}

	void OpenGLShader::uploadUniformInt(const std::string& name, const int& value)
	{
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniform1i(location, value);
		else
			ENGINE_CORE_WARN("Uniform {0} not found", name);
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& value)
	{
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
		else
			ENGINE_CORE_WARN("Uniform {0} not found", name);
	}

}