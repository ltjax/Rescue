#pragma once
#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <vector>

namespace std
{
template <> struct hash<boost::uuids::uuid>
{
  size_t operator()(const boost::uuids::uuid& uid) const
  {
    return boost::hash<boost::uuids::uuid>()(uid);
  }
};
} // namespace std

namespace Rescue
{
template <typename T> using Ptr = std::shared_ptr<T>;
template <typename T> using PtrList = std::vector<std::shared_ptr<T>>;
using Id = boost::uuids::uuid;

template <class T> struct Identifiable
{
  Identifiable(Id id, Ptr<T const> value)
  : id(id)
  , value(std::move(value))
  {
  }

  bool operator==(Identifiable const& rhs) const
  {
    return id == rhs.id && value == rhs.value;
  }

  bool operator!=(Identifiable const& rhs) const
  {
    return !(rhs == *this);
  };

  Id id;
  Ptr<T const> value;
};

template <class T> inline Ptr<T const> const& locate(std::vector<Ptr<T const>> const& container, Id id)
{
  auto found = std::find_if(container.begin(), container.end(), [&](auto const& x) { return x->id == id; });
  if (found == container.end())
  {
    throw std::out_of_range("Did not find object with given identity");
  }
  return *found;
}

template <class T> inline Ptr<T const>& locate(std::vector<Ptr<T const>>& container, Id id)
{
  auto found = std::find_if(container.begin(), container.end(), [&](auto const& x) { return x->id == id; });
  if (found == container.end())
  {
    throw std::out_of_range("Did not find object with given identity");
  }
  return *found;
}

Id createId();

template <typename T> Ptr<T> clone(Ptr<T const> const& p)
{
  return std::make_shared<T>(*p);
}

} // namespace Rescue
