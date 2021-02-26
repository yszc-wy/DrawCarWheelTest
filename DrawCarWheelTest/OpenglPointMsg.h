#pragma once
#include <vector>

#include "Vertex.h"
namespace OpenglObj {
	const float kPIR = 3.1415926f;
	class OpenglPointMsg
	{
	public:
		OpenglPointMsg() :vertices(), indices() {}
		OpenglPointMsg(const std::vector<Vertex> &vertices, const std::vector<unsigned int>& indices)
			:vertices(vertices), indices(indices) {}
		OpenglPointMsg(std::vector<Vertex> &&overtices, std::vector<unsigned int>&& oindices) noexcept
			: vertices(overtices), indices(oindices)
		{}
		const std::vector<Vertex>& GetVertices() const;
		const std::vector<unsigned int>& GetIndices() const;
		void SetMsg_Move(std::vector<Vertex>&& overtices, std::vector<unsigned int>&& oindices);
		OpenglPointMsg& operator+=(const OpenglPointMsg& rhs);
		void AddVertice(const Vertex& vertex);
		void AddIndice(unsigned int indice);
		//size_t GetVerticesSize() const;
	private:
		//void AddjustIndices(unsigned int start_id);

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
	};
	OpenglPointMsg operator+(const OpenglPointMsg &lhs, const OpenglPointMsg &rhs);
	OpenglPointMsg LoadCircle(const glm::vec3& circle_center, float radius, int section,const glm::vec3& normal, const glm::vec3& color);
}