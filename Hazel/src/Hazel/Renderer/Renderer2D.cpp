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
		// ����
		static const uint32_t MaxQuads = 10000;            // ��󳤷�������
		static const uint32_t MaxVertices = MaxQuads * 4;  // ��󶥵����� -- һ�����������ĸ��������
		static const uint32_t MaxIndices = MaxQuads * 6;   // ����������� -- ����һ����������Ҫ��������
		static const uint32_t MaxTextureSlots = 32;        // �����������
		// ������Դ
		Ref<VertexArray> QuadVertexArray;     // ��������
		Ref<VertexBuffer> QuadVertexBuffer;   // ���㻺��
		Ref<Shader> SimpleColorTextureShader; // ��ɫ����Դ
		Ref<Texture2D> WhiteTexture;          // ������Դ
		// ��������
		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr; // ���ٶ������ݴ洢�ռ�
		QuadVertex* QuadVertexBufferPtr = nullptr;  // ���ٵ�ǰ�������ݴ洢�ռ�ʹ�����

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;// 0�Ÿ���ɫ����

		glm::vec4 QuadVertexPositions[4];
		Renderer2D::Statistics Stats;


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

		s_Data.QuadVertexPositions[0] = { 0.0f, 0.0f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 1.0f, 0.0f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 1.0f, 1.0f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { 0.0f, 1.0f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthoGraphicsCamera& camera)
	{
		s_Data.SimpleColorTextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0)
			return;

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) 
		{
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		Flush();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	float Renderer2D::GetTextureIndex(const Ref<Texture2D>& texture)
	{
		if (!texture) return 0.0f; // Ĭ�ϰ�ɫ����

		auto it = std::find(s_Data.TextureSlots.begin(), s_Data.TextureSlots.begin() + s_Data.TextureSlotIndex, texture);
		if (it != s_Data.TextureSlots.begin() + s_Data.TextureSlotIndex)
		{
			return static_cast<float>(std::distance(s_Data.TextureSlots.begin(), it));
		}
		else
		{
			float textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex++] = texture;
			return textureIndex;
		}
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
		{
			FlushAndReset();
		}

		const float textureIndex = 0.0f; // ��ɫ����
		const float tilingFactor = 1.0f;

		/* Vertices */
		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = position + glm::vec3(s_Data.QuadVertexPositions[i]) * glm::vec3(size, 1.0f);
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { (i % 2) ? 1.0f : 0.0f, (i / 2) ? 1.0f : 0.0f };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
		{
			FlushAndReset();
		}

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr float tilingFactor = 1.0f;
		float textureIndex = GetTextureIndex(texture);
		
		/* Vertices */
		// �����ĸ������ƫ��������������
		constexpr glm::vec3 offsets[4] = {
			{ 0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 0.0f },
			{ 1.0f, 1.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f }
		};

		constexpr glm::vec2 texCoords[4] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		// ʹ��ѭ����䶥������
		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = { position.x + size.x * offsets[i].x, position.y + size.y * offsets[i].y, 0.0f };
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
			FlushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		constexpr float textureIndex = 0.0f;
		constexpr float tilingFactor = 1.0f;

		constexpr glm::vec4 quadVertexPositions[4] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f, 1.0f },
			{ 1.0f, 1.0f, 0.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f }
		};

		constexpr glm::vec2 texCoords[4] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * quadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture)
	{
		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
			FlushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		constexpr float tilingFactor = 1.0f;
		float textureIndex = GetTextureIndex(texture);

		constexpr glm::vec4 quadVertexPositions[4] = {
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};
		constexpr glm::vec2 texCoords[4] = { 
			{ 0.0f, 0.0f }, 
			{ 1.0f, 0.0f }, 
			{ 1.0f, 1.0f }, 
			{ 0.0f, 1.0f } 
		};

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * quadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = glm::vec4(1.0f);
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
		{
			FlushAndReset();
		}

		const float textureIndex = 0.0f; // Ĭ��ʹ�ð�ɫ����
		const float tilingFactor = 1.0f;

		// ����ѭ�������¡����¡����ϡ����ϣ�
		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = { (i % 2) ? 1.0f : 0.0f, (i / 2) ? 1.0f : 0.0f };
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6; // ÿ���ı���ʹ�� 6 ������
		s_Data.Stats.QuadCount++;   // ͳ���ѻ��Ƶ� Quad ����
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

}