#include "PercussionSet.hpp"

namespace z2sound
{

void PercussionSet::set_entry(Key key, PercussionSet::PercussionEntry entry)
{
  key_map_.insert_or_assign(key, entry);
}

}