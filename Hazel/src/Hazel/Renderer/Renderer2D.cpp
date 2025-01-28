#include "pch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
	struct QuadVertex
	{
		glm::vec3 Position; // 位置坐标
		glm::vec2 TexCoord; // 纹理坐标
		glm::vec4 Color;    // 颜色
	};

	static struct Renderer2DStorage
	{
		const uint32_t MaxQuads = 10000;            // 最大长方形数量
		const uint32_t MaxVertices = MaxQuads * 4;  // 最大顶点数量 -- 一个长方形由四个顶点决定
		const uint32_t MaxIndices = MaxQuads * 6;   // 最大索引数量 -- 绘制一个长方形需要六个索引

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> SimpleColorTextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

	};

	static Renderer2DStorage s_Data;

	void Renderer2D::Init()
	{
		/* 
		float squareVertices[3 * 4 + 2 * 4 + 4 * 4] = {
			// Position           // TexCoord  // Color
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f, // Vertex 0
			0.75f, -0.75f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f, // Vertex 1
			0.75f,  0.75f, 0.0f,  1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f, // Vertex 2
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f  // Vertex 3
		};

		unsigned int squareIndice[6] = { 0,1,2,2,3,0 };
		*/


		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxQuads];
		/* VB */
		s_Data.QuadVertexBuffer = Hazel::VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{Hazel::ShaderDataType::Float3, "a_Position"},
			{Hazel::ShaderDataType::Float2, "a_TexCoord"},
			{Hazel::ShaderDataType::Float4, "a_Color"},
			});
		/* EB */
		uint32_t* indices = new uint32_t[s_Data.MaxIndices];
		for (uint32_t i = 0, offset = 0; i < s_Data.MaxIndices; i += 6, offset += 4)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
		}

		Hazel::Ref<Hazel::IndexBuffer> squareEB = Hazel::IndexBuffer::Create(indices, s_Data.MaxIndices);
		delete[] indices;
		/* VA */
		s_Data.QuadVertexArray = (Hazel::VertexArray::Create());
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		s_Data.QuadVertexArray->SetIndexBuffer(squareEB);
		

		/* 纯白色纹理 */
		s_Data.WhiteTexture = Hazel::Texture2D::Create(1, 1);
		uint32_t WhiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32_t));
		/* Shader */
		s_Data.SimpleColorTextureShader = Hazel::Shader::Create("assets/shaders/SimpleColorTextureShader.glsl");
		
	}

	void Renderer2D::Shutdown()
	{

	}

	void Renderer2D::BeginScene(const OrthoGraphicsCamera& camera)
	{
		s_Data.SimpleColorTextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		HZ_CORE_INFO("DataSize:{0}", dataSize);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		/* Vertices */
		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 1.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 1.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 1.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		/* 
		s_Data.SimpleColorTextureShader->SetFloat4("u_Color", color);
		s_Data.WhiteTexture->Bind(); // 绑定纯色纹理

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.SimpleColorTextureShader->SetMat4("u_Transform", transform);

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
		*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture)
	{
		s_Data.SimpleColorTextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.SimpleColorTextureShader->SetMat4("u_Transform", transform);

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{

	}

}