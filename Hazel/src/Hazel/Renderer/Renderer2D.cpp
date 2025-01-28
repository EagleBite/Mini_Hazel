#include "pch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
	// �����ζ���ṹ��
	struct QuadVertex
	{
		glm::vec3 Position; // λ������
		glm::vec2 TexCoord; // ��������
		glm::vec4 Color;    // ��ɫ
		float TexIndex;     // ��������
		float TilingFactor; // 
	};

	static struct Renderer2DStorage
	{
		const uint32_t MaxQuads = 10000;            // ��󳤷�������
		const uint32_t MaxVertices = MaxQuads * 4;  // ��󶥵����� -- һ�����������ĸ��������
		const uint32_t MaxIndices = MaxQuads * 6;   // ����������� -- ����һ����������Ҫ��������
		static const uint32_t MaxTextureSlots = 32; // �����������

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> SimpleColorTextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;// 0�Ÿ���ɫ����

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


		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
		/* VB */
		s_Data.QuadVertexBuffer = Hazel::VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{Hazel::ShaderDataType::Float3, "a_Position"},
			{Hazel::ShaderDataType::Float2, "a_TexCoord"},
			{Hazel::ShaderDataType::Float4, "a_Color"},
			{Hazel::ShaderDataType::Float, "a_TexIndex"},
			{Hazel::ShaderDataType::Float, "a_TilingFactor"}
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
		s_Data.QuadVertexArray = Hazel::VertexArray::Create();
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		s_Data.QuadVertexArray->SetIndexBuffer(squareEB);
		
		/* ����ɫ���� */
		s_Data.WhiteTexture = Hazel::Texture2D::Create(1, 1);
		uint32_t WhiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32_t));
		s_Data.TextureSlots[0] = s_Data.WhiteTexture; // 0�Ų۴洢����ɫ����

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}
		/* Shader */
		s_Data.SimpleColorTextureShader = Hazel::Shader::Create("assets/shaders/SimpleColorTextureShader.glsl");
		s_Data.SimpleColorTextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
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
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) 
		{
			s_Data.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		const float textureIndex = 0.0f; // ��ɫ����
		const float tilingFactor = 1.0f;

		/* Vertices */
		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr float tilingFactor = 1.0f;

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		// ����������������е�λ��
		auto it = std::find(s_Data.TextureSlots.begin(), s_Data.TextureSlots.begin() + s_Data.TextureSlotIndex, texture);

		// ����ҵ��ˣ��ͷ���������
		if (it != s_Data.TextureSlots.begin() + s_Data.TextureSlotIndex) {
			textureIndex = static_cast<float>(std::distance(s_Data.TextureSlots.begin(), it));
		}
		else {
			// ���û���ҵ�������ӵ�����۲���������
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex++] = texture;
		}

		/* Vertices */
		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

}