#pragma once
#include <QMainWindow>

#include "StandardFlangeTreadMsg.h"
#include "DxfStandardFlangeTreadMsgMaker.h"
#include "DxfFloatPlantMsgMaker.h"
#include "FilePath.h"
class QLocalSpreadMainWindow;
class QLocalMainWindow;

class QLabel;
class QComboBox;
class QPushButton;
class QDoubleSpinBox;
class QProgressDialog;

//enum FloatPlant{a,b};
class QTwoOpenglMainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit QTwoOpenglMainWindow(QWidget * parent = Q_NULLPTR);
	~QTwoOpenglMainWindow();
	//void SetWheel(
	//	int wheel_id,         //车轮号
	//	const QVector<QVector2D>& survey_flange,//激光扫描
	//	const QMap<float, QVector<float>>& survey_tread,//一圈一圈
	//	FloatPlant float_plant_type
	//);
	void SetWheel(const QString& wheel_id, const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, FilePath::FloatPlant float_plant);

	void AddWheel(const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, std::string float_plant_path="");
	void SetCurrentWheel(int id);

signals:
	void LocalProcessLog(const QString& info, int present);//主用
	void LocalErrorLog(const QString& info);//主用

	void LocalSpreadProcessLog(const QString& info, int present);//可以不用
protected:
	void showEvent(QShowEvent* e) override;
private slots:
	void AddSFTCad();
	void AddFPCad();
	void ReplaceStandardWheelMsg(const QString& text);
	void ReplaceStandardWheelMsg(int id);
	void SetColorGamut(double color_gamut);
	void FullOrRecoverLocalWindow(bool fs);
	void FullOrRecoverLocalSpreadWindow(bool fs);
	void SwitchFloatPlant(const QString& text);
	void SwitchFloatPlant(int id);
	void AddDefaultFloatPlant();//必须要在显示后调用
	void UpdateProcessDialog(const QString& info,int present);
private:
	void RecoverWidget(const QString& wheel_id);
	void AddFloatPlant(const QString &float_plant_path);
	void AddDefaultStandardWheelMaker();
	DxfReader::StandardFlangeTreadMsgMaker ReadStandardFlangeTreadCadFile(const char* file_path);
	DxfReader::FloatPlantMsgMaker ReadFloatPlantCadFile(const char* file_path);
	void CreateWidget();
	void CreateContext();

	QWidget *central_widget;
	QLocalMainWindow *local_mainwindow;
	QLocalSpreadMainWindow *local_spread_mainwindow;

	QToolBar *standard_flange_tread_tool_bar;
	QToolBar *float_plant_tool_bar;
	QToolBar *color_gamut_tool_bar;
	QLabel *combobox_label;
	QComboBox *contrast_combobox;
	QPushButton *add_sft_button;
	std::map<std::string,DxfReader::StandardFlangeTreadMsgMaker> standard_wheel_makers;

	std::map<QString, int> contrast_combobox_records;
	std::map<QString, int> float_plant_combobox_records;

	QLabel *float_plant_label;
	QComboBox *float_plant_combobox;
	QPushButton *add_fp_button;

	QLabel *color_gamut_label;
	QDoubleSpinBox* color_gamut_spin_box;

	//QPushButton *test_button;

	QString default_fp_path;
	QString current_wheel_id;

	QProgressDialog* process_dialog;
	//QOpenGLContext *local_context;
	//QOpenGLContext *local_spread_context;
};
