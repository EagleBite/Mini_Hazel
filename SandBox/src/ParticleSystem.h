#pragma once

#include "Hazel.h"

// �ṹ��: �������ԣ��������ڴ��������ӵĲ���
struct ParticleProps
{
	glm::vec2 Position;             // ���ӳ�ʼλ��
	glm::vec2 Velocity;             // ���ӳ�ʼ�ٶ�
	glm::vec2 VelocityVariation;    // �ٶȱ仯��Χ
	glm::vec4 ColorBegin, ColorEnd; // ��ɫ����ʼ & ������
	float SizeBegin, SizeEnd;       // ���Ӵ�С����ʼ & ������
	float SizeVariation;            // ���Ӵ�С�仯��Χ
	float LifeTime = 1.0f;          // �����������ڣ��룩
};

// ����ϵͳ��: �������Ⱦ����
class ParticleSystem
{
public :
	ParticleSystem(uint32_t maxParticles); // ���캯������ʼ�����ӳ�

	void OnUpdate(Hazel::Timestep ts);                        // ��������״̬
	void OnRender(const Hazel::OrthoGraphicsCamera& camera);  // ��Ⱦ����

	// �������ӣ�ʹ�ø��������ԣ�
	void Emit(const ParticleProps& props);

private:
	// �ṹ��: ���������ʵ�����ڲ������ã�
	struct Particle
	{
		glm::vec2 Position;             // ��ǰλ��
		glm::vec2 Velocity;             // ��ǰ�ٶ�
		glm::vec4 ColorBegin, ColorEnd; // ��ɫ����ʼ & ������
		float Rotation;                 // ��ת�Ƕ�
		float SizeBegin, SizeEnd;       // ��С����ʼ & ������
		float LifeTime = 1.0f;          // ����������
		float LifeRemaining = 0.0f;     // ʣ������
		bool Active = false;            // �Ƿ�����ʹ��
	};

	std::vector<Particle> m_ParticlePool; // ���ӳأ��洢��������
	uint32_t m_PoolIndex = 0;             // ��ǰ��������������ѭ��ʹ������
};