#pragma once
#include "shader.h"
class BasicShader :
	public Shader
{
public:
	BasicShader();
	~BasicShader();

	virtual void Init(const std::string & filename) override;
	virtual void Update(const Transform & transform, const Camera & camera) override;
	virtual void Update(const glm::mat4 & M, const glm::mat4 & V, const glm::mat4 & P) override;
};

