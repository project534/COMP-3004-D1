#include "Book.h"
#include <sstream>

namespace hinlibs {

Book::Book(int id, std::string title, std::string author, int year,
           BookType type, std::optional<std::string> dewey)
    : Item(id, std::move(title), std::move(author), year, ItemKind::Book),
      type_(type), dewey_(std::move(dewey)) {}

std::string Book::typeName() const {
    return (type_ == BookType::Fiction) ? "Fiction Book" : "Non-Fiction Book";
}

std::string Book::detailsSummary() const {
    std::ostringstream oss;
    oss << "Author: " << creator_ << " | Year: " << year_;
    if (type_ == BookType::NonFiction && dewey_.has_value()) {
        oss << " | Dewey: " << *dewey_;
    }
    return oss.str();
}

} // namespace hinlibs
