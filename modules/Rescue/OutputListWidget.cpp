#include "OutputListWidget.hpp"

using namespace Rescue;

OutputListWidget::OutputListWidget(ushiro::link<Rescue::State> link, QWidget* parent)
: QTableView(parent)
, mObserver(std::move(link))
{
  mLayout = new QVBoxLayout(this);
  setLayout(mLayout);

  this->setModel(&mModel);

  mObserver.observe([](Rescue::State const& state) { return std::tie(state.outputs, state.group); },
                    [this](Outputs const& outputs, Group const& group) { return updateFrom(outputs, group); });
}

OutputListWidget::~OutputListWidget() = default;

void OutputListWidget::updateFrom(Outputs const& outputs, Group const& group)
{
  mModel.updateFrom(outputs, group);
}

int OutputTreeModel::rowCount(QModelIndex const& parent) const
{
  return mNames.size();
}

int OutputTreeModel::columnCount(QModelIndex const& parent) const
{
  return 2;
}

QVariant OutputTreeModel::data(const QModelIndex& index, int role) const
{
  int row = index.row();
  int col = index.column();

  if (role != Qt::DisplayRole || row > mNames.size())
    return QVariant();

  if (col == 0)
    return mNames[row];
  if (col == 1)
    return mValues[row];

  return QVariant();
}

void OutputTreeModel::updateFrom(Outputs const& outputs, Group const& group)
{
  beginResetModel();

  mNames.clear();
  mValues.clear();

  for (auto const& output : outputs)
  {
    auto name = locate(group, output->id)->name;
    mNames.push_back(name.c_str());
    mValues.push_back(output->value);
  }

  endResetModel();
  if (mNames.empty())
    return;

//  QModelIndex topLeft = createIndex(0,0);
//  QModelIndex bottomRight = createIndex(mNames.size()-1, 1);
//  emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole});
  //emit modelReset();
}

QVariant OutputTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QString("Action");
      case 1:
        return QString("Weight");
    }
  }
  return QVariant();
}
