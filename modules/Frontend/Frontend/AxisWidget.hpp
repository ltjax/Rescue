#pragma once
#include "Domain/Axis.hpp"
#include <QtWidgets/QWidget>
#include <functional>
#include <memory>

namespace Ui
{
class Axis;
}

class AxisWidget : public QWidget
{
public:
    AxisWidget(std::shared_ptr<Rescue::Axis> axis, QWidget* parent);
    ~AxisWidget();

private:
    void modifyCurve(std::function<Rescue::Curve(Rescue::Curve)> modifier);
    template <typename T> std::function<void(T)> directTo(Rescue::Curve (Rescue::Curve::*f)(T) const);

    std::unique_ptr<Ui::Axis> mUi;
    std::shared_ptr<Rescue::Axis> mAxis;
};

template <typename T> inline std::function<void(T)> AxisWidget::directTo(Rescue::Curve (Rescue::Curve::*f)(T) const)
{
    return [this, f](T value) { modifyCurve(std::bind(f, std::placeholders::_1, value)); };
}
