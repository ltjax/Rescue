#pragma once
#include <QWidget>
#include <memory>
#include <QBoxLayout>
#include "Domain/Action.hpp"

namespace Ui { class Action; }

class AxisWidget;
class ActionWidget : public QWidget
{
public:
	ActionWidget(std::shared_ptr<Action> action, QWidget* parent);
	~ActionWidget();

	void onAddAxis();
	AxisWidget* addAxisWidget(std::shared_ptr<Axis> const& axis);
private:
	std::unique_ptr<Ui::Action> mUi;
	std::shared_ptr<Action> mAction;
	QBoxLayout* mAreaLayout=nullptr;
};
