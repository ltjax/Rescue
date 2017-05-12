#pragma once
#include <QMainWindow>
#include <memory>
#include <QBoxLayout>

namespace Ui { class MainWindow; }

class Rescue : public QMainWindow
{
public:
	Rescue();
	~Rescue();

	void onAddAction();

	void onFileSave();
	void onFileSaveAs();
	void onFileOpen();
	void onFileNew();

private:
	std::unique_ptr<Ui::MainWindow> mUi;
	QBoxLayout* mAreaLayout=nullptr;
};