#pragma once
#include "User.h"

namespace hinlibs {

class Patron : public User {
public:
    explicit Patron(int id, std::string name)
        : User(id, std::move(name), Role::Patron) {}
    ~Patron() override = default;
};

} // namespace hinlibs
