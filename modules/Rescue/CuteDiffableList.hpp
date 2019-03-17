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

  template <typename ContainerType, typename Inserter>
  void update(ContainerType const& container, Inserter inserter)
  {
    static_assert(std::is_base_of<QWidget, WidgetType>::value, "This only works for QWidgets");
    auto extractId = [](auto const& item)
    {
      return item->id;
    };

    auto deleter = [](auto widget, int /*index*/)
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