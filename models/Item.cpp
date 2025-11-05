#include "Item.h"

namespace hinlibs {

Item::Item(int id, std::string title, std::string creator, int year, ItemKind kind)
    : id_(id), title_(std::move(title)), creator_(std::move(creator)), year_(year), kind_(kind) {}

} // namespace hinlibs
