#pragma once
#include "Action.hpp"
#include <QtWidgets/QWidget>
#include <memory>
#include <optional.hpp>

class GraphWidget : public QWidget
{
public:
  template <typename T> using Optional = nonstd::optional<T>;
  explicit GraphWidget(QWidget* parent);

  void setRangedCurve(Rescue::RangedCurve curve);
  Rescue::RangedCurve const& getRangedCurve() const;

  void setSimulatedInput(Optional<float> input);

  void mouseMoveEvent(QMouseEvent* event) override;
  void leaveEvent(QEvent* event) override;

  void paintEvent(QPaintEvent* e) override;

private:
  Rescue::RangedCurve mCurve;
  Optional<float> mMouseInput;
  Optional<float> mSimulatedInput;
};
