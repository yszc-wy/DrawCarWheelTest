#include "qtwoopenglmainwindow.hpp"

#include <iostream>
#include <string>

#include <qlayout.h>
#include <qsplitter.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qtoolbar.h>
#include <qmessagebox.h>
#include <qspinbox.h>
#include <qprogressdialog.h>


#include "qlocalmainwindow.h"
#include "qlocalspreadmainwindow.hpp"
#include "FilePath.h"
#include "DxfReader.h"
#include "CadLib\dl_dxf.h"
#include "CadLib\dl_creationadapter.h"

QTwoOpenglMainWindow::QTwoOpenglMainWindow(QWidget * parent) : QMainWindow(parent)
, process_dialog(nullptr)
{
	this->CreateWidget();
	this->showMaximized();
}

QTwoOpenglMainWindow::~QTwoOpenglMainWindow() {
	
}
//void QTwoOpenglMainWindow::CreateContext()
//{
//	this->local_context = new QOpenGLContext;
//	this->local_context->create();
//
//	this->local_spread_context = new QOpenGLContext;
//	this->local_spread_context->create();
//}
void QTwoOpenglMainWindow::CreateWidget()
{
	this->central_widget = new QWidget(this);
	this->local_mainwindow = new QLocalMainWindow(this->central_widget);
	this->local_spread_mainwindow = new QLocalSpreadMainWindow(this->central_widget);
	connect(this->local_spread_mainwindow, SIGNAL(PickSectionRadian(float)), this->local_mainwindow, SLOT(SetPickRadian(float)));//展开条拾取轮子响应
	connect(this->local_mainwindow, SIGNAL(FullScreen(bool)), this, SLOT(FullOrRecoverLocalWindow(bool)));
	connect(this->local_spread_mainwindow, SIGNAL(FullScreen(bool)), this, SLOT(FullOrRecoverLocalSpreadWindow(bool)));
	connect(this->local_mainwindow, SIGNAL(InitGLOver()), this, SLOT(AddDefaultFloatPlant()));

	connect(this->local_mainwindow, &QLocalMainWindow::ProcessLog, this, &QTwoOpenglMainWindow::LocalProcessLog);
	connect(this->local_mainwindow, &QLocalMainWindow::ErrorLog, this, &QTwoOpenglMainWindow::LocalErrorLog);
	connect(this->local_spread_mainwindow, &QLocalSpreadMainWindow::ProcessLog, this, &QTwoOpenglMainWindow::LocalSpreadProcessLog);

	QVBoxLayout *main_layout = new QVBoxLayout();
	main_layout->addWidget(this->local_mainwindow);
	main_layout->addWidget(this->local_spread_mainwindow);

	this->central_widget->setLayout(main_layout);
	
	//QSplitter *mainSplitter = new QSplitter(Qt::Vertical, 0);
	//this->local_mainwindow->setParent(mainSplitter);
	//this->local_spread_mainwindow->setParent(mainSplitter);
	//mainSplitter->setStretchFactor(0, 1);
	this->setCentralWidget(central_widget);

	//create sft import
	this->combobox_label = new QLabel("Contrast flange type:", this);

	this->contrast_combobox = new QComboBox(this);
	this->contrast_combobox->setEditable(false);
	this->contrast_combobox->setFixedWidth(500);
	this->contrast_combobox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	this->contrast_combobox->addItem("Height Contrast");
	this->AddDefaultStandardWheelMaker();

	this->add_sft_button = new QPushButton("Add standard wheel msg");

	connect(this->contrast_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(ReplaceStandardWheelMsg(int)));
	connect(this->add_sft_button, SIGNAL(clicked()), this, SLOT(AddSFTCad()));
	
	this->color_gamut_label= new QLabel("Color Gamut:",this);
	this->color_gamut_spin_box=new QDoubleSpinBox(this);
	this->color_gamut_spin_box->setRange(0.0001,1000);
	this->color_gamut_spin_box->setValue(0.75);
	this->color_gamut_spin_box->setSuffix(" mm");
	this->color_gamut_spin_box->setWrapping(true);
	this->color_gamut_spin_box->setEnabled(false);
	this->color_gamut_spin_box->setSingleStep(0.01);
	connect(this->color_gamut_spin_box, SIGNAL(valueChanged(double)), this, SLOT(SetColorGamut(double)));

	this->float_plant_label = new QLabel("Float Plant Choose:", this);

	this->float_plant_combobox = new QComboBox(this);
	this->float_plant_combobox->setFixedWidth(500);
	this->float_plant_combobox->setEditable(false);
	this->float_plant_combobox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	this->float_plant_combobox->addItem("Default float plant");
	//this->AddDefaultFloatPlant();

	this->add_fp_button= new QPushButton("Add float plant msg");
	connect(this->float_plant_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(SwitchFloatPlant(int)));
	connect(this->add_fp_button, SIGNAL(clicked()), this, SLOT(AddFPCad()));

	//this->test_button= new QPushButton("Test");
	//connect(this->test_button, SIGNAL(clicked()), this->local_spread_mainwindow, SLOT(AddTestMsg()));

	this->standard_flange_tread_tool_bar = this->addToolBar("Standard Flange Tread");
	this->standard_flange_tread_tool_bar->setFloatable(true);
	this->standard_flange_tread_tool_bar->setMovable(true);
	this->standard_flange_tread_tool_bar->addWidget(this->combobox_label);
	this->standard_flange_tread_tool_bar->addWidget(this->contrast_combobox);
	this->standard_flange_tread_tool_bar->addWidget(this->add_sft_button);

	this->float_plant_tool_bar = this->addToolBar("Float Plant Tool Bar");
	this->float_plant_tool_bar->setFloatable(true);
	this->float_plant_tool_bar->setMovable(true);
	this->float_plant_tool_bar->addWidget(this->float_plant_label);
	this->float_plant_tool_bar->addWidget(this->float_plant_combobox);
	this->float_plant_tool_bar->addWidget(this->add_fp_button);

	this->color_gamut_tool_bar = this->addToolBar("Color Gamut Tool Bar");
	this->color_gamut_tool_bar->setFloatable(true);
	this->color_gamut_tool_bar->setMovable(true);
	this->color_gamut_tool_bar->addWidget(this->color_gamut_label);
	this->color_gamut_tool_bar->addWidget(this->color_gamut_spin_box);
	//this->color_gamut_tool_bar->addWidget(this->test_button);
}
void QTwoOpenglMainWindow::AddDefaultFloatPlant()
{
	//std::cout << "QTwoOpenglMainWindow::AddDefaultFloatPlant::initOver" << std::endl;
	//disconnect(this->float_plant_combobox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(SwitchFloatPlant(const QString&)));

	//第一个一定要是默认的cadpath
	//this->AddFloatPlant(FilePath::k_FloatPlant_SH840_4L_CadPath);
	//this->AddFloatPlant(FilePath::k_FloatPlant_NJ840_2L_CadPath);
	//this->AddFloatPlant(FilePath::k_FloatPlant_NJ840_4L_CadPath);
	//this->AddFloatPlant(FilePath::k_FloatPlant_SH13LA_CadPath);
	//this->AddFloatPlant(FilePath::k_FloatPlant_SH840_135_CadPath);

	//connect(this->float_plant_combobox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(SwitchFloatPlant(const QString&)));
}
void QTwoOpenglMainWindow::AddFloatPlant(const QString &float_plant_path)
{
	QDir temDir(float_plant_path);
	QString absDir = temDir.absolutePath();

	auto msg_maker = this->ReadFloatPlantCadFile(absDir.toStdString().c_str());
	//std::cout << "QTwoOpenglMainWindow::AddFloatPlant::OK?" << std::endl;
	this->local_mainwindow->UpdateFloatPlant(absDir.toStdString(), msg_maker);
	//std::cout << "QTwoOpenglMainWindow::AddFloatPlant::OK!" << std::endl;
	if (this->float_plant_combobox->findText(float_plant_path) == -1)
		this->float_plant_combobox->addItem(absDir);
}
void QTwoOpenglMainWindow::AddFPCad()
{
	QStringList name_list = QFileDialog::getOpenFileNames(this, "import standard wheel cad", "/", "Cad Files(*.dxf)");

	for (auto &str : name_list)
	{
		auto msg_maker = this->ReadFloatPlantCadFile(str.toStdString().c_str());

		this->local_mainwindow->UpdateFloatPlant(str.toStdString(), msg_maker);

		if (this->float_plant_combobox->findText(str) == -1)
			this->float_plant_combobox->addItem(str/*+QString(" Contrast")*/);
	}
}

