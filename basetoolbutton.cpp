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
 */
#include "basetoolbutton.h"

/*这里默认提供一组样式表(父类QToolButton作为选择器,使用pressed,checked,disabled伪状态来设置
 * 按钮不同状态的样式,{}内为声明(属性名:属性值))
 * border-radius:边角弧度半径    padding:填充区域宽度,可用来控制content区域(比如图标)的大小
 * margin:边缘区域宽度 QToolButton默认的按下下沉效果就是通过该属性控制的
 * color:前景色   background-color:背景色
 * 注意:Qt的样式表默认具有子部件继承性，一旦该按钮(BaseToolButton)的父部件显式设置了stylesheet,
 * 那么某些属性就有可能被按钮继承，导致显示异常。解决方法就是父部件设置样式表时指定选择器(特定某个对
 * 象或某个类，不让子部件继承)，或者在该类中显式设置异常的样式属性，避免继承父部件对应的属性。
 */
#define BTN_STYLE "QToolButton{border-radius:5px;padding:2px;color:black;background-color:lightGray;}\
QToolButton:pressed{padding:6px;background-color:orange;}\
QToolButton:checked{padding:6px;background-color:orange;}\
QToolButton:disabled{padding:2px;color:white;background-color:lightGray;}"
/*
 *@brief:   构造函数
 *@author:  缪庆瑞
 *@date:    2019.8.20
 *@param:   parent:父对象
 */
BaseToolButton::BaseToolButton(QWidget *parent)
    :QToolButton(parent)
{
    this->initBtnPropertyValue();
    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    //this->setStyleSheet(BTN_STYLE);
}
/*
 *@brief:   设置按钮显示的图标(正常(非选中)状态)
 *@author:  缪庆瑞
 *@date:    2019.8.19
 *@param:   iconUrl:图标路径，可以是ico或png格式图片
 *@param:   iconSize:图标size
 *@param:   scaledUp:图标放大。因为这里图标的大小是通过QAbstractButton::setIconSize()方法设置的，
 *该方法设置的大小受限于图标文件本身的size，即无法放大。所以这里采用的放大方式是使用QPixmap放大图标本身，
 *但如果可以的话建议尽量还是选取合适大小的图标，因为靠这里的放大方式会使图标有些失真。
 */
