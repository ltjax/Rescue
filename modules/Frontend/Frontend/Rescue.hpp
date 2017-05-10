#pragma once
#include <QMainWindow>
#include <memory>

namespace Ui { class MainWindow; }

class Rescue : public QMainWindow
{
public:
	Rescue();
	~Rescue();

	void OnAddAction();

private:
	std::unique_ptr<Ui::MainWindow> mUi;
};