void QTwoOpenglMainWindow::SwitchFloatPlant(const QString& text)
{
	//std::cout << "?" << std::endl
	QString real_text;
	if (text == "Default float plant")
	{
		real_text = this->default_fp_path;
	}
	else
	{
		real_text = text;
	}
	this->local_mainwindow->SwitchFloatPlant(real_text.toStdString());
}
void QTwoOpenglMainWindow::SwitchFloatPlant(int id)
{
	QString real_text;
	if (id == 0)
	{
		real_text = this->default_fp_path;
	}
	else
	{
		real_text = this->float_plant_combobox->itemText(id);
	}
	this->local_mainwindow->SwitchFloatPlant(real_text.toStdString());

	if (this->current_wheel_id == ""|| this->float_plant_combobox_records.find(this->current_wheel_id)== this->float_plant_combobox_records.end())
	{
		std::cout << "Error::QTwoOpenglMainWindow::ReplaceStandardWheelMsg::illegal current wheel id" << std::endl;
	}
	else
		this->float_plant_combobox_records[this->current_wheel_id] = id;
}
void QTwoOpenglMainWindow::SetColorGamut(double color_gamut)
{
	if (color_gamut <= 0)
	{
		std::cout << "Error::QTwoOpenglMainWindow::SetColorGamut:: illegal color_gamut" << std::endl;
		return;
	}
	
	if(this->contrast_combobox->currentText()=="Height Contrast")
	{
		std::cout << "Error::QTwoOpenglMainWindow::SetColorGamut:: can not set gamut in height model" << std::endl;
		return;
	}
	this->local_mainwindow->SetColorGamut(color_gamut);
	this->local_spread_mainwindow->SetColorGamut(color_gamut);
}
void QTwoOpenglMainWindow::AddDefaultStandardWheelMaker()
{//相对路径和绝对路径？
	QDir temDir(FilePath::kExampleStandardTreadCadPath);
	QString absDir = temDir.absolutePath();

	//Wheel::StandardFlangeTreadMsgMake msg_make(absDir.toStdString());
	auto msg_maker = this->ReadStandardFlangeTreadCadFile(absDir.toStdString().c_str());
	//if (!msg_make.get_result())
	//{
	//	std::string error_log;
	//	error_log += "Error File:: " + absDir.toStdString() + ":\n";
	//	error_log += msg_make.get_info_log();
	//	error_log += "\n";

	//	QMessageBox::warning(NULL, "Import text error log", QString(error_log.c_str()), QMessageBox::Ok);
	//	return;
	//}
	//std::cout << "Ok?" << std::endl;
	this->contrast_combobox->addItem(absDir);
	this->standard_wheel_makers.insert({ absDir.toStdString(),msg_maker });
}

