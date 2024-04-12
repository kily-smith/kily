#ifndef LYSSTYLE_H
#define LYSSTYLE_H

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>

class lysStyle
{
public:
    lysStyle();
    ~lysStyle();
    void sytleInit(QMainWindow *windowPtr, QLayout *layoutPtr);

public:
    void pushButtonStyleInit(QPushButton *ptr);
    void comboBoxStyleInit(QComboBox *ptr);
    void lineEditStyleInit(QLineEdit *ptr);
private:
    bool initFlag;
};

#endif // LYSSTYLE_H
