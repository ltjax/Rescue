#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QFormLayout>
#include <diffable_list.hpp>
#include <QtWidgets/QLabel>
#include <QtCore/QAbstractTableModel>
#include <QtWidgets/QTableView>
#include "state_observer.hpp"
#include "State.hpp"

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
  ~OutputListWidget();

private:
  void updateFrom(Outputs const& outputs, Group const& group);
  ushiro::state_observer<Rescue::State> mObserver;
  QVBoxLayout* mLayout;
  OutputTreeModel mModel;
};

}




