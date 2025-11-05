#include "User.h"

namespace hinlibs {

User::User(int id, std::string name, Role role)
    : id_(id), name_(std::move(name)), role_(role) {}

} // namespace hinlibs
