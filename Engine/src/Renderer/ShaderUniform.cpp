#include "Renderer/ShaderUniform.h"

namespace Engine {

	ShaderUniform::ShaderUniform(const std::string& name, ShaderUniformType type, uint32_t size, uint32_t offset)
		: m_Name(name), m_Type(type), m_Size(size), m_Offset(offset)
	{
	}

	const std::string& ShaderUniform::UniformTypeToString(ShaderUniformType type)
	{
		if (type == ShaderUniformType::Bool)
		{
			return "Boolean";
		}
		else if (type == ShaderUniformType::Int)
		{
			return "Int";
		}
		else if (type == ShaderUniformType::Float)
		{
			return "Float";
		}

		return "None";
	}


}
