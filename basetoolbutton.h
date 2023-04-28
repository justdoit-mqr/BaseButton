/****************************************************************************
*
* Copyright (C) 2019-2023 MiaoQingrui. All rights reserved.
* Author: 缪庆瑞 <justdoit_mqr@163.com>
*
****************************************************************************/
/*
 *@author: 缪庆瑞
 *@date:   2019.8.19
 *@brief:  基类工具按钮(提供按钮常用的功能接口实现)
 *
 * 1.该类继承自QToolButton，可以使用父类自身的setToolButtonStyle()方法实现按钮不同的显示风格(文本
 * /图标/文本+图标)。同时因为QToolButton又继承自QAbstractButton，所以该类也可以使用Qt基类按钮的
 * 很多通用功能，比如pressed、clicked、autoRepeat等功能，以及使用QButtonGroup来管理单选按钮组，
 * 降低了代码复杂度，减少不必要的重复造轮子。
 * 2.该类重新实现了QAbstractButton的nextCheckState()方法，方便通过check状态模拟按钮的"开关"状态，
 * 并可以配置在点击时自动/手动切换状态。
 * 3.该类重新实现了mousePressEvent()和mouseReleaseEvent()鼠标事件处理方法，添加了防抖和长按的功
 * 能处理。
 */
#ifndef BASETOOLBUTTON_H
#define BASETOOLBUTTON_H

#include <QToolButton>
#include <QTimer>
#include <QMouseEvent>

class BaseToolButton : public QToolButton
{
    Q_OBJECT
public:
    BaseToolButton(QWidget *parent=0);

    //设置按钮图标
    void setBtnIcon(const QString &iconUrl,QSize iconSize = QSize(32,32),bool scaledUp = false);
    void setBtnIcons(QString normalIcon,QString checkedIcon,QString disabledIcon,QSize iconSize = QSize(32,32));
    //设置按钮文本居左
    void setBtnTextAlignLeft();

    //设置/获取按钮名称
    void setBtnName(QString btnName){this->btnName = btnName;}
    QString getBtnName(){return this->btnName;}
    //设置按钮是否可以自动check
    void setBtnAutoChecked(bool isAutoChecked){this->isAutoChecked = isAutoChecked;}
    //设置按钮防抖属性
    void setBtnAntiShakeProperty(bool antiShakeEnabled,uint antiShakeMs = 200);
    //设置按钮长按属性
    void setBtnLongPressProperty(bool longPressEnabled,uint longPressRespondMs = 3000,
                                 uint longPressMaxMs=3000);
    void releaseBtn();//手动释放按钮

protected:
    virtual void nextCheckState();
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    void initBtnPropertyValue();//初始化按钮属性(参数变量)值

    QString btnName;//按钮名 类似于objectname,存放一些特定信息
    bool isAutoChecked;//是否可以自动(通过点击)切换check状态  默认为true
    /*按钮防抖*/
    bool antiShakeEnabled;//防抖使能标记 默认不使能
    QTimer *antiShakeTimer;//按钮防抖定时器
    uint antiShakeMs;//按钮防抖时间 ms
    /*按钮长按*/
    bool longPressEnabled;//长按使能标记 默认不使能
    QTimer *longPressTimer;//按钮长按定时器
    uint longPressRespondMs;//按钮长按响应时间(长按定时器超时时间) ms
    uint longPressRespondedCount;//按钮长按响应计数(长按定时器的超时次数)
    uint longPressMaxMs;//按钮长按最大时间 ms

signals:
    void longPressSig(uint longPressMs);//长按信号,参数为长按的时间

public slots:
    void longPressTimerSlot();//长按定时器的响应槽
};

#endif // BASETOOLBUTTON_H
