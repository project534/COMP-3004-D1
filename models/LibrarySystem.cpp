#include "LibrarySystem.h"

#include "Item.h"
#include "Book.h"
#include "Magazine.h"
#include "Movie.h"
#include "VideoGame.h"

#include "User.h"
#include "Patron.h"
#include "Librarian.h"
#include "Administrator.h"

#include <algorithm>
#include <cctype>

// ---------- helpers ----------
std::string LibrarySystem::toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

std::optional<std::chrono::system_clock::time_point> LibrarySystem::dueIn14Days() {
    using namespace std::chrono;
    return system_clock::now() + hours{24 * 14};
}

Item* LibrarySystem::getItem(const std::string& itemId) {
    auto it = items_.find(itemId);
    return (it == items_.end()) ? nullptr : it->second.get();
}

// ---------- boot / reset ----------
void LibrarySystem::loadDefaultData() {
    items_.clear();
    users_.clear();

    // ----- 5 Fiction Books -----
    for (int i=1;i<=5;++i) {
        auto fb = std::make_unique<FictionBook>();
        fb->id = "FIC" + std::to_string(i);
        fb->title = "Fiction Title " + std::to_string(i);
        fb->creator = "Author F" + std::to_string(i);
        fb->publicationYear = 2000 + i;
        fb->genre = (i%2==0) ? "Drama" : "Sci-Fi";
        items_[fb->id] = std::move(fb);
    }

    // ----- 5 Non-Fiction Books (Dewey required) -----
    for (int i=1;i<=5;++i) {
        auto nfb = std::make_unique<NonFictionBook>();
        nfb->id = "NFC" + std::to_string(i);
        nfb->title = "Non-Fiction Title " + std::to_string(i);
        nfb->creator = "Author NF" + std::to_string(i);
        nfb->publicationYear = 1990 + i;
        // simple Dewey samples like "005.13", "320.11", etc.
        nfb->dewey = (i%2==0) ? "005.13" : "320.11";
        items_[nfb->id] = std::move(nfb);
    }

    // ----- 3 Magazines (issue + pub date) -----
    for (int i=1;i<=3;++i) {
        auto m = std::make_unique<Magazine>();
        m->id = "MAG" + std::to_string(i);
        m->title = "Tech Monthly " + std::to_string(i);
        m->creator = "Editorial Board";
        m->issue = 100 + i;
        m->publicationDate = "2025-0" + std::to_string(i) + "-15";
        items_[m->id] = std::move(m);
    }

    // ----- 3 Movies (genre + rating) -----
    struct { const char* title; const char* genre; const char* rating; } movies[3] = {
        {"The Algorithm", "Thriller", "PG-13"},
        {"Deep Space",    "Sci-Fi",   "PG"},
        {"Quiet City",    "Drama",    "R"}
    };
    for (int i=0;i<3;++i) {
        auto mv = std::make_unique<Movie>();
        mv->id = "MOV" + std::to_string(i+1);
        mv->title = movies[i].title;
        mv->creator = "Director " + std::to_string(i+1);
        mv->genre = movies[i].genre;
        mv->rating = movies[i].rating;
        items_[mv->id] = std::move(mv);
    }

    // ----- 4 Video Games (genre + rating) -----
    struct { const char* title; const char* genre; const char* rating; } games[4] = {
        {"Dungeon Quest", "RPG",       "T"},
        {"Street Racer",  "Racing",    "E"},
        {"Battle Ops",    "Shooter",   "M"},
        {"Puzzle Grid",   "Puzzle",    "E"}
    };
    for (int i=0;i<4;++i) {
        auto vg = std::make_unique<VideoGame>();
        vg->id = "GME" + std::to_string(i+1);
        vg->title = games[i].title;
        vg->creator = "Studio " + std::to_string(i+1);
        vg->genre = games[i].genre;
        vg->rating = games[i].rating;
        items_[vg->id] = std::move(vg);
    }

    // ----- 5 Patrons + 1 Librarian + 1 Admin -----
    for (int i=1;i<=5;++i) {
        auto p = std::make_shared<Patron>();
        p->id = "pat" + std::to_string(i);
        p->name = "Patron " + std::to_string(i);
        users_[toLower(p->name)] = p;
        users_[toLower(p->id)]   = p; // allow login by ID or name
    }

    {
        auto l = std::make_shared<Librarian>();
        l->id = "lib1";
        l->name = "Librarian One";
        users_[toLower(l->name)] = l;
        users_[toLower(l->id)]   = l;
    }
    {
        auto a = std::make_shared<Administrator>();
        a->id = "admin1";
        a->name = "System Administrator";
        users_[toLower(a->name)] = a;
        users_[toLower(a->id)]   = a;
    }
}

void LibrarySystem::resetForRestart() {
    // Per spec: all items -> Available; patrons: zero borrowed items
    for (auto& kv : items_) {
        Item* it = kv.second.get();
        it->status = ItemStatus::Available;
        it->dueDate.reset();
        it->holdQueue.clear(); // not mandated, but prevents stale queues
    }
    for (auto& kv : users_) {
        if (kv.second->type == UserType::Patron) {
            auto p = std::static_pointer_cast<Patron>(kv.second);
            p->activeLoans.clear();
            p->activeHolds.clear();
        }
    }
}

// ---------- login ----------
std::shared_ptr<User> LibrarySystem::findUserByName(const std::string& name) {
    auto it = users_.find(toLower(name));
    if (it != users_.end()) return it->second;
    return nullptr;
}

// ---------- patron features ----------
std::vector<Item*> LibrarySystem::browseCatalogue(bool availableOnly) {
    std::vector<Item*> out;
    out.reserve(items_.size());
    for (auto& kv : items_) {
        Item* it = kv.second.get();
        if (!availableOnly || it->status == ItemStatus::Available) {
            out.push_back(it);
        }
    }
    // Optional: sort by title for nicer display
    std::sort(out.begin(), out.end(), [](Item* a, Item* b){ return a->title < b->title; });
    return out;
}

bool LibrarySystem::borrowItem(Patron& p, const std::string& itemId) {
    // Enforce loan cap
    if ((int)p.activeLoans.size() >= kMaxLoans) return false;

    Item* it = getItem(itemId);
    if (!it) return false;

    // Item must be Available
    if (it->status != ItemStatus::Available) return false;

    // If there is a hold queue, enforce FIFO fairness:
    // Only let the front of the queue borrow (if any), otherwise no one.
    if (!it->holdQueue.empty()) {
        if (it->holdQueue.front() != p.id) return false;
        // If this patron is first, pop them (fulfilling their hold)
        it->holdQueue.pop_front();
        // remove from patron's activeHolds list if present
        auto& holds = p.activeHolds;
        holds.erase(std::remove(holds.begin(), holds.end(), itemId), holds.end());
    }

    // All good: borrow it
    it->status = ItemStatus::CheckedOut;
    it->dueDate = dueIn14Days();
    p.activeLoans.push_back(itemId);
    return true;
}

bool LibrarySystem::returnItem(Patron& p, const std::string& itemId) {
    Item* it = getItem(itemId);
    if (!it) return false;

    // Patron must currently have this loan
    if (!p.hasLoan(itemId)) return false;

    // Process return
    it->status = ItemStatus::Available;
    it->dueDate.reset();

    // Remove from patron's active loans
    auto& loans = p.activeLoans;
    loans.erase(std::remove(loans.begin(), loans.end(), itemId), loans.end());

    // Note: We keep holds queue as-is (FIFO). UI can show that the first in queue
    // will be able to borrow next; spec doesn’t require “on hold” shelf logic.
    return true;
}

bool LibrarySystem::placeHold(Patron& p, const std::string& itemId) {
    Item* it = getItem(itemId);
    if (!it) return false;

    // Holds allowed only on checked-out items
    if (it->status != ItemStatus::CheckedOut) return false;

    // Already holding or already borrowed? Don’t duplicate
    if (p.hasHold(itemId) || p.hasLoan(itemId)) return false;

    // Already in queue?
    if (std::find(it->holdQueue.begin(), it->holdQueue.end(), p.id) != it->holdQueue.end())
        return false;

    it->holdQueue.push_back(p.id);
    p.activeHolds.push_back(itemId);
    return true;
}

bool LibrarySystem::cancelHold(Patron& p, const std::string& itemId) {
    Item* it = getItem(itemId);
    if (!it) return false;

    // If patron isn’t in the queue, nothing to do
    auto pos = std::find(it->holdQueue.begin(), it->holdQueue.end(), p.id);
    if (pos == it->holdQueue.end()) return false;

    it->holdQueue.erase(pos); // remove from queue

    // remove from patron’s list
    auto& holds = p.activeHolds;
    holds.erase(std::remove(holds.begin(), holds.end(), itemId), holds.end());
    return true;
}