void QTwoOpenglMainWindow::ReplaceStandardWheelMsg(const QString& text)
{
	//std::cout << "QTwoOpenglMainWindow::ReplaceStandardWheelMsg::OK?" << std::endl;
	if (text == "Height Contrast")
	{
		//std::cout << "QTwoOpenglMainWindow::ReplaceStandardWheelMsg::OK!!" << std::endl;
		/*Wheel::StandardFlangeTreadMsgMake*/DxfReader::StandardFlangeTreadMsgMaker msg_maker;
		this->local_mainwindow->ReplaceStandardWheelMsg(msg_maker);
		this->local_spread_mainwindow->ReplaceStandardWheelMsg(msg_maker);
		
		//关闭模式
		this->color_gamut_spin_box->setEnabled(false);
	}
	else
	{
		//std::cout << "QTwoOpenglMainWindow::ReplaceStandardWheelMsg::OK!!!!" << std::endl;
		if (this->standard_wheel_makers.find(text.toStdString()) == this->standard_wheel_makers.end())
		{
			std::cout << "Error::QTwoOpenglMainWindow::ReplaceStandardWheelMsg::illegal selected item, can not find text in msgmakes list" << std::endl;
			std::cout << "Text is:" << text.toStdString() << std::endl;
			return;
		}
		else
		{
			//std::cout << "QTwoOpenglMainWindow::ReplaceStandardWheelMsg::OK?" << std::endl;
			this->local_mainwindow->ReplaceStandardWheelMsg(this->standard_wheel_makers[text.toStdString()]);
			//std::cout << "QTwoOpenglMainWindow::ReplaceStandardWheelMsg::OK?" << std::endl;
			this->local_spread_mainwindow->ReplaceStandardWheelMsg(this->standard_wheel_makers[text.toStdString()]);
			//std::cout << "QTwoOpenglMainWindow::ReplaceStandardWheelMsg::OK!" << std::endl;
			this->color_gamut_spin_box->setEnabled(true);
		}
	}
}

