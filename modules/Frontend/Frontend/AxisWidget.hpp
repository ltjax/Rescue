#pragma once
#include <QWidget>
#include <memory>
#include "Domain/Axis.hpp"
#include <functional>

namespace Ui { class Axis; }

class AxisWidget : public QWidget
{
public:
	AxisWidget(std::shared_ptr<Axis> axis, QWidget* parent);
	~AxisWidget();
private:
	void modifyCurve(std::function<Curve(Curve)> modifier);
	template <typename T>
	std::function<void(T)> directTo(Curve(Curve::*f)(T) const);

	std::unique_ptr<Ui::Axis> mUi;
	std::shared_ptr<Axis> mAxis;
};

template<typename T>
inline std::function<void(T)> AxisWidget::directTo(Curve(Curve::*f)(T) const)
{
	return [this,f](T value)
	{
		modifyCurve(std::bind(f, std::placeholders::_1, value));
	};
}
