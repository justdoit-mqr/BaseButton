/*
 *@author:  缪庆瑞
 *@date:    2020.04.03
 *@brief:   用来测试基类按钮的功能接口
 */
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include "basetoolbutton.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    void initBaseToolButtonUi();//初始化BaseToolButton功能测试界面

    Ui::Widget *ui;
    QButtonGroup *btnGroup;

public slots:
    void singleButtonsSlot(int btnIndex);
    void btnClickedSlot();
    void btnLongPressSlot();

};

#endif // WIDGET_H
