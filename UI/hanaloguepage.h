#ifndef HANALOGUEPAGE_H
#define HANALOGUEPAGE_H

#include <QDialog>

namespace Ui {
class HAnaloguePage;
}

class HAnaloguePage : public QDialog
{
    Q_OBJECT

public:
    explicit HAnaloguePage(QWidget *parent = 0);
    ~HAnaloguePage();

private:
    Ui::HAnaloguePage *ui;
};

#endif // HANALOGUEPAGE_H
