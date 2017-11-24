#pragma once

#include "shader.h"

class SceneShader :
	public Shader
{
public:
	SceneShader();
	SceneShader(ShaderResource *sr);
	~SceneShader();

	virtual void Init(const std::string & vs, const std::string & fs) override;
	virtual void Update(const Transform & transform, const Camera & camera) override;
	virtual void Update(const glm::mat4 & M, const glm::mat4 & V, const glm::mat4 & P) override;
};

