#include "ParticleSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

ParticleSystem::ParticleSystem(uint32_t maxParticles)
	: m_PoolIndex(maxParticles - 1)  // ��ʼ������Ϊ 0
{
	m_ParticlePool.resize(maxParticles);
}

// ��������״̬
void ParticleSystem::OnUpdate(Hazel::Timestep ts)
{
	for (int i = m_ParticlePool.size() - 1; i >= 0; --i)
	{
		Particle& particle = m_ParticlePool[i];

		if (!particle.Active)
			continue;

		// ��������������ڽ���������Ϊ�ǻ
		if (particle.LifeRemaining <= 0.01f) // ��һ��С��ֵ�����⸡�����
		{
			particle.Active = false;
			particle.LifeRemaining = 0.0f; // ���׹��㣬ȷ����������
			continue;
		}

		// ����������������
		particle.LifeRemaining -= ts;

		// ����λ�ã����ٶ�Ӱ�죩
		particle.Position += particle.Velocity * (float)ts;

		// ������ת
		particle.Rotation += 0.01f * (float)ts;
	}
}

void ParticleSystem::OnRender(const Hazel::OrthoGraphicsCamera& camera)
{
	Hazel::Renderer2D::BeginScene(camera);
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active || particle.LifeRemaining <= 0.0f)
			continue;

		float life = particle.LifeRemaining / particle.LifeTime;                  // �������ӵ�ǰ�������ڱ��� (0.0 ~ 1.0)
		glm::vec4 color = glm::mix(particle.ColorEnd, particle.ColorBegin, life); // �����ֵ��ɫ���ӿ�ʼɫ���䵽����ɫ��
		float size = glm::mix(particle.SizeEnd, particle.SizeBegin, life);        // �����ֵ��С
		color.a *= life; // ��͸����Ҳ�����������ڱ仯

		// �������ӱ任����ƽ�� + ��ת + ���ţ�
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, 0.0f }) *
			glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size, size, 1.0f });

		Hazel::Renderer2D::DrawQuad(transform, color);
	}
	Hazel::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& props)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = props.Position;  
	// �������������ת�Ƕ�
	particle.Rotation = glm::linearRand(0.0f, 2.0f * glm::pi<float>());

	// ���ó�ʼ�ٶȣ���Ӧ���ٶ�����仯
	particle.Velocity = props.Velocity;
	particle.Velocity.x += props.VelocityVariation.x * (glm::linearRand(-0.5f, 0.5f));
	particle.Velocity.y += props.VelocityVariation.y * (glm::linearRand(-0.5f, 0.5f));
	
	particle.ColorBegin = props.ColorBegin;
	particle.ColorEnd = props.ColorEnd;

	particle.LifeTime = props.LifeTime;
	particle.LifeRemaining = props.LifeTime;

	particle.SizeBegin = props.SizeBegin + props.SizeVariation * (glm::linearRand(-0.5f, 0.5f));
	particle.SizeEnd = props.SizeEnd;

	// ����������ѭ�����ó��е����ӣ�
	m_PoolIndex = (m_PoolIndex - 1 + m_ParticlePool.size()) % m_ParticlePool.size();
}
