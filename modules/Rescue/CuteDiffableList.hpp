#pragma once
#include <type_traits>
#include <QtWidgets/QWidget>
#include "diffable_list.hpp"
#include "Vocabulary.hpp"

namespace Rescue
{

template <typename WidgetType>
class CuteDiffableList
{
public:
  static_assert(std::is_base_of<QWidget, WidgetType>::value, "This only works for QWidgets");

  template <typename ContainerType, typename Inserter>
  void update(ContainerType const& container, Inserter inserter)
  {
    auto extractId = [](auto const& item)
    {
      return item->id;
    };

    auto deleter = [](auto widget)
    {
      // QWidgets can just be deleted and will deregister automatically
      delete widget;
    };

    mList.update(container, extractId, inserter, deleter);
  }

private:
  ushiro::diffable_list<Id, WidgetType*> mList;
};

}