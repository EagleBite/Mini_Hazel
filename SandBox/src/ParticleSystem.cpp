#include "ParticleSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

ParticleSystem::ParticleSystem(uint32_t maxParticles)
	: m_PoolIndex(maxParticles - 1)  // 初始化索引为 0
{
	m_ParticlePool.resize(maxParticles);
}

// 更新粒子状态
void ParticleSystem::OnUpdate(Hazel::Timestep ts)
{
	for (int i = m_ParticlePool.size() - 1; i >= 0; --i)
	{
		Particle& particle = m_ParticlePool[i];

		if (!particle.Active)
			continue;

		// 如果粒子生命周期结束，则标记为非活动
		if (particle.LifeRemaining <= 0.01f) // 给一个小阈值，避免浮点误差
		{
			particle.Active = false;
			particle.LifeRemaining = 0.0f; // 彻底归零，确保不会误判
			continue;
		}

		// 更新粒子生命周期
		particle.LifeRemaining -= ts;

		// 更新位置（受速度影响）
		particle.Position += particle.Velocity * (float)ts;

		// 更新旋转
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

		float life = particle.LifeRemaining / particle.LifeTime;                  // 计算粒子当前生命周期比率 (0.0 ~ 1.0)
		glm::vec4 color = glm::mix(particle.ColorEnd, particle.ColorBegin, life); // 计算插值颜色（从开始色渐变到结束色）
		float size = glm::mix(particle.SizeEnd, particle.SizeBegin, life);        // 计算插值大小
		color.a *= life; // 让透明度也跟着生命周期变化

		// 计算粒子变换矩阵（平移 + 旋转 + 缩放）
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
	// 赋予粒子随机旋转角度
	particle.Rotation = glm::linearRand(0.0f, 2.0f * glm::pi<float>());

	// 设置初始速度，并应用速度随机变化
	particle.Velocity = props.Velocity;
	particle.Velocity.x += props.VelocityVariation.x * (glm::linearRand(-0.5f, 0.5f));
	particle.Velocity.y += props.VelocityVariation.y * (glm::linearRand(-0.5f, 0.5f));
	
	particle.ColorBegin = props.ColorBegin;
	particle.ColorEnd = props.ColorEnd;

	particle.LifeTime = props.LifeTime;
	particle.LifeRemaining = props.LifeTime;

	particle.SizeBegin = props.SizeBegin + props.SizeVariation * (glm::linearRand(-0.5f, 0.5f));
	particle.SizeEnd = props.SizeEnd;

	// 递增索引（循环复用池中的粒子）
	m_PoolIndex = (m_PoolIndex - 1 + m_ParticlePool.size()) % m_ParticlePool.size();
}
