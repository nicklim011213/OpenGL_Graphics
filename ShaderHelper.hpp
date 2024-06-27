#ifndef ShaderHelper
#define ShaderHelper
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

class Shader
{
public:
	const char* ShaderCode;
	const char* ShaderType;
	unsigned int ShaderResult;

	Shader(std::string Path, char Type)
	{
		std::ifstream FileHandle;
		FileHandle.open(Path);
		std::string Line;
		std::string SourceCode;

		while(std::getline(FileHandle, Line))
		{
			SourceCode += Line + "\n";
		}
		FileHandle.close();
		char* ShaderCode = new char[SourceCode.length() + 2] {};
		strcpy_s(ShaderCode, SourceCode.length() + 1, SourceCode.c_str());
		ShaderCode[SourceCode.length() + 2] = '\0';
		this->ShaderCode = ShaderCode;
		ShaderType = &Type;
	}

	bool ValidateShader()
	{
		int code;
		char infoLog[1024];
		glGetShaderiv(ShaderResult, GL_COMPILE_STATUS, &code);
		if (!code)
		{
			glGetShaderInfoLog(ShaderResult, 1024, NULL, infoLog);
			std::cerr << "Error with Shader: " << infoLog;
			return false;
		}
		return true;
	}

	bool Compile()
	{
		if (*ShaderType == 'F')
		{
			return CompileFragment();
		}
		else if (*ShaderType == 'V')
		{
			return CompileVertex();
		}
	}

	bool CompileFragment()
	{
		ShaderResult = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(ShaderResult, 1, &ShaderCode, NULL);
		glCompileShader(ShaderResult);
		return ValidateShader();
	}

	bool CompileVertex()
	{
		ShaderResult = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(ShaderResult, 1, &ShaderCode, NULL);
		glCompileShader(ShaderResult);
		return ValidateShader();
	}
};
#endif