void QTwoOpenglMainWindow::ReplaceStandardWheelMsg(int id)
{
	if (id==0)
	{
		//std::cout << "QTwoOpenglMainWindow::ReplaceStandardWheelMsg::OK!!" << std::endl;
		/*Wheel::StandardFlangeTreadMsgMake*/DxfReader::StandardFlangeTreadMsgMaker msg_maker;
		this->local_mainwindow->ReplaceStandardWheelMsg(msg_maker);
		this->local_spread_mainwindow->ReplaceStandardWheelMsg(msg_maker);

		//关闭模式
		this->color_gamut_spin_box->setEnabled(false);
	}
	else
	{
		QString text = this->contrast_combobox->itemText(id);
		//std::cout << "QTwoOpenglMainWindow::ReplaceStandardWheelMsg::OK!!!!" << std::endl;
		if (this->standard_wheel_makers.find(text.toStdString()) == this->standard_wheel_makers.end())
		{
			std::cout << "Error::QTwoOpenglMainWindow::ReplaceStandardWheelMsg::illegal selected item, can not find text in msgmakes list" << std::endl;
			std::cout << "Text is:" << text.toStdString() << std::endl;
			return;
		}
		else
		{
			//std::cout << "QTwoOpenglMainWindow::ReplaceStandardWheelMsg::OK?" << std::endl;
			this->local_mainwindow->ReplaceStandardWheelMsg(this->standard_wheel_makers[text.toStdString()]);
			//std::cout << "QTwoOpenglMainWindow::ReplaceStandardWheelMsg::OK?" << std::endl;
			this->local_spread_mainwindow->ReplaceStandardWheelMsg(this->standard_wheel_makers[text.toStdString()]);
			//std::cout << "QTwoOpenglMainWindow::ReplaceStandardWheelMsg::OK!" << std::endl;
			this->color_gamut_spin_box->setEnabled(true);
		}
	}

	if (this->current_wheel_id == "" || this->contrast_combobox_records.find(this->current_wheel_id) == this->contrast_combobox_records.end())
	{
		std::cout << "Error::QTwoOpenglMainWindow::ReplaceStandardWheelMsg::illegal current wheel id" << std::endl;
	}
	else
		this->contrast_combobox_records[this->current_wheel_id] = id;
}

void QTwoOpenglMainWindow::AddSFTCad()
{
	/*if (this->file_dialog == NULL)
	{
	this->file_dialog = new QFileDialog(this);
	this->file_dialog->setWindowTitle("导入标准轮txt");
	this->file_dialog->setAcceptMode(QFileDialog::AcceptOpen);
	this->file_dialog->setFilter(tr("Text Files(*.txt)"));
	}*/
	QStringList name_list = QFileDialog::getOpenFileNames(this, "import standard wheel cad", "/", "Cad Files(*.dxf)");
	
	//bool result = true;
	//std::string error_log;
	for (auto &str : name_list)
	{
		//std::cout << str.toStdString() << std::endl;
		//Wheel::StandardFlangeTreadMsgMake msg_maker(str.toStdString());
		//if (!msg_maker.get_result())
		//{
		//	error_log += "Error File:: " + str.toStdString() + ":\n";
		//	error_log += msg_maker.get_info_log();
		//	error_log += "\n";
		//	result = false;
		//	continue;
		//}

		auto msg_maker = this->ReadStandardFlangeTreadCadFile(str.toStdString().c_str());

		this->standard_wheel_makers.insert({ str.toStdString(),msg_maker });
	}
	
	//if (!result)//弹出修改对话框
	//{
	//	QMessageBox::warning(NULL, "Import text error log", QString(error_log.c_str()), QMessageBox::Ok);
	//}
	//刷新combobox（不能清空）

	//this->contrast_combobox->clear();
	//this->contrast_combobox->addItem("Height Contrast");
	for (auto i : this->standard_wheel_makers)
	{
		if(this->contrast_combobox->findText(i.first.c_str())==-1)
			this->contrast_combobox->addItem(i.first.c_str()/*+QString(" Contrast")*/);
	}
}

