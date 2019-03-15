#pragma once
#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>

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
template <class T> using Ptr = std::shared_ptr<T>;
using Id = boost::uuids::uuid;
} // namespace Rescue
