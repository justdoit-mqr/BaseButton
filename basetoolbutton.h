/*
 *@author: 缪庆瑞
 *@date:   2019.8.19
 *@brief:  基类工具按钮(提供实现常用按钮的样式和功能接口)
 *
 * 1.该类继承自QToolButton，可以使用父类自身的setToolButtonStyle()方法实现按钮不同的显示风格(文本
 * /图标/文本+图标)。同时因为QToolButton又继承自QAbstractButton，所以该类也可以使用Qt基类按钮的
 * 很多通用功能，比如pressed、clicked、autoRepeat等功能，以及使用QButtonGroup来管理单选按钮组，
 * 降低了代码复杂度，减少不必要的重复造轮子。
 * 2.该类重新实现了QAbstractButton的nextCheckState()方法，方便通过check状态模拟按钮的"开关"状态，
 * 并可以配置在点击时自动/手动切换状态。
 * 3.该类重新实现了mousePressEvent()和mouseReleaseEvent()鼠标事件处理方法，添加了防抖和长按的功
 * 能处理。
 * 4.该类可使用QToolButton的样式表实现各式各样的按钮样式，在cpp文件中提供了一组默认样式可供参考
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
    enum ButtonType{//按钮类型
        TEXT=0,//纯文本
        ICON,//纯图标
        ICON_TEXT_H,//图标+文本(左右结构)
        ICON_TEXT_V//图标+文本(上下结构)
    };

    BaseToolButton(ButtonType btnType,int btnIndex=-1,QWidget *parent=0);

    //设置按钮图标和样式
    void setBtnIcon(const QString &iconUrl,QSize iconSize = QSize(32,32),bool scaledUp = false);
    void setBtnIcons(QString normalIcon,QString checkedIcon,QString disabledIcon,QSize iconSize = QSize(32,32));
    void setBtnStyleSheet(const QString &normalStyle="",const QString &pressedStyle="",
                          const QString &checkedStyle="",const QString &disabledStyle="");
    //设置按钮文本居左
    void setBtnTextAlignLeft();

    //获取按钮索引号
    int getBtnIndex(){return btnIndex;}
    //设置/获取按钮名称
    void setBtnName(QString btnName){this->btnName = btnName;}
    QString getBtnName(){return this->btnName;}
    //设置按钮是否可以自动check
    void setBtnAutoChecked(bool isAutoChecked){this->isAutoChecked = isAutoChecked;}
    //设置按钮防抖时间 ms
    void setBtnAntiShakeTime(uint antiShakeTime){this->antiShakeTime = antiShakeTime;}

    //设置按钮长按属性
    void setBtnLongPressProperty(bool longPressEnabled,uint longPressTime = 3000);
    void releaseBtn();//手动释放按钮

protected:
    virtual void nextCheckState();
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    void initBtnPropertyValue();//初始化按钮属性(参数变量)值

    int btnIndex;//按钮索引号
    QString btnName;//按钮名 类似于objectname,存放一些特定信息
    bool isAutoChecked;//是否可以自动(通过点击)切换check状态  默认为true
    /*按钮防抖*/
    QTimer *antiShakeTimer;//按钮防抖定时器
    uint antiShakeTime;//按钮防抖时间 ms
    /*按钮长按*/
    bool longPressEnabled;//长按使能标记 默认不使能
    QTimer *longPressTimer;//按钮长按定时器
    uint longPressTime;//按钮长按时间 ms

signals:
    void longPressSig();//长按信号

public slots:
    void longPressTimerSlot();//长按定时器的响应槽
};

#endif // BASETOOLBUTTON_H