void BaseToolButton::setBtnIcon(const QString &iconUrl, QSize iconSize, bool scaledUp)
{
    QPixmap iconPixmap(iconUrl);
    if(scaledUp)
    {
        iconPixmap = iconPixmap.scaled(iconSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }
    this->setIcon(QIcon(iconPixmap));
    this->setIconSize(iconSize);
}
/*
 *@brief:   设置按钮不同状态下显示的图标
 * 本例参考自帮助例程:Icons Example，如果是使用样式表则可以通过设置属性参数达到同样的效果
 * 例:qproperty-icon:url() (disabled | active | normal | selected)? (on | off)? )
 *@author:  缪庆瑞
 *@date:    2020.08.10
 *@param:   normalIcon:正常状态图标路径
 *@param:   checkedIcon:选中状态图标路径
 *@param:   disabledIcon:禁用状态图标路径
 *@return:  iconSize:图标尺寸
 */
void BaseToolButton::setBtnIcons(QString normalIcon, QString checkedIcon,
                                 QString disabledIcon, QSize iconSize)
{
    QIcon icon;
    if(!normalIcon.isEmpty())
    {
        icon.addFile(normalIcon,iconSize,QIcon::Normal,QIcon::Off);
    }
    if(!checkedIcon.isEmpty())
    {
        icon.addFile(checkedIcon,iconSize,QIcon::Normal,QIcon::On);
    }
    if(!disabledIcon.isEmpty())
    {
        icon.addFile(disabledIcon,iconSize,QIcon::Disabled,QIcon::Off);
    }
    this->setIcon(icon);
    this->setIconSize(iconSize);
}
/*
 *@brief:   设置按钮文本居左
 * 注1:QToolButton在只显示文本的情况下，默认居中对齐，没有提供方便的接口修改为其他
 * 对齐方式。如果项目中对按钮文本对齐方式统一要求居左/居右时，建议换用QPushButton,
 * 可以参考该类重新实现一个继承QPushButton的按钮基类，添加一些方便的接口。但如果只
 * 是个别按钮文本需要左对齐，且依然想使用该类生成按钮，可以调用该接口实现，不过前提
 * 是按钮类型设置为:ToolButtonTextBesideIcon。
 * 注2:该接口实际是借助QToolButton在水平显示图标+文本时默认居左的特性实现的。
 *@author:  缪庆瑞
 *@date:    2020.10.27
 */
void BaseToolButton::setBtnTextAlignLeft()
{
    this->setIcon(QIcon(QPixmap(1,1)));//Icon必须有效，否则内部会当成ToolButtonTextOnly处理
    this->setIconSize(QSize(0,0));
}
/*
 *@brief:   设置按钮防抖属性
 *@author:  缪庆瑞
 *@date:    2022.4.6
 *@param:   antiShakeEnabled:防抖使能状态
 *@param:   antiShakeMs:防抖时间 ms
 */
void BaseToolButton::setBtnAntiShakeProperty(bool antiShakeEnabled, uint antiShakeMs)
{
    this->antiShakeEnabled = antiShakeEnabled;
    this->antiShakeMs = antiShakeMs;
    if(this->antiShakeEnabled)//防抖功能使能
    {
        //初始化防抖定时器
        if(antiShakeTimer == NULL)
        {
            antiShakeTimer = new QTimer(this);
            antiShakeTimer->setSingleShot(true);//单次定时器
        }
    }
    else//防抖功能禁用
    {
        if(antiShakeTimer != NULL)
        {
            antiShakeTimer->deleteLater();//清理防抖定时器
            antiShakeTimer = NULL;
        }
    }
}
/*
 *@brief:   设置按钮长按属性
 *@author:  缪庆瑞
 *@date:    2019.12.17
 *@param:   longPressEnabled:长按使能状态
 *@param:   longPressRespondMs:长按响应时间 ms
 *@param:   longPressMaxMs:长按最大时间 ms
 */
void BaseToolButton::setBtnLongPressProperty(bool longPressEnabled, uint longPressRespondMs,
                                             uint longPressMaxMs)
{
    this->longPressEnabled = longPressEnabled;
    this->longPressRespondMs = longPressRespondMs;
    this->longPressMaxMs = longPressMaxMs;
    if(this->longPressEnabled)//长按功能使能
    {
        //初始化长按定时器
        if(longPressTimer == NULL)
        {
            longPressTimer = new QTimer(this);
            connect(longPressTimer,SIGNAL(timeout()),this,SLOT(longPressTimerSlot()));
            /* 当鼠标在按钮上按下然后脱离按钮区域时会触发released信号，但不会执行mouseReleaseEvent
             * 处理函数(释放鼠标才进入该函数)，所以为了确保鼠标离开按钮区域就不再发送长按信号，这里
             * 关联了released信号和长按定时器的stop槽*/
            connect(this,SIGNAL(released()),longPressTimer,SLOT(stop()));
        }
    }
    else//长按功能禁用
    {
        if(longPressTimer != NULL)
        {
            longPressTimer->deleteLater();//清理长按定时器
            longPressTimer = NULL;
        }
    }
}
/*
 *@brief:   手动释放按钮，避免因长按信号触发(半)模态窗口导致按钮释放操作无法响应
 *@author:  缪庆瑞
 *@date:    2020.02.22
 */
void BaseToolButton::releaseBtn()
{
    /* 模拟一个鼠标释放事件
     * 第二个参数表示鼠标指针相对按钮的位置,这里设置为(-1,-1)表示鼠标是在按钮之外
     * 释放的，这样既不会触发按钮的clicked()信号,也能让按钮(样式)回到正常状态。
     */
    QMouseEvent mouseEvent(QMouseEvent::MouseButtonRelease,
                           QPoint(-1,-1),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    mouseReleaseEvent(&mouseEvent);
}
/*
 *@brief:   这是QAbstractButton类提供的一个抽象方法,当按钮被点击时会自动调用。该方法默认的实现是
 *"calls setChecked(!isChecked()) if the button isCheckable()."，这里重新实现该方法，通过
 *一个标志变量来控制按钮是否可以通过点击自动切换check状态。
 *@author:  缪庆瑞
 *@date:    2019.09.03
 */
void BaseToolButton::nextCheckState()
{
    if(isAutoChecked)
    {
        QToolButton::nextCheckState();
    }
}
/*
 *@brief:   鼠标按下事件处理  重写该函数给按钮添加防抖和长按功能
 *@author:  缪庆瑞
 *@date:    2019.09.11
 *@param:   e:鼠标事件
 */
void BaseToolButton::mousePressEvent(QMouseEvent *e)
{
    //判断防抖是否使能以及防抖定时器是否为激活状态,激活状态(防抖时间未结束)则直接返回不做响应
    if(antiShakeEnabled && antiShakeTimer->isActive())
    {
        return;
    }
    /* 非激活状态则调用父类的mousePressEvent()进行默认处理,并在其后根据使能状态开启定时器
     * 查看QAbstractButton::mousePressEvent()的源码实现可知,如果这里没有调用父类的
     * 处理函数，则在mouseReleaseEvent()处理函数中也不会发出released和clicked信号,
     * 即这里添加防抖处理不会引起信号逻辑混乱。
     */
    QToolButton::mousePressEvent(e);
    //如果防抖使能，则开启防抖定时器
    if(antiShakeEnabled)
    {
        antiShakeTimer->start(antiShakeMs);
    }
    //如果长按使能，则开启长按定时器
    if(longPressEnabled)
    {
        longPressRespondedCount = 0;
        longPressTimer->start(longPressRespondMs);
    }
}
/*
 *@brief:   鼠标释放事件处理  重写添加长按功能
 *@author:  缪庆瑞
 *@date:    2019.12.17
 *@param:   e:鼠标事件
 */
void BaseToolButton::mouseReleaseEvent(QMouseEvent *e)
{
    //如果长按使能，则关闭长按定时器
    if(longPressEnabled)
    {
        longPressTimer->stop();
    }
    QToolButton::mouseReleaseEvent(e);
}
/*
 *@brief:   初始化按钮属性(参数变量)值
 *@author:  缪庆瑞
 *@date:    2020.03.13
 */
void BaseToolButton::initBtnPropertyValue()
{
    btnName = "";
    isAutoChecked = true;
    /* 注:按钮的长按和防抖功能默认是不开启的，同时对应定时器
     * 默认情况下也不分配内存空间，避免资源浪费。*/
    //防抖
    antiShakeEnabled = false;
    antiShakeMs = 200;
    antiShakeTimer = NULL;
    //长按
    longPressEnabled = false;
    longPressRespondMs = 3000;
    longPressRespondedCount = 0;
    longPressMaxMs = 3000;
    longPressTimer = NULL;
}
/*
 *@brief:   长按定时器的响应槽
 *@author:  缪庆瑞
 *@date:    2019.12.17
 */
void BaseToolButton::longPressTimerSlot()
{
    longPressRespondedCount++;
    if(longPressRespondedCount*longPressRespondMs >= longPressMaxMs)
    {
        longPressTimer->stop();
    }
    emit longPressSig(longPressRespondedCount*longPressRespondMs);//上报长按信号
}

