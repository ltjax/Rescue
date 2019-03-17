#pragma once
#include "Action.hpp"
#include "State.hpp"
#include "event_bus.hpp"
#include <QtWidgets/QWidget>
#include <functional>
#include <memory>
#include <state_observer.hpp>

namespace Ui
{
class Axis;
}

namespace Rescue
{

class AxisWidget : public QWidget
{
public:
  AxisWidget(
    Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer, Id actionId, Id axisId, QWidget* parent);
  ~AxisWidget() final;

private:
  void updateFrom(Ptr<Rescue::Axis const> const& axis);
  void updateInputSelect(Inputs const& inputs, Id current);
  void modifyCurve(std::function<Rescue::Curve(Rescue::Curve)> modifier);
  void emitChange(RangedCurve const& change);
  template <typename T> std::function<void(T)> directTo(Rescue::Curve (Rescue::Curve::*f)(T) const);

  Rescue::RangedCurve const& currentCurve() const;

  std::unique_ptr<Ui::Axis> mUi;
  Ptr<ushiro::event_bus> mBus;
  Id mActionId;
  Id mAxisId;
};

template <typename T> inline std::function<void(T)> AxisWidget::directTo(Rescue::Curve (Rescue::Curve::*f)(T) const)
{
  return [this, f](T value) { modifyCurve(std::bind(f, std::placeholders::_1, value)); };
}

} // namespace Rescue
