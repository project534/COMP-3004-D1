#pragma once
#include <string>

namespace hinlibs {

enum class ItemStatus { Available, CheckedOut };
enum class ItemKind   { Book, Movie, VideoGame, Magazine };

class Item {
public:
    Item(int id, std::string title, std::string creator, int year, ItemKind kind);
    virtual ~Item() = default;

    int id() const noexcept { return id_; }
    const std::string& title() const noexcept { return title_; }
    const std::string& creator() const noexcept { return creator_; }
    int year() const noexcept { return year_; }
    ItemKind kind() const noexcept { return kind_; }

    ItemStatus status() const noexcept { return status_; }
    void setStatus(ItemStatus s) noexcept { status_ = s; }

    // For UI display
    virtual std::string typeName() const = 0;
    virtual std::string detailsSummary() const = 0;

protected:
    int id_;
    std::string title_;
    std::string creator_;
    int year_;
    ItemKind kind_;
    ItemStatus status_{ItemStatus::Available};
};

} // namespace hinlibs
