#include "hzpch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// 1.1.����������ɫ������
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		// 1.2.���Ӷ�����ɫ��Դ�뵽������ɫ��������
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);
		// 1.3.���붥����ɫ������
		glCompileShader(vertexShader);
		// 1.4.����Ƿ����ɹ�
		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			// 1.4.2����ʧ�ܿ��Դ�ӡ������Ϣ
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
			// We don't need the shader anymore.
			glDeleteShader(vertexShader);
			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}
		// Ƭ����ɫ��һ��
		// 2.1.����Ƭ����ɫ������
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		// 2.2.����Ƭ����ɫ��Դ�뵽Ƭ����ɫ��������
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);
		// 2.3.����Ƭ����ɫ������
		glCompileShader(fragmentShader);
		// 2.4.����Ƿ����ɹ�
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			// 2.4.2����ʧ�ܿ��Դ�ӡ������Ϣ
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);
			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		// 3.1������ɫ���������
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;
		// 3.2������ɫ���������ɫ���������
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		// 3.3������ɫ���������
		glLinkProgram(program);
		// 3.4���Լ�������Ƿ�ɹ�
		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader link failure!");
			return;
		}
		// 4.ɾ����ɫ������
		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}