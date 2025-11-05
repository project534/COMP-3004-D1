#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include <QMessageBox>
#include "gui/PatronWindow.h"

LoginWindow::LoginWindow(std::shared_ptr<hinlibs::LibrarySystem> system, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::LoginWindow), system_(std::move(system)) {
    ui->setupUi(this);
    // expects: QLineEdit usernameEdit, QPushButton loginButton
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::onLogin);
}

LoginWindow::~LoginWindow() = default;

void LoginWindow::onLogin() {
    const auto name = ui->usernameEdit->text().toStdString();
    auto user = system_->findUserByName(name);
    if (!user) {
        QMessageBox::warning(this, "Login Failed", "User not found.");
        return;
    }
    if (user->role() != hinlibs::Role::Patron) {
        QMessageBox::information(this, "Not Implemented",
                                 "Only Patron features are implemented for D1.");
        return;
    }

    auto patron = std::static_pointer_cast<hinlibs::Patron>(user);
    auto* win = new PatronWindow(system_, patron);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->close(); // clear previous user context per D1
}
