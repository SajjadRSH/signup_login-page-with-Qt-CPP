#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class startWindow; }
QT_END_NAMESPACE

class startWindow : public QMainWindow
{
    Q_OBJECT

public:
    startWindow(QWidget *parent = nullptr);
    ~startWindow();

private slots:
    void on_eyeBTN_clicked();

    void on_editUsername_textChanged(const QString &);

    void on_editPassword_textChanged(const QString &);

    void on_loginBTN_clicked();

    void on_signupBTN_clicked();

    void on_eyeBTN_2_clicked();

    void on_eyeBTN_3_clicked();

    void on_editUsername_2_textChanged(const QString &);

    void on_editPassword_2_textChanged(const QString &);

    void on_editPassword_3_textChanged(const QString &);

    void on_refreshBTN_clicked();

    void on_signupBTN_2_clicked();

private:
    Ui::startWindow *ui;
    bool showPass, showPass_2, showPass_3;
    const QString dictionary = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString captchaCode;
    void hideLoginPage();
    void hideSignupPage();
    void showSignupPage();
    void showLoginPage();
    int checkUsername(QLineEdit*);
    bool checkPassword(QLineEdit*);
    int searchUser(QString, QString);
    int checkConfrimPass();
    QString generateRandomTxt(int);
    QPixmap generateCaptchaImg(const QString &);
};
#endif // STARTWINDOW_H
