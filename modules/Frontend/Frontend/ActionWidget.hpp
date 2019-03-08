#pragma once
#include "Domain/Action.hpp"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QWidget>
#include <memory>

namespace Ui
{
class Action;
}

class AxisWidget;
class FlowLayout;

class ActionWidget : public QWidget
{
public:
    ActionWidget(std::shared_ptr<Rescue::Action> action, QWidget* parent);
    ~ActionWidget();

    void onAddAxis();
    AxisWidget* addAxisWidget(std::shared_ptr<Rescue::Axis> const& axis);

private:
    std::unique_ptr<Ui::Action> mUi;
    std::shared_ptr<Rescue::Action> mAction;
    FlowLayout* mAreaLayout = nullptr;
};
