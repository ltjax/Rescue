#pragma once
#include <QWidget>
#include <memory>
#include <QBoxLayout>

namespace Ui { class Action; }

class ActionWidget : public QWidget
{
public:
	ActionWidget(QWidget* parent);
	~ActionWidget();

	void OnAddAxis();
private:
	std::unique_ptr<Ui::Action> mUi;
	QBoxLayout* mAreaLayout=nullptr;
};
