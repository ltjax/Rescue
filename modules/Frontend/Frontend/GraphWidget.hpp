#pragma once
#include <QWidget>
#include <memory>
#include "Domain/Axis.hpp"
#include <optional.hpp>

class GraphWidget : public QWidget
{
public:
    template <typename T>
    using Optional = nonstd::optional<T>;
	GraphWidget(QWidget* parent);

	void setCurve(Curve curve);
	Curve const& getCurve() const;

    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

	void paintEvent(QPaintEvent* e) override;
private:
	Curve mCurve;
    Optional<float> mCurrentX;
};
