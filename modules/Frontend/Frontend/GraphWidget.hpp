#pragma once
#include "Domain/Axis.hpp"
#include <QWidget>
#include <memory>
#include <optional.hpp>

class GraphWidget : public QWidget
{
public:
    template <typename T> using Optional = nonstd::optional<T>;
    GraphWidget(QWidget* parent);

    void setCurve(Rescue::Curve curve);
    Rescue::Curve const& getCurve() const;

    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;

    void paintEvent(QPaintEvent* e) override;

private:
    Rescue::Curve mCurve;
    Optional<float> mCurrentX;
};
