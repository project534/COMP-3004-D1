#include "Magazine.h"
#include <sstream>

namespace hinlibs {

Magazine::Magazine(int id, std::string title, std::string publisher, int year,
                   int issueNumber, QDate publicationDate)
    : Item(id, std::move(title), std::move(publisher), year, ItemKind::Magazine),
      issueNumber_(issueNumber), publicationDate_(publicationDate) {}

std::string Magazine::detailsSummary() const {
    std::ostringstream oss;
    oss << "Publisher: " << creator_ << " | Year: " << year_
        << " | Issue: " << issueNumber_
        << " | Published: " << publicationDate_.toString("yyyy-MM-dd").toStdString();
    return oss.str();
}

} // namespace hinlibs