void QTwoOpenglMainWindow::FullOrRecoverLocalWindow(bool fs)
{
	if (fs == true&&(!this->local_mainwindow->isHidden())&&(!this->local_spread_mainwindow->isHidden()))//放大本窗口就让其他窗口全部隐藏
	{
		this->local_spread_mainwindow->hide();
		//添加其他窗口要隐藏的窗口
	}
	if (fs == false && (!this->local_mainwindow->isHidden()) && this->local_spread_mainwindow->isHidden())//缩小本窗口就让其他窗口全部隐藏
	{
		this->local_spread_mainwindow->show();
		//添加其他窗口要显示的窗口
	}
}
void QTwoOpenglMainWindow::FullOrRecoverLocalSpreadWindow(bool fs)
{
	if (fs == true && (!this->local_mainwindow->isHidden()) && (!this->local_spread_mainwindow->isHidden()))//放大本窗口就让其他窗口全部隐藏
	{
		this->local_mainwindow->hide();
		//添加其他窗口要隐藏的窗口
	}
	if (fs == false && (this->local_mainwindow->isHidden()) && (!this->local_spread_mainwindow->isHidden()))//缩小本窗口就让其他窗口全部隐藏
	{
		this->local_mainwindow->show();
		//添加其他窗口要显示的窗口
	}
}

void QTwoOpenglMainWindow::AddWheel(const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, std::string type)
{
	Wheel::WheelFlangeTread wheel_flange_tread(surveyFlange, surveyTread/*, filePath*/);
	this->local_mainwindow->AddWheel(wheel_flange_tread,type);
	this->local_spread_mainwindow->AddWheel(wheel_flange_tread);
}

void QTwoOpenglMainWindow::SetCurrentWheel(int id)
{
	this->local_mainwindow->SetCurrentWheel(id);
	this->local_spread_mainwindow->SetCurrentWheel(id);
}

