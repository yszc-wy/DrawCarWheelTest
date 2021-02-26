#include "FilePath.h"
namespace FilePath {
	const char* GetFloatPlantPath(FloatPlant float_plant)
	{
		switch (float_plant)
		{
		case FilePath::NJ840_2L:
			return k_FloatPlant_NJ840_2L_CadPath;
			break;
		case FilePath::NJ840_4L:
			return k_FloatPlant_NJ840_4L_CadPath;
			break;
		case FilePath::SH13LA:
			return k_FloatPlant_SH13LA_CadPath;
			break;
		case FilePath::SH840_4L:
			return k_FloatPlant_SH840_4L_CadPath;
			break;
		case FilePath::SH840_135:
			return k_FloatPlant_SH840_135_CadPath;
			break;
		default:
			return nullptr;
			break;
		}
	}
}