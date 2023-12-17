#include "startwindow.h"
#include "ui_startwindow.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include <QMessageBox>
#include <QStringList>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>

#include <QRandomGenerator>
#include <QPainter>

QRegularExpression username_regex, password_regex;
QRegularExpressionValidator username_regex_val, password_regex_val;

startWindow::startWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::startWindow)
{
    ui->setupUi(this);

    hideSignupPage();

    QSqlDatabase database;
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\myDatabase.db");
    database.open();

    username_regex.setPattern("\\w+");
    password_regex.setPattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[@#$%^&*_.]).{7,12}$");
    username_regex_val.setRegularExpression(username_regex);
    password_regex_val.setRegularExpression(password_regex);

    ui->eyeBTN->setCursor(Qt::PointingHandCursor);
    ui->eyeBTN_2->setCursor(Qt::PointingHandCursor);
    ui->eyeBTN_3->setCursor(Qt::PointingHandCursor);

    ui->eyeBTN->setIcon(QIcon("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\eye-icons\\eye-close.png"));
    ui->eyeBTN_2->setIcon(QIcon("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\eye-icons\\eye-close.png"));
    ui->eyeBTN_3->setIcon(QIcon("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\eye-icons\\eye-close.png"));

    ui->editPassword->setEchoMode(QLineEdit::Password);
    ui->editPassword_2->setEchoMode(QLineEdit::Password);
    ui->editPassword_3->setEchoMode(QLineEdit::Password);

    showPass = showPass_2 = showPass_3 =  false;

    ui->signupBTN->setCursor(Qt::PointingHandCursor);

    ui->editUsername->setToolTip("The valid username can only contain\nletters and numbers and underline character\n6<len<13");
    ui->editPassword->setToolTip("password must contain:\nat least one uppercase\nat least one lowercase\nat least one number\nat least one special character\n6<len<13");
    ui->editUsername_2->setToolTip("The valid username can only contain\nletters and numbers and underline character\n6<len<13");
    ui->editPassword_2->setToolTip("password must contain:\nat least one uppercase\nat least one lowercase\nat least one number\nat least one special character\n6<len<13");

    ui->refreshBTN->setIcon(QIcon("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\refreshCaptcha\\refresh.png"));

    const QPixmap China("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\CountryFlags\\china.png");
    const QPixmap Germany("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\CountryFlags\\germany.png");
    const QPixmap Iran("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\CountryFlags\\iran.png");
    const QPixmap Iraq("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\CountryFlags\\iraq.png");
    const QPixmap Russia("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\CountryFlags\\russia.png");
    const QPixmap UnitedStates("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\CountryFlags\\united_states.png");

    QStringList countries = {"choose country", "China    +86", "Germany    +49", "Iran    +98", "Iraq    +964", "Russia    +7095", "United States    +1"};
    ui->phoneComboBox->addItems(countries);

    ui->phoneComboBox->setItemIcon(1,QIcon(China));
    ui->phoneComboBox->setItemIcon(2,QIcon(Germany));
    ui->phoneComboBox->setItemIcon(3,QIcon(Iran));
    ui->phoneComboBox->setItemIcon(4,QIcon(Iraq));
    ui->phoneComboBox->setItemIcon(5,QIcon(Russia));
    ui->phoneComboBox->setItemIcon(6,QIcon(UnitedStates));

}

startWindow::~startWindow()
{
    delete ui;
}


void startWindow::on_eyeBTN_clicked()
{
    if(showPass)
    {
        showPass = false;
        ui->eyeBTN->setIcon(QIcon("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\eye-icons\\eye-close.png"));
        ui->editPassword->setEchoMode(QLineEdit::Password);
    }

    else
    {
        showPass = true;
        ui->eyeBTN->setIcon(QIcon("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\eye-icons\\eye.png"));
        ui->editPassword->setEchoMode(QLineEdit::Normal);
    }
}


void startWindow::on_editUsername_textChanged(const QString &text)
{
    int pos = 0;
    QString tmp_text = text;

    if(tmp_text == NULL)
    {
        ui->isUsernameCorrect->setStyleSheet("background-color: rgb(255, 255, 255);");
        return;
    }

    if (username_regex_val.validate(tmp_text, pos) == QValidator::Acceptable && tmp_text.length() >= 7 && tmp_text.length() <= 12)
        ui->isUsernameCorrect->setStyleSheet("background-color: rgb(0, 255, 127);");

    else
        ui->isUsernameCorrect->setStyleSheet("background-color: rgb(255, 44, 48);");
}


void startWindow::on_editPassword_textChanged(const QString &text)
{
    int pos = 0;
    QString tmp_text = text;

    if(tmp_text == NULL)
    {
        ui->isPassCorrect->setStyleSheet("background-color: rgb(255, 255, 255);");
        return;
    }

    if (password_regex_val.validate(tmp_text, pos) == QValidator::Acceptable)
        ui->isPassCorrect->setStyleSheet("background-color: rgb(0, 255, 127);");

    else
        ui->isPassCorrect->setStyleSheet("background-color: rgb(255, 44, 48);");
}

int startWindow::checkUsername(QLineEdit* lineEdit)
{
    QString username = lineEdit->text();
    int pos = 0;

    if( !(username_regex_val.validate(username, pos) == QValidator::Acceptable) )
        return 0;

    else if(username.length() > 12 || username.length() < 7)
        return 1;

    return 2;
}

bool startWindow::checkPassword(QLineEdit* lineEdit)
{
    QString password = lineEdit->text();
    int pos = 0;

    if( !(password_regex_val.validate(password, pos) == QValidator::Acceptable) )
        return false;

    else
        return true;
}

int startWindow::searchUser(QString username, QString password)
{
    QSqlQuery query;
    query.exec("SELECT password FROM user WHERE username = '"+ username +"'");

    if(query.first())
    {
        if(password == query.value(0).toString())
            return 1;

        else
            return 0;
    }

    return -1;
}

int startWindow::checkConfrimPass()
{
    if(!checkPassword(ui->editPassword_2))
        return -1;

    else if(ui->editPassword_2->text() != ui->editPassword_3->text())
        return 0;

    return 1;
}

void startWindow::on_loginBTN_clicked()
{
    switch(checkUsername(ui->editUsername))
    {
    case 0:
        QMessageBox::warning(this, "incorrect username", "Enter the username in the correct format");
        return;

    case 1:
        QMessageBox::warning(this, "incorrect username", "Username length must be between 7 and 12");
        return;

    default:
        break;
    }

    //---------------------------------------------------------------------------------------------------

    if(!checkPassword(ui->editPassword))
    {
        QMessageBox::warning(this, "incorrect password", "Enter the password in the correct format");
        return;
    }

    //---------------------------------------------------------------------------------------------------

    switch(searchUser(ui->editUsername->text(), ui->editPassword->text()))
    {
    case -1:
        QMessageBox::warning(this, " ", "user not found");
        ui->editUsername->clear();
        ui->editPassword->clear();
        return;

    case 0:
        QMessageBox::warning(this, " ", "password incorrect");
        return;

    case 1:
        QString tmp_str2;
        tmp_str2 = "Hello " + ui->editUsername->text();
        QMessageBox::information(this, "Welcome", tmp_str2);
        QCoreApplication::quit();
    }
}

void startWindow::hideLoginPage()
{
    ui->loginLBL->hide();
    ui->line_1->hide();
    ui->usernameGroup->hide();
    ui->passwordGroup->hide();
    ui->loginBTN->hide();
    ui->groupBox_1->hide();
}

void startWindow::hideSignupPage()
{
    ui->signupLBL->hide();
    ui->line_6->hide();
    ui->usernameGroup_2->hide();
    ui->passwordGroup_2->hide();
    ui->phoneAndCaptchaBox->hide();
    ui->signupBTN_2->hide();
}

void startWindow::showSignupPage()
{
    ui->signupLBL->show();
    ui->line_6->show();
    ui->usernameGroup_2->show();
    ui->passwordGroup_2->show();
    ui->phoneAndCaptchaBox->show();
    ui->signupBTN_2->show();
}

void startWindow::showLoginPage()
{
    ui->loginLBL->show();
    ui->line_1->show();
    ui->usernameGroup->show();
    ui->passwordGroup->show();
    ui->loginBTN->show();
    ui->groupBox_1->show();
}

void startWindow::on_signupBTN_clicked()
{
    hideLoginPage();
    showSignupPage();
    on_refreshBTN_clicked();
}

void startWindow::on_eyeBTN_2_clicked()
{
    if(showPass_2)
    {
        showPass_2 = false;
        ui->eyeBTN_2->setIcon(QIcon("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\eye-icons\\eye-close.png"));
        ui->editPassword_2->setEchoMode(QLineEdit::Password);
    }

    else
    {
        showPass_2 = true;
        ui->eyeBTN_2->setIcon(QIcon("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\eye-icons\\eye.png"));
        ui->editPassword_2->setEchoMode(QLineEdit::Normal);
    }
}


void startWindow::on_eyeBTN_3_clicked()
{
    if(showPass_3)
    {
        showPass_3 = false;
        ui->eyeBTN_3->setIcon(QIcon("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\eye-icons\\eye-close.png"));
        ui->editPassword_3->setEchoMode(QLineEdit::Password);
    }

    else
    {
        showPass_3 = true;
        ui->eyeBTN_3->setIcon(QIcon("E:\\University\\semester 3\\Advanced Programming\\Qt\\signup_login_page\\eye-icons\\eye.png"));
        ui->editPassword_3->setEchoMode(QLineEdit::Normal);
    }
}

void startWindow::on_editUsername_2_textChanged(const QString &text)
{
    int pos = 0;
    QString tmp_text = text;

    if(tmp_text == NULL)
    {
        ui->isUsernameCorrect_2->setStyleSheet("background-color: rgb(255, 255, 255);");
        return;
    }

    if (username_regex_val.validate(tmp_text, pos) == QValidator::Acceptable && tmp_text.length() >= 7 && tmp_text.length() <= 12)
        ui->isUsernameCorrect_2->setStyleSheet("background-color: rgb(0, 255, 127);");

    else
        ui->isUsernameCorrect_2->setStyleSheet("background-color: rgb(255, 44, 48);");
}


void startWindow::on_editPassword_2_textChanged(const QString &text)
{
    int pos = 0;
    QString tmp_text = text;

    if(tmp_text == NULL)
    {
        ui->isPasswordCorrect_1->setStyleSheet("background-color: rgb(255, 255, 255);");
        return;
    }

    if (password_regex_val.validate(tmp_text, pos) == QValidator::Acceptable)
        ui->isPasswordCorrect_1->setStyleSheet("background-color: rgb(0, 255, 127);");

    else
        ui->isPasswordCorrect_1->setStyleSheet("background-color: rgb(255, 44, 48);");
}


void startWindow::on_editPassword_3_textChanged(const QString &text)
{
    int pos = 0;
    QString tmp_text = text;

    if(tmp_text == NULL)
    {
        ui->isPasswordCorrect_2->setStyleSheet("background-color: rgb(255, 255, 255);");
        return;
    }

    if (password_regex_val.validate(tmp_text, pos) == QValidator::Acceptable)
        ui->isPasswordCorrect_2->setStyleSheet("background-color: rgb(0, 255, 127);");

    else
        ui->isPasswordCorrect_2->setStyleSheet("background-color: rgb(255, 44, 48);");
}

QString startWindow::generateRandomTxt(int len)
{
    QString randomTxt;
    for (int i = 0; i < len; i++)
        {
            int index = QRandomGenerator::global()->bounded(dictionary.length());
            randomTxt.append(dictionary.at(index));
        }
    return randomTxt;
}

QPixmap startWindow::generateCaptchaImg(const QString &text)
{
    QPixmap pixmap(160, 50);
    pixmap.fill(Qt::white);

    QPainter painter(&pixmap);
    painter.setPen(Qt::black);
    QFont font("Arial", 16);
    painter.setFont(font);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, text);

    return pixmap;
}

