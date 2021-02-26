#pragma once
#include <memory>

#include "DxfIPolygon.h"
namespace DxfReader {
	class Polygon {//�����ʹ��������������˸�ֵ�����ʼ�������ṩ�����ĸ�ֵ�����ӿ�
	public:
		Polygon();//�յ�ֵ���˸�ֵ���ⶼ������
		Polygon(const std::shared_ptr<IPolygon>& p_ipolygon, Direction direction);
		Polygon(const Polygon& polygon);
		Polygon& operator=(const Polygon& polygon);
		//����Ҳû�п���ֱ���޸����ֵ�ĺ���
		//Point get_start_point() const;
		//Point get_end_point() const;
		Direction get_direction() const;
		void set_direction(Direction direction);
		void ReverseDirection();
		const std::shared_ptr<IPolygon>& get_p_ipolygon() const;
		void Show() const;
		std::string Get_Log() const;
	private:
		//void LoadSEPoint();

		std::shared_ptr<IPolygon> p_ipolygon;
		//Point start_point;//���������Ԥ����һ��
		//Point end_point;

		Direction direction;
	};
}