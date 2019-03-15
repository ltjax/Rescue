#pragma once


#include <utility>
#include <vector>
#include <unordered_set>

namespace ushiro
{
template <typename KeyType, typename ItemType>
class diffable_list
{
public:
  template <typename T, typename Hasher>
  static bool contains(std::unordered_set<T, Hasher> const& set, T rhs)
  {
    return set.find(rhs) != set.end();
  }

  template <typename T, typename ExtractId, typename InsertItem, typename RemoveItem>
  void update(T const& container, ExtractId&& extract_id, InsertItem&& insert_item, RemoveItem&& remove_item)
  {
    // Build a set of new ids
    std::unordered_set<KeyType> newIds;
    for (auto const& each : container)
      newIds.insert(extract_id(each));

    // Delete those that are no longer necessary
    auto target = children_.begin();
    for (auto i = target; i != children_.end(); ++i)
    {
      auto const& each = *i;

      if (contains(newIds, each.first))
      {
        *target++ = *i;
      }
      else
      {
        remove_item(std::move(each.second));
      }
    }
    children_.erase(target, children_.end());

    // Add new widgets
    int targetIndex = 0;
    for (auto const& each : container)
    {
      if (targetIndex < children_.size() && extract_id(each) == children_[targetIndex].first)
      {
        targetIndex++;
        continue;
      }
      ItemType widget = insert_item(each, targetIndex);
      children_.insert(children_.begin() + targetIndex, std::make_pair(extract_id(each), std::move(widget)));
      targetIndex++;
    }
  }

private:
  std::vector<std::pair<KeyType, ItemType>> children_;
};

}


