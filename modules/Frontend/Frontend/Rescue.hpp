#pragma once
#include <QMainWindow>
#include <memory>
#include <QBoxLayout>
#include "Domain/Group.hpp"
#include <functional>

namespace Ui { class MainWindow; }

class ActionWidget;
class Rescue : public QMainWindow
{
public:
	Rescue();
	~Rescue();

	void onAddAction();

	ActionWidget* addActionWidget(std::shared_ptr<Action> const& action);

	void onFileSave();
	void onFileSaveAs();
	void onFileOpen();
	void onFileNew();

private:
	void catchAll(std::function<void()> rhs);
	std::unique_ptr<Ui::MainWindow> mUi;
	QBoxLayout* mAreaLayout=nullptr;

	std::shared_ptr<Group> mGroup;
};