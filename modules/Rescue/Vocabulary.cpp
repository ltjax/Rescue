#include "Vocabulary.hpp"
#include <boost/uuid/random_generator.hpp>

Rescue::Id Rescue::createId()
{
  static boost::uuids::random_generator generator;
  return generator();
}
