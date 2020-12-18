#pragma once
#include "DrawableObject.h"
#include "ForwardDecl.h"
#include <memory>
#include <unordered_map>
#include "PointCloud.h"
namespace OpenGL
{
	class GLPointCloud : public DrawableObject
	{
	private:
		std::unique_ptr<VertexBuffer> mpVBO;
		std::unique_ptr<IndexBuffer> mpIBO;
		std::unique_ptr<VertexArray> mpVAO;
		std::unique_ptr<PointCloud> mpPointCloud;
		mutable std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;

	public:
		void DrawFace(const Renderer& renderer) const override;
		BBox GetBBox() const override;
		void AddPoints(const std::string& filename);
		void AddShader(const std::string& shaderName, std::shared_ptr<Shader> shader);

	};
}