void startWindow::on_refreshBTN_clicked()
{
    QString captchatext = generateRandomTxt(5);
    captchaCode = captchatext;
    QPixmap captchaimage = generateCaptchaImg(captchatext);
    ui->showCaptcha->setPixmap(captchaimage);
}

void startWindow::on_signupBTN_2_clicked()
{
    QSqlQuery query;
    QString username = ui->editUsername_2->text();
    QString password = ui->editPassword_2->text();
    QString phoneNum = ui->editPhoneNum->text();

    switch(checkUsername(ui->editUsername_2))
    {
    case 0:
        QMessageBox::warning(this, "incorrect username", "Enter the username in the correct format");
        on_refreshBTN_clicked();
        ui->captchaLineEdit->clear();
        return;

    case 1:
        QMessageBox::warning(this, "incorrect username", "Username length must be between 7 and 12");
        on_refreshBTN_clicked();
        ui->captchaLineEdit->clear();
        return;

    default:
        break;
    }

    //--------------------------------------------------------------------------------------------------

    switch(checkConfrimPass())
    {
    case -1:
        QMessageBox::warning(this, "incorrect password", "Enter the password in the correct format");
        on_refreshBTN_clicked();
        ui->captchaLineEdit->clear();
        return;

    case 0:
        QMessageBox::warning(this, " ", "The confirm password does not match with password");
        return;

    default:
        break;
    }

    query.exec("SELECT username FROM user WHERE username = '"+ username +"'");

    if(query.first())
    {
        QMessageBox::warning(this, "duplicate username", "An account with the entered username exists");
        return;
    }

    else if(ui->captchaLineEdit->text() == captchaCode)
    {
        QSqlQuery tmpQuery;
        tmpQuery.exec("INSERT INTO user(username, password, phoneNumber)VALUES('"+ username +"', '"+ password +"', '"+ phoneNum +"')");
        ui->editUsername_2->clear();
        ui->editPassword_2->clear();
        ui->editPassword_3->clear();
        ui->editPhoneNum->clear();
        ui->captchaLineEdit->clear();
        QMessageBox::information(this, "signup successfully", "the account created");
        hideSignupPage();
        showLoginPage();
    }
    else
    {
        QMessageBox::warning(this, " ", "captcha code incorrect");
        on_refreshBTN_clicked();
        ui->captchaLineEdit->clear();
    }
}

