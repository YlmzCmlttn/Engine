#pragma once

#include "Core/Core.h"


namespace Engine {

	enum class ShaderDomain
	{
		None = 0, Vertex = 0, Pixel = 1
	};

	class ShaderResourceDeclaration
	{
	public:
		ShaderResourceDeclaration() = default;
		ShaderResourceDeclaration(const std::string& name, uint32_t resourceRegister, uint32_t count)
			: m_Name(name), m_Register(resourceRegister), m_Count(count) {}

		virtual const std::string& getName() const { return m_Name; }
		virtual uint32_t getRegister() const { return m_Register; }
		virtual uint32_t getCount() const { return m_Count; }
	private:
		std::string m_Name;
		uint32_t m_Register = 0;
		uint32_t m_Count = 0;
	};

	typedef std::vector<Ref<ShaderResourceDeclaration>> ShaderResourceList;

	enum class ShaderUniformType
	{
		None = 0, Bool, Int, Float, Vec2, Vec3, Vec4, Mat3, Mat4, VoidPtr
	};

	enum class ShaderBufferType
	{
		None = 0, UniformBuffer, PushConstant
	};

	class ShaderUniform
	{
	public:
		ShaderUniform() = default;
		ShaderUniform(const std::string& name, ShaderUniformType type, uint32_t size, uint32_t offset);

		const std::string& getName() const { return m_Name; }
		ShaderUniformType getType() const { return m_Type; }
		uint32_t getSize() const { return m_Size; }
		uint32_t getOffset() const { return m_Offset; }

		static const std::string& UniformTypeToString(ShaderUniformType type);
	private:
		std::string m_Name;
		ShaderUniformType m_Type = ShaderUniformType::None;
		uint32_t m_Size = 0;
		uint32_t m_Offset = 0;
	};

	struct ShaderUniformBuffer
	{
		std::string name;
		uint32_t bindingPoint;
		uint32_t size;
		uint32_t rendererID;
		std::unordered_map<std::string, ShaderUniform> uniforms;		
	};

	struct ShaderBuffer
	{
		std::string name;
		uint32_t size = 0;
		std::unordered_map<std::string, ShaderUniform> uniforms;
	};
}
