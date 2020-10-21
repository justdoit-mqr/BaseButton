/*
 *@author: 缪庆瑞
 *@date:   2019.8.19
 *@brief:  基类工具按钮(提供实现常用按钮的样式和功能接口)
 */
#include "basetoolbutton.h"

/*这里默认提供一组样式表(父类QToolButton作为选择器,使用pressed,checked,disabled伪状态来设置
 * 按钮不同状态的样式,{}内为声明(属性名:属性值))
 * border-radius:边角弧度半径
 * padding:填充区域宽度,可用来控制content区域(比如图标)的大小
 * margin:边缘区域宽度 QToolButton默认的按下下沉效果就是通过该属性控制的
 * color:前景色
 * background-color:背景色
 * 注意:Qt的样式表默认具有子部件继承性，一旦该按钮(BaseToolButton)的父部件显式设置了stylesheet,
 * 那么某些属性就有可能被按钮继承，导致显示异常。解决方法就是父部件设置样式表时指定选择器(特定
 * 某个对象或某个类，不让子部件继承)，或者在该类中显式设置异常的样式属性，避免继承父部件对应的
 * 属性。
 */
#define BTN_NORMAL_STYLE "QToolButton{border-radius:5px;padding:2px;color:black;background-color:lightGray;}"
#define BTN_PRESSED_STYLE "QToolButton:pressed{border-radius:5px;padding:6px;color:black;background-color:orange;}"
#define BTN_CHECKED_STYLE "QToolButton:checked{border-radius:5px;padding:6px;color:black;background-color:orange;}"
#define BTN_DISABLED_STYLE "QToolButton:disabled{border-radius:5px;padding:2px;color:white;background-color:lightGray;}"

/*
 *@brief:   构造函数，根据button类型，构建对应结构的按钮
 *@author:  缪庆瑞
 *@date:    2019.8.20
 *@param:   btnType: 按钮类型
 *@param:   btnIndex:按钮索引号,方便多个按钮关联一个槽，不需要用时默认初始化为-1
 *@param:   parent:父对象
 */
BaseToolButton::BaseToolButton(ButtonType btnType, int btnIndex, QWidget *parent)
    :QToolButton(parent),btnIndex(btnIndex)
{
    this->initBtnPropertyValue();
    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    switch (btnType) {//设置按钮类型
    case TEXT:
        this->setToolButtonStyle(Qt::ToolButtonTextOnly);
        break;
    case ICON:
        this->setToolButtonStyle(Qt::ToolButtonIconOnly);
        break;
    case ICON_TEXT_H:
        this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        break;
    case ICON_TEXT_V:
        this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        break;
    default:
        break;
    }
    /*防抖定时器*/
    antiShakeTimer = new QTimer(this);
    antiShakeTimer->setSingleShot(true);//单次定时器

    //this->setBtnStyleSheet(BTN_NORMAL_STYLE,BTN_PRESSED_STYLE,BTN_CHECKED_STYLE,BTN_DISABLED_STYLE);
}
/*
 *@brief:   设置按钮显示的图标(正常(非选中)状态)
 *@author:  缪庆瑞
 *@date:    2019.8.19
 *@param:   iconUrl:图标路径，可以是ico或png格式图片
 *@param:   iconSize:图标size
 *@param:   scaledUp:图标放大。因为这里图标的大小是通过QAbstractButton::setIconSize()方法
 * 设置的，该方法设置的大小受限于图标文件本身的size，即无法放大。所以这里采用的放大方式
 * 是使用QPixmap放大图标本身，但如果可以的话建议尽量还是选取合适大小的图标，因为靠这里
 * 的放大方式会使图标有些失真。
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
void BaseToolButton::setBtnIcons(QString normalIcon, QString checkedIcon, QString disabledIcon, QSize iconSize)
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
 *@brief:   设置按钮样式表(默认参数为空则采用QToolButton的默认样式)
 *@author:  缪庆瑞
 *@date:    2019.8.19
 *@param:   normalStyle:正常样式 "QToolButton{}"
 *@param:   pressedStyle:按下样式 "QToolButton:pressed{}"
 *@param:   checkedStyle:选中样式 "QToolButton:checked{}"
 *@param:   disabledStyle:禁用样式 "QToolButton:disabled{}"
 */
void BaseToolButton::setBtnStyleSheet(const QString &normalStyle,const QString &pressedStyle,
                                      const QString &checkedStyle,const QString &disabledStyle)
{
    this->setStyleSheet(normalStyle+pressedStyle+checkedStyle+disabledStyle);
}
/*
 *@brief:   设置按钮长按属性
 *@author:  缪庆瑞
 *@date:    2019.12.17
 *@update:  2020.10.21   修改长按定时器的处理，内存按需分配与释放
 *@param:   longPressEnabled:长按使能状态
 *@param:  longPressTime:长按时间 ms
 */
void BaseToolButton::setBtnLongPressProperty(bool longPressEnabled, uint longPressTime)
{
    this->longPressEnabled = longPressEnabled;
    this->longPressTime = longPressTime;
    if(this->longPressEnabled)//长按功能使能
    {
        //初始化长按定时器
        if(longPressTimer == NULL)
        {
            longPressTimer = new QTimer(this);
            longPressTimer->setSingleShot(true);//单次定时器
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
 *@brief:   这是QAbstractButton类提供的一个抽象方法,当按钮被点击时会自动调用。该
 * 方法默认的实现是"calls setChecked(!isChecked()) if the button isCheckable()."，这里重新
 * 实现该方法，通过一个标志变量来控制按钮是否可以通过点击自动切换check状态。
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
    //判断防抖定时器是否为激活状态,激活状态(防抖时间未结束)则直接返回不做响应
    if(antiShakeTimer->isActive())
    {
        return;
    }
    /* 非激活状态则调用父类的mousePressEvent()进行默认处理,并在其后开启防抖定时器
     * 查看QAbstractButton::mousePressEvent()的源码实现可知,如果这里没有调用父类的
     * 处理函数，则在mouseReleaseEvent()处理函数中也不会发出released和clicked信号,
     * 即这里添加防抖处理不会引起信号逻辑混乱。
     */
    QToolButton::mousePressEvent(e);
    //开启防抖定时器
    antiShakeTimer->start(antiShakeTime);
    //如果长按使能，则开启长按定时器
    if(longPressEnabled)
    {
        longPressTimer->start(longPressTime);
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
    antiShakeTime = 200;
    /*注:按钮的长按功能默认是不开启的，同时长按定时器默认情况下也不分配内存空间，
     * 避免资源浪费。*/
    longPressEnabled = false;
    longPressTime = 3000;
    longPressTimer = NULL;
}
/*
 *@brief:   长按定时器的响应槽
 *@author:  缪庆瑞
 *@date:    2019.12.17
 */
void BaseToolButton::longPressTimerSlot()
{
    emit longPressSig();//上报长按信号
}

