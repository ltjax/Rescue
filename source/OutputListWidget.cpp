#include <memory>

#include "OutputListWidget.hpp"

using namespace Rescue;

namespace
{
class ValueItemDelegate : public QStyledItemDelegate
{
public:
  QString displayText(const QVariant& value, const QLocale& locale) const override
  {
    return QString("%1").arg(value.toDouble(), 5, 'f', 3);
  }

private:
};
} // namespace
OutputListWidget::OutputListWidget(ushiro::link<Rescue::State> link, QWidget* parent)
: QTableView(parent)
, mObserver(std::move(link))
{
  mLayout = new QVBoxLayout(this);
  setLayout(mLayout);
  mFloatItemDelegate = std::make_unique<ValueItemDelegate>();
  this->setModel(&mModel);
  this->setItemDelegateForColumn(1, mFloatItemDelegate.get());
  this->setColumnWidth(0, 250);
  this->setColumnWidth(1, 150);

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

  if (row > mNames.size())
    return QVariant();

  if (role == Qt::DisplayRole)
  {
    if (col == 0)
      return mNames[row];
    if (col == 1)
      return mValues[row];
  }
  else if (role == Qt::TextAlignmentRole)
  {
    if (col == 1)
      return Qt::AlignRight;
    return Qt::AlignLeft;
  }

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
  // emit modelReset();
}

QVariant OutputTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
  {
    switch (section)
    {
    case 0:
      return QString("Action");
    case 1:
      return QString("Weight");
    default:
      break;
    }
  }
  if (role == Qt::TextAlignmentRole)
  {
    switch (section)
    {
    case 0:
      return Qt::AlignLeft;
    case 1:
      return Qt::AlignRight;
    default:
      break;
    }
  }
  return QVariant();
}
