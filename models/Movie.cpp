#include "Movie.h"
#include <sstream>

namespace hinlibs {

Movie::Movie(int id, std::string title, std::string director, int year,
             std::string genre, std::string rating)
    : Item(id, std::move(title), std::move(director), year, ItemKind::Movie),
      genre_(std::move(genre)), rating_(std::move(rating)) {}

std::string Movie::detailsSummary() const {
    std::ostringstream oss;
    oss << "Director: " << creator_ << " | Year: " << year_
        << " | Genre: " << genre_ << " | Rating: " << rating_;
    return oss.str();
}

} // namespace hinlibs
