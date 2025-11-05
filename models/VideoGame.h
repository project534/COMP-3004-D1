#pragma once
#include "Item.h"

namespace hinlibs {

class VideoGame : public Item {
public:
    VideoGame(int id, std::string title, std::string studio, int year,
              std::string genre, std::string rating);

    const std::string& genre() const noexcept { return genre_; }
    const std::string& rating() const noexcept { return rating_; }

    std::string typeName() const override { return "Video Game"; }
    std::string detailsSummary() const override;

private:
    std::string genre_;
    std::string rating_;
};

} // namespace hinlibs
