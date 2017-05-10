#pragma once
#include <QWidget>
#include <memory>

namespace Ui { class Action; }

class ActionWidget : public QWidget
{
public:
	ActionWidget(QWidget* parent);
	~ActionWidget();
private:
	std::unique_ptr<Ui::Action> mUi;
};
