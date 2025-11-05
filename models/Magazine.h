#pragma once
#include "Item.h"
#include <QDate>

namespace hinlibs {

class Magazine : public Item {
public:
    Magazine(int id, std::string title, std::string publisher, int year,
             int issueNumber, QDate publicationDate);

    int issueNumber() const noexcept { return issueNumber_; }
    const QDate& publicationDate() const noexcept { return publicationDate_; }

    std::string typeName() const override { return "Magazine"; }
    std::string detailsSummary() const override;

private:
    int issueNumber_;
    QDate publicationDate_;
};

} // namespace hinlibs
