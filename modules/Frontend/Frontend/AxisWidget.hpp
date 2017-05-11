#pragma once
#include <QWidget>
#include <memory>

namespace Ui { class Axis; }

class AxisWidget : public QWidget
{
public:
	AxisWidget(QWidget* parent);
	~AxisWidget();
private:
	std::unique_ptr<Ui::Axis> mUi;
};
