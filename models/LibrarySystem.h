#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <optional>
#include <chrono>

// fwd decls
class Item;
class User;
class Patron;

class LibrarySystem {
public:
    // Boot & reset
    void loadDefaultData();  // 20 items + 7 users (in memory)
    void resetForRestart();  // items -> Available; clear due dates; patrons: 0 loans/holds

    // Login / role routing
    std::shared_ptr<User> findUserByName(const std::string& name);

    // Patron features (per spec)
    std::vector<Item*> browseCatalogue(bool availableOnly = false); // default: show all
    bool borrowItem(Patron& p, const std::string& itemId);
    bool returnItem(Patron& p, const std::string& itemId);
    bool placeHold(Patron& p, const std::string& itemId);
    bool cancelHold(Patron& p, const std::string& itemId);

    // Helper
    static std::optional<std::chrono::system_clock::time_point> dueIn14Days();

    // Accessors for UI (optional)
    Item* getItem(const std::string& itemId);

private:
    // storage (IDs map to objects)
    std::unordered_map<std::string, std::unique_ptr<Item>> items_;
    std::unordered_map<std::string, std::shared_ptr<User>> users_;

    // rules
    static constexpr int kMaxLoans = 3;

    // internal helpers
    static std::string toLower(std::string s);
};
