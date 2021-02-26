#pragma once
#include <memory>
#include <list>

#include "CadLib/dl_creationadapter.h"
#include "DxfPolygon.h"
#include "DxfStandardFlangeTreadMsgMaker.h"
#include "DxfFloatPlantMsgMaker.h"
namespace DxfReader {

	class DxfReader : public DL_CreationAdapter {
	public:
		static const double kMinPointDis;

		DxfReader();

		virtual void addLayer(const DL_LayerData& data);
		virtual void addPoint(const DL_PointData& data);
		virtual void addLine(const DL_LineData& data);
		virtual void addArc(const DL_ArcData& data);
		virtual void addCircle(const DL_CircleData& data);
		virtual void addPolyline(const DL_PolylineData& data);
		virtual void addVertex(const DL_VertexData& data);
		virtual void add3dFace(const DL_3dFaceData& data);
		
		//void printAttributes();
		StandardFlangeTreadMsgMaker LoadSFTMsgMaker();
		FloatPlantMsgMaker LoadFPMsgMaker();
		std::string get_error_log() const;
	private:
		void SortToLine();

		std::vector<Polygon> polygons;
		bool sort_line;
		//bool result;
		std::string error_log;
	};

	double GetPointsDis(const Point& p1, const Point& p2);
}