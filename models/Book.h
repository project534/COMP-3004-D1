#pragma once
#include "Item.h"
#include <optional>

namespace hinlibs {

enum class BookType { Fiction, NonFiction };

class Book : public Item {
public:
    // For NonFiction, provide dewey (e.g., "641.59"). For Fiction, leave std::nullopt.
    Book(int id, std::string title, std::string author, int year,
         BookType type, std::optional<std::string> dewey = std::nullopt);

    BookType bookType() const noexcept { return type_; }
    const std::optional<std::string>& dewey() const noexcept { return dewey_; }

    std::string typeName() const override;
    std::string detailsSummary() const override;

private:
    BookType type_;
    std::optional<std::string> dewey_;
};

} // namespace hinlibs
