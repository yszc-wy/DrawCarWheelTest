#pragma once
namespace FilePath
{
	////shader path
	//const char kFTrainShaderPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Shader\\ftrainshader.glsl";
	//const char kVTrainShaderPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Shader\\vtrainshader.glsl";
	//const char kFWheelFlangeTreadShaderPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Shader\\fwheelflangetreadshader.glsl";
	//const char kVWheelFlangeTreadShaderPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Shader\\vwheelflangetreadshader.glsl";
	//const char kFWheelOtherShaderPath[]= "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Shader\\fwheelothershader.glsl";
	//const char kVWheelOtherShaderPath[]="W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Shader\\vwheelothershader.glsl";
	//const char kFWheelSpreadShaderPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Shader\\fwheelspreadshader.glsl";
	//const char kVWheelSpreadShaderPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Shader\\vwheelspreadshader.glsl";
	//const char kFLineShaderPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Shader\\f_line_shader.glsl";
	//const char kVLineShaderPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Shader\\v_line_shader.glsl";
	//const char kVFontShaderPath[]= "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Shader\\v_font_shader.glsl";
	//const char kFFontShaderPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Shader\\f_font_shader.glsl";
	////model path
	//const char kTrainFramePath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Resources\\Model\\TrainFrame.obj";
	//const char kTrainWheelPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Resources\\Model\\TrainWheel.obj";
	//const char kTrainBoxPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Resources\\Model\\TrainBox.obj";
	//const char kArrowPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Resources\\Model\\Arrow.obj";
	//const char kLinePath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Resources\\Model\\Line.obj";
	////wheel msg path
	//const char kStandardOutsideTreadPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Resources\\CadMsg\\SH4L-SY-TD.txt";
	//const char kFloatPlateDirectory[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Resources\\CadMsg\\";
	////test msg path
	//const char kSurveyTreadPath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Resources\\SurveyMsg\\Tread.txt";
	//const char kSurveyFlangePath[] = "W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Resources\\SurveyMsg\\Flange.txt";
	////font texture path
	//const char kFontTexturePath[] = "W:\\CarWheel\\DrawCarWheelTest\D\rawCarWheelTest\\Resources\\Texture\\ExportedFont32.tga";

	enum FloatPlant { NJ840_2L, NJ840_4L, SH13LA, SH840_4L, SH840_135 };

	//shader path
	const char kFTrainShaderPath[] = ".\\Shader\\ftrainshader.glsl";
	const char kVTrainShaderPath[] = ".\\Shader\\vtrainshader.glsl";
	const char kFWheelFlangeTreadShaderPath[] = ".\\Shader\\fwheelflangetreadshader.glsl";
	const char kVWheelFlangeTreadShaderPath[] = ".\\Shader\\vwheelflangetreadshader.glsl";
	const char kFWheelOtherShaderPath[]= ".\\Shader\\fwheelothershader.glsl";
	const char kVWheelOtherShaderPath[]=".\\Shader\\vwheelothershader.glsl";
	const char kFWheelSpreadShaderPath[] = ".\\Shader\\fwheelspreadshader.glsl";
	const char kVWheelSpreadShaderPath[] = ".\\Shader\\vwheelspreadshader.glsl";
	const char kFLineShaderPath[] = ".\\Shader\\f_line_shader.glsl";
	const char kVLineShaderPath[] = ".\\Shader\\v_line_shader.glsl";
	const char kVFontShaderPath[]= ".\\Shader\\v_font_shader.glsl";
	const char kFFontShaderPath[] = ".\\Shader\\f_font_shader.glsl";
	const char kV2dShaderPath[] = ".\\Shader\\v_2d_shader.glsl";
	const char kF2dShaderPath[] = ".\\Shader\\f_2d_shader.glsl";

	//model path
	const char kTrainFramePath[] = ".\\Resources\\Model\\TrainFrame.obj";
	const char kSingleTrainFramePath[] = ".\\Resources\\Model\\single_train_frame.obj";
	const char kTrainWheelPath[] = ".\\Resources\\Model\\TrainWheel.obj";
	const char kTrainBoxPath[] = ".\\Resources\\Model\\TrainBox.obj";
	const char kSingleTrainBoxPath[] = ".\\Resources\\Model\\single_train_box.obj";
	const char kSingleTrainHeadPath[] = ".\\Resources\\Model\\single_train_head.obj";
	const char kArrowPath[] = ".\\Resources\\Model\\Arrow.obj";
	const char kLinePath[] = ".\\Resources\\Model\\Line.obj";
	//cad path
	const char k_FloatPlant_NJ840_2L_CadPath[] = ".\\Resources\\CadMsg\\NJ840-2L_float.dxf";
	const char k_FloatPlant_NJ840_4L_CadPath[] = ".\\Resources\\CadMsg\\NJ840-4L_float.dxf";
	const char k_FloatPlant_SH13LA_CadPath[] = ".\\Resources\\CadMsg\\SH13LA_float.dxf";
	const char k_FloatPlant_SH840_4L_CadPath[] = ".\\Resources\\CadMsg\\SH840-4L_float.dxf";
	const char k_FloatPlant_SH840_135_CadPath[] = ".\\Resources\\CadMsg\\SH840-135_float.dxf";

	//wheel msg path
	const char kStandardOutsideTreadPath[] = ".\\Resources\\CadMsg\\SH4L-SY-TD.txt";
	const char kFloatPlateDirectory[] = ".\\Resources\\CadMsg\\";
	const char kExampleStandardTreadCadPath[] = ".\\Resources\\standard_msg\\SH4L-SY_flange_tread.dxf";
	const char kExampleStandardTreadTextPath[] = ".\\Resources\\standard_msg\\SH4L-SY.txt";
	//test msg path
	const char kSurveyTreadPath[] = ".\\Resources\\SurveyMsg\\Tread.txt";
	const char kSurveyFlangePath[] = ".\\Resources\\SurveyMsg\\Flange.txt";
	//font texture path
	const char kFontTexturePath[] = ".\\Resources\\Texture\\ExportedFont32.tga";

	const char* GetFloatPlantPath(FloatPlant float_plant);

}