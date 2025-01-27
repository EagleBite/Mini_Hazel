#include "Hazel.h"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() : 
		Layer("ExampleLayer"), m_CameraController(1280.f / 720.f)
	{
		// 绘制三角形
		float triangleVertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		unsigned int triangleIndices[3] = { 0,1,2 };
		Hazel::BufferLayout layout1 = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};
		m_TriangleVertexArray = Hazel::VertexArray::Create();
		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer = Hazel::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices));
		Hazel::Ref<Hazel::IndexBuffer> indexBuffer = Hazel::IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t));
		vertexBuffer->SetLayout(layout1);              // 先设置Layout
		m_TriangleVertexArray->AddVertexBuffer(vertexBuffer); // 后添加VertexBuffer
		m_TriangleVertexArray->SetIndexBuffer(indexBuffer);
		
		// 绘制长方形
		float squareVertices[4 * 5] = {
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
			0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
			0.75f, 0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f, 0.75f, 0.0f, 0.0f, 1.0f
		};
		unsigned int squareIndice[6] = { 0,1,2,2,3,0 };
		Hazel::BufferLayout layout2 = {
			{Hazel::ShaderDataType::Float3, "a_Position"},
			{Hazel::ShaderDataType::Float2, "a_TexCoord"}
		};
		m_SquareVertexArray = Hazel::VertexArray::Create();
		Hazel::Ref<Hazel::VertexBuffer> squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		Hazel::Ref<Hazel::IndexBuffer> squareEB = Hazel::IndexBuffer::Create(squareIndice, sizeof(squareIndice) / sizeof(uint32_t));
		squareVB->SetLayout(layout2);
		m_SquareVertexArray->AddVertexBuffer(squareVB);
		m_SquareVertexArray->SetIndexBuffer(squareEB);

		// Shaders
		m_SimpleShader = Hazel::Shader::Create("assets/shaders/SimpleShader.glsl");
		m_SimpleTextureShader = Hazel::Shader::Create("assets/shaders/SimpleTextureShader.glsl");
		m_SimpleColorShader = Hazel::Shader::Create("assets/shaders/SimpleColorShader.glsl");

		// Textures
		m_Texture = Hazel::Texture2D::Create("assets/textures/Brick_159S.jpg");
		m_Texture->Bind(0);
		
	}
	void OnUpdate(Hazel::Timestep ts) override 
	{
		m_CameraController.OnUpdate(ts);

		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SimpleColorShader)->UploadUniform4f("u_Color", m_SquareColor);
		// 绘制一组正方形
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), { 0.05f, 0.05f, 0.05f });
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				glm::mat4 smallsqtransfrom = glm::translate(glm::mat4(1.0f), { i * 0.08f, j * 0.08f, 0.0f }) * scale;
				Hazel::Renderer::Submit(m_SimpleColorShader, m_SquareVertexArray, smallsqtransfrom);
			}
		}

		Hazel::Renderer::Submit(m_SimpleTextureShader, m_SquareVertexArray); // 绘制正方形
		Hazel::Renderer::Submit(m_SimpleShader, m_TriangleVertexArray);
		Hazel::Renderer::EndScene();
	}

	void OnEvent(Hazel::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
private:
	Hazel::Ref<Hazel::VertexArray> m_TriangleVertexArray;
	Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;

	Hazel::Ref<Hazel::Shader> m_SimpleColorShader;
	Hazel::Ref<Hazel::Shader> m_SimpleTextureShader;
	Hazel::Ref<Hazel::Shader> m_SimpleShader;

	Hazel::Ref<Hazel::Texture2D> m_Texture;

	glm::vec4 m_SquareColor = glm::vec4(1, 0, 0, 1);

	Hazel::OrthographicsCameraController m_CameraController;
};

class SandBoxApplication : public Hazel::Application
{
public:
	SandBoxApplication() : Application()
	{
		PushLayer(new ExampleLayer());
	}
	~SandBoxApplication()
	{

	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBoxApplication();
}
