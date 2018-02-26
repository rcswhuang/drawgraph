#ifndef HRELAYPAGE_H
#define HRELAYPAGE_H

#include <QDialog>

namespace Ui {
class HRelayPage;
}

class HRelayPage : public QDialog
{
    Q_OBJECT

public:
    explicit HRelayPage(QWidget *parent = 0);
    ~HRelayPage();

private:
    Ui::HRelayPage *ui;
};

#endif // HRELAYPAGE_H