DxfReader::StandardFlangeTreadMsgMaker QTwoOpenglMainWindow::ReadStandardFlangeTreadCadFile(const char* file_path)
{
	DxfReader::DxfReader* reader = new DxfReader::DxfReader();
	DL_Dxf* dxf = new DL_Dxf();

	std::string error_log=std::string("There are some problem when read this dxf file:\n")+ file_path+" :\n";

	if (!dxf->in(file_path, reader)) { // if file open failed
		std::cerr << file_path << " could not be opened.\n";
		error_log += std::string("Error::This file could not be opened.\n");

		QMessageBox::warning(NULL, "Read Standard Flange Tread Error Log", QString(error_log.c_str()), QMessageBox::Ok);

		return DxfReader::StandardFlangeTreadMsgMaker();
	}
	auto msg_maker = reader->LoadSFTMsgMaker();

	if (reader->get_error_log() !="")//弹出修改对话框
	{
		QMessageBox::warning(NULL, "Read Standard Flange Tread Error Log", QString((error_log+reader->get_error_log()).c_str()), QMessageBox::Ok);
	}

	delete dxf;
	delete reader;

	return msg_maker;
}
void QTwoOpenglMainWindow::UpdateProcessDialog(const QString& info, int present)
{
	if (present == 100)
	{
		this->process_dialog->setValue(present);
		this->process_dialog->setLabelText(info);
		this->process_dialog->close();
	}
	else
	{
		this->process_dialog->setValue(present);
		this->process_dialog->setLabelText(info);
	}
}
void QTwoOpenglMainWindow::SetWheel(const QString& wheel_id, const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, FilePath::FloatPlant float_plant)
{
	//创建进度条
	if (this->process_dialog == nullptr)
	{
		this->process_dialog = new QProgressDialog(this);
		this->process_dialog->setRange(0, 100);
		this->process_dialog->setCancelButton(0);
		connect(this, &QTwoOpenglMainWindow::LocalProcessLog, this, &QTwoOpenglMainWindow::UpdateProcessDialog);//connect不要加常量！！！
	}
	this->process_dialog->setValue(0);
	this->process_dialog->setLabelText("please wait!");
	this->process_dialog->setModal(true);
	this->process_dialog->setLabelText("please wait!");
	this->process_dialog->show();
	this->process_dialog->activateWindow();//激活状态

	//添加或更新记录
	if (this->contrast_combobox_records.find(wheel_id) == this->contrast_combobox_records.end())
	{
		this->contrast_combobox_records.insert({ wheel_id,0 });
	}
	if (this->float_plant_combobox_records.find(wheel_id) == this->float_plant_combobox_records.end())
	{
		this->float_plant_combobox_records.insert({ wheel_id,0 });
	}

	//更新默认fp_path和combobox的default text
	QDir temDir(FilePath::GetFloatPlantPath(float_plant));
	QString absDir = temDir.absolutePath();
	this->default_fp_path = absDir;
	QFileInfo file_info(absDir);
	this->float_plant_combobox->setItemText(0, "Default float plant - " + file_info.fileName());

	this->current_wheel_id = wheel_id;

	this->RecoverWidget(wheel_id);

	this->local_mainwindow->SetWheel(wheel_id, surveyFlange, surveyTread, float_plant);
	this->local_spread_mainwindow->SetWheel(wheel_id, surveyFlange, surveyTread);
}
DxfReader::FloatPlantMsgMaker QTwoOpenglMainWindow::ReadFloatPlantCadFile(const char* file_path)
{
	DxfReader::DxfReader* reader = new DxfReader::DxfReader();
	DL_Dxf* dxf = new DL_Dxf();

	std::string error_log = std::string("There are some problem when read this dxf file:\n") + file_path + " :\n";
	if (!dxf->in(file_path, reader)) { // if file open failed
		std::cerr << file_path << " could not be opened.\n";
		error_log += std::string("Error::This file could not be opened.\n");

		QMessageBox::warning(NULL, "Read Float Plant Error Log", QString(error_log.c_str()), QMessageBox::Ok);
		return DxfReader::FloatPlantMsgMaker();
	}
	auto msg_maker = reader->LoadFPMsgMaker();

	if (reader->get_error_log() != "")//弹出修改对话框
	{
		QMessageBox::warning(NULL, "Read Float Plant Error Log", QString((error_log + reader->get_error_log()).c_str()), QMessageBox::Ok);
	}

	delete dxf;
	delete reader;

	return msg_maker;
}

void QTwoOpenglMainWindow::showEvent(QShowEvent* e) {
	QWidget::showEvent(e);
	//your code here
	//this->AddDefaultFloatPlant();
}

void QTwoOpenglMainWindow::RecoverWidget(const QString& wheel_id)
{
	disconnect(this->contrast_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(ReplaceStandardWheelMsg(int)));
	disconnect(this->float_plant_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(SwitchFloatPlant(int)));

	if(this->float_plant_combobox_records.find(wheel_id)!= this->float_plant_combobox_records.end())
		this->float_plant_combobox->setCurrentIndex(this->float_plant_combobox_records[wheel_id]);
	else
	{
		std::cout << "Error::QTwoOpenglMainWindow::RecoverWidget::illegal wheel id in float_plant_combobox_records" << std::endl;
	}

	if(this->contrast_combobox_records.find(wheel_id)!=this->contrast_combobox_records.end())
		this->contrast_combobox->setCurrentIndex(0);
	else
	{
		std::cout << "Error::QTwoOpenglMainWindow::RecoverWidget::illegal wheel id in contrast_combobox_records" << std::endl;
	}

	connect(this->contrast_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(ReplaceStandardWheelMsg(int)));
	connect(this->float_plant_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(SwitchFloatPlant(int)));
}