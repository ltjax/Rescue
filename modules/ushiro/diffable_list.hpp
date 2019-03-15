#pragma once


#include <utility>
#include <vector>
#include <unordered_set>

template <typename KeyType, typename WidgetType>
class DiffableItemList
{
public:
  template <typename T, typename Hasher>
  static bool contains(std::unordered_set<T, Hasher> const& set, T rhs)
  {
    return set.find(rhs) != set.end();
  }

  template <typename T, typename GetId, typename InsertWidget, typename RemoveWidget>
  void update(T const& container, GetId&& getId, InsertWidget&& insertWidget, RemoveWidget&& removeWidget)
  {
    // Build a set of new ids
    std::unordered_set<KeyType> newIds;
    for (auto const& each : container)
      newIds.insert(getId(each));

    // Delete those that are no longer necessary
    auto target = mChildren.begin();
    for (auto i = target; i != mChildren.end(); ++i)
    {
      auto const& each = *i;

      if (contains(newIds, each.first))
      {
        *target++ = *i;
      }
      else
      {
        removeWidget(std::move(each.second));
      }
    }
    mChildren.erase(target, mChildren.end());

    // Add new widgets
    int targetIndex = 0;
    for (auto const& each : container)
    {
      if (targetIndex < mChildren.size() && getId(each) == mChildren[targetIndex].first)
      {
        targetIndex++;
        continue;
      }
      WidgetType widget = insertWidget(each, targetIndex);
      mChildren.insert(mChildren.begin() + targetIndex, std::make_pair(getId(each), std::move(widget)));
      targetIndex++;
    }
  }

private:
  std::vector<std::pair<KeyType, WidgetType>> mChildren;
};



