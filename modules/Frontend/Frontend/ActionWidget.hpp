#pragma once
#include "Domain/Action.hpp"
#include <QBoxLayout>
#include <QWidget>
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
    ActionWidget(std::shared_ptr<Action> action, QWidget* parent);
    ~ActionWidget();

    void onAddAxis();
    AxisWidget* addAxisWidget(std::shared_ptr<Axis> const& axis);

private:
    std::unique_ptr<Ui::Action> mUi;
    std::shared_ptr<Action> mAction;
    FlowLayout* mAreaLayout = nullptr;
};
