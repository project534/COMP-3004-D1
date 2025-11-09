#include "Item.h"

namespace hinlibs {

int Item::item_id = 0;

Item::Item(
            const std::string& title,
            const std::string& creator,
            int publicationYear,
            ItemKind kind
        ): id_(++item_id),
           title_(title),
           creator_(creator),
           publicationYear_(publicationYear),
           kind_(kind),
           status_(ItemStatus::Available),
           condition_(Condition::Undamaged),
           checkoutStart_(QDate::currentDate()),
           checkoutEnd_(QDate::currentDate()) {}

// Getters
int Item::id() const noexcept {
    return id_;
}

const std::string& Item::title() const noexcept {
    return title_;
}

const std::string& Item::creator() const noexcept {
    return creator_;
}

int Item::publicationYear() const noexcept {
    return publicationYear_;
}

ItemKind Item::kind() const noexcept {
    return kind_;
}

Condition Item::condition() const noexcept {
    return condition_;
}

const QDate& Item::checkoutStart() const noexcept {
    return checkoutStart_;
}

const QDate& Item::checkoutEnd() const noexcept {
    return checkoutEnd_;
}

ItemStatus Item::status() const noexcept {
    return  status_;
}

// Setters
void Item::setStatus(ItemStatus s) noexcept {
    status_ = s;
}

void Item::setCondition(Condition c) noexcept {
    condition_ = c;
}

void Item::setCheckOutAndEndDates(const QDate& start, const QDate& end) noexcept {
    checkoutStart_ = start;
    checkoutEnd_ = end;
}

void Item::setitemQueue(QQueue<int> holdQueue){
    itemQueue_  = holdQueue;
};


// Queue Operations
void Item::addPatronIdToHoldQueue(int patronID) {
    if (!itemQueue_.contains(patronID)){
        itemQueue_.enqueue(patronID);
    }
}

int Item::nextInHoldQueue() const {
    if (Item::holdQueueSize() != 0){
        return itemQueue_.head();
       }
    return -1;
}

int Item::removeNextInHoldQueue() {
    if (Item::holdQueueSize() != 0){
        return itemQueue_.dequeue();
    }
    return -1;
}

void Item::removePatronIdFromHoldQueue(int patronID) {

    if(Item::holdQueueSize() == 0){
        return;
    }
    if(!(Item::patronIsInHoldQueue(patronID))){
        return;
    }

    QQueue<int> newHoldQueue;
    while (Item::holdQueueSize() != 0) {
        int currQueueData = itemQueue_.dequeue();
        if (currQueueData != patronID){
            newHoldQueue.enqueue(currQueueData);
        }
    }
    Item::setitemQueue(newHoldQueue);
}

bool Item::patronIsInHoldQueue(int patronID) const {
    return itemQueue_.contains(patronID);
}

int Item::positionInHoldQueue(int patronID) const {
    if(Item::holdQueueSize() == 0){
        return -1;
    }
    int queuePosition = 1;
    for (const int& patronid : itemQueue_) {
        if (patronid == patronID){
            return queuePosition;
        }
        queuePosition++;
    }
    return -1;
}

int Item::holdQueueSize() const {
    return itemQueue_.size();
}

} // namespace hinlibs
