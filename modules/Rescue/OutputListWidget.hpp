#pragma once

#include "State.hpp"
#include "state_observer.hpp"
#include <QtCore/QAbstractTableModel>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>
#include <diffable_list.hpp>

namespace Rescue
{

class OutputTreeModel : public QAbstractTableModel
{
public:
  void updateFrom(Outputs const& outputs, Group const& group);

  int rowCount(QModelIndex const& parent) const override;
  int columnCount(QModelIndex const& parent) const override;

  QVariant data(const QModelIndex& index, int role) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
  QStringList mNames;
  std::vector<float> mValues;
};

class OutputListWidget : public QTableView
{
public:
  OutputListWidget(ushiro::link<Rescue::State> link, QWidget* parent);
  ~OutputListWidget() final;

private:
  void updateFrom(Outputs const& outputs, Group const& group);
  ushiro::state_observer<Rescue::State> mObserver;
  QVBoxLayout* mLayout;
  OutputTreeModel mModel;
  std::unique_ptr<QStyledItemDelegate> mFloatItemDelegate;
};

} // namespace Rescue
