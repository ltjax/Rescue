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
	void clearActionWidgets();

	void onFileSave();
	void onFileSaveAs();
	void onFileOpen();
	void onFileNew();

private:
	void saveTo(QString filename);
	void setCurrentFilename(QString filename);
	QString getFilePath() const;
	void syncWidgets();
	void catchAll(std::function<void()> rhs);
	std::unique_ptr<Ui::MainWindow> mUi;
	QBoxLayout* mAreaLayout=nullptr;
	std::vector<ActionWidget*> mActionWidgetList;
	QString mCurrentFilename;

	std::shared_ptr<Group> mGroup;
};