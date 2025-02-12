#pragma once

#include "Hazel.h"

// 结构体: 粒子属性，描述用于创建新粒子的参数
struct ParticleProps
{
	glm::vec2 Position;             // 粒子初始位置
	glm::vec2 Velocity;             // 粒子初始速度
	glm::vec2 VelocityVariation;    // 速度变化范围
	glm::vec4 ColorBegin, ColorEnd; // 颜色（初始 & 结束）
	float SizeBegin, SizeEnd;       // 粒子大小（初始 & 结束）
	float SizeVariation;            // 粒子大小变化范围
	float LifeTime = 1.0f;          // 粒子生命周期（秒）
};

// 粒子系统类: 管理和渲染粒子
class ParticleSystem
{
public :
	ParticleSystem(uint32_t maxParticles); // 构造函数，初始化粒子池

	void OnUpdate(Hazel::Timestep ts);                        // 更新粒子状态
	void OnRender(const Hazel::OrthoGraphicsCamera& camera);  // 渲染粒子

	// 发射粒子（使用给定的属性）
	void Emit(const ParticleProps& props);

private:
	// 结构体: 具体的粒子实例（内部管理用）
	struct Particle
	{
		glm::vec2 Position;             // 当前位置
		glm::vec2 Velocity;             // 当前速度
		glm::vec4 ColorBegin, ColorEnd; // 颜色（初始 & 结束）
		float Rotation;                 // 旋转角度
		float SizeBegin, SizeEnd;       // 大小（初始 & 结束）
		float LifeTime = 1.0f;          // 总生命周期
		float LifeRemaining = 0.0f;     // 剩余寿命
		bool Active = false;            // 是否正在使用
	};

	std::vector<Particle> m_ParticlePool; // 粒子池，存储所有粒子
	uint32_t m_PoolIndex = 0;             // 当前粒子索引，用于循环使用粒子
};