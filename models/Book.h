#pragma once
#include "Item.h"

class Book : public Item {
public:
    int publicationYear{};
    std::string format() const override { return "Book"; }
};

class FictionBook : public Book {
public:
    std::string genre; // optional detail
    std::string format() const override { return "Fiction Book"; }
};

class NonFictionBook : public Book {
public:
    std::string dewey; // REQUIRED by spec
    std::string format() const override { return "Non-Fiction Book"; }
};
