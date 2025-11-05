#include "VideoGame.h"
#include <sstream>

namespace hinlibs {

VideoGame::VideoGame(int id, std::string title, std::string studio, int year,
                     std::string genre, std::string rating)
    : Item(id, std::move(title), std::move(studio), year, ItemKind::VideoGame),
      genre_(std::move(genre)), rating_(std::move(rating)) {}

std::string VideoGame::detailsSummary() const {
    std::ostringstream oss;
    oss << "Studio: " << creator_ << " | Year: " << year_
        << " | Genre: " << genre_ << " | Rating: " << rating_;
    return oss.str();
}

} // namespace hinlibs
