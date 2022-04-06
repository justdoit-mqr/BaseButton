/*
 *@author:  缪庆瑞
 *@date:    2020.04.03
 *@brief:   用来测试基类按钮的功能接口
 */
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    initBaseToolButtonUi();

}

Widget::~Widget()
{
    delete ui;
}
/*
 *@brief:   初始化BaseToolButton功能测试界面
 *@author:  缪庆瑞
 *@date:    2020.04.03
 */
void Widget::initBaseToolButtonUi()
{
    //文本(图标)
    BaseToolButton *textOrIconBtn[4];
    textOrIconBtn[0] = new BaseToolButton(BaseToolButton::ICON_TEXT_H);
    textOrIconBtn[0]->setText("adcd");
    textOrIconBtn[0]->setCheckable(true);
    textOrIconBtn[0]->setBtnTextAlignLeft();

    textOrIconBtn[1] = new BaseToolButton(BaseToolButton::TEXT);
    textOrIconBtn[1]->setText("中文");
    textOrIconBtn[1]->setBtnStyleSheet(
                "QToolButton{border: 1px solid white;background-color:lightGray;}",
                "QToolButton:pressed{border: 1px solid black;background-color:cyan;}"
                );

    textOrIconBtn[2] = new BaseToolButton(BaseToolButton::ICON);
    textOrIconBtn[2]->setBtnIcon("./images/1.ico",QSize(60,60));
    textOrIconBtn[2]->setCheckable(true);
    textOrIconBtn[2]->setBtnAutoChecked(false);

    textOrIconBtn[3] = new BaseToolButton(BaseToolButton::ICON);
    textOrIconBtn[3]->setBtnIcon("./images/2.ico",QSize(100,100));
    textOrIconBtn[3]->setBtnStyleSheet("QToolButton{background-color:cyan;border-radius:20px;}",
                "QToolButton:pressed{background-color:rgb(228,215,243,125);border-radius:20px;}"
                );

    ui->baseToolButtonHLayout1->addWidget(textOrIconBtn[0]);
    ui->baseToolButtonHLayout1->addWidget(textOrIconBtn[1]);
    ui->baseToolButtonHLayout1->addWidget(textOrIconBtn[2]);
    ui->baseToolButtonHLayout1->addWidget(textOrIconBtn[3]);

    //文本+图标
    BaseToolButton *textAndIconBtn[4];
    textAndIconBtn[0] = new BaseToolButton(BaseToolButton::ICON_TEXT_H);
    textAndIconBtn[0]->setText("QQ");
    textAndIconBtn[0]->setBtnIcon("./images/1.ico",QSize(40,40));
    textAndIconBtn[0]->setBtnAntiShakeProperty(true,1000);
    connect(textAndIconBtn[0],SIGNAL(clicked()),this,SLOT(btnClickedSlot()));

    textAndIconBtn[1] = new BaseToolButton(BaseToolButton::ICON_TEXT_V);
    textAndIconBtn[1]->setText("WeChat");
    textAndIconBtn[1]->setBtnIcon("./images/4.ico",QSize(80,80),true);
    textAndIconBtn[1]->setBtnStyleSheet(
                "QToolButton{border:none;color:white;border-image:url(./images/4.ico);background-color:rgba(0,0,0,127)}"
                );

    textAndIconBtn[2] = new BaseToolButton(BaseToolButton::ICON_TEXT_V);
    textAndIconBtn[2]->setText("weibo");
    textAndIconBtn[2]->setBtnIcon("./images/2.ico",QSize(72,72));
    textAndIconBtn[2]->setBtnStyleSheet(
                "QToolButton{border:1px solid rgba(255,255,255,70);background-color:rgba(255,255,255,0)}",
                "QToolButton:pressed{background-color:rgba(50,50,50,127);}",
                ""
                "QToolButton:disabled{color:red;}"
                );

    textAndIconBtn[3] = new BaseToolButton(BaseToolButton::ICON_TEXT_H);
    textAndIconBtn[3]->setText("微信");
    textAndIconBtn[3]->setBtnIcon("./images/4.ico",QSize(40,40));
    textAndIconBtn[3]->setBtnLongPressProperty(true);
    connect(textAndIconBtn[3],SIGNAL(longPressSig()),this,SLOT(btnLongPressSlot()));

    ui->baseToolButtonHLayout2->addWidget(textAndIconBtn[0]);
    ui->baseToolButtonHLayout2->addWidget(textAndIconBtn[1]);
    ui->baseToolButtonHLayout2->addWidget(textAndIconBtn[2]);
    ui->baseToolButtonHLayout2->addWidget(textAndIconBtn[3]);

    //单选按钮组
    btnGroup = new QButtonGroup(this);
    BaseToolButton *groupBtn[4];
    groupBtn[0] = new BaseToolButton(BaseToolButton::ICON);
    groupBtn[0]->setCheckable(true);
    groupBtn[0]->setBtnIcon("./images/2.ico");
    groupBtn[0]->setBtnName("groupBtn[0]");
    groupBtn[0]->setBtnStyleSheet(
                "QToolButton{border-radius:5px;padding:2px;color:black;background-color:lightGray}",
                "QToolButton:pressed{border-radius:5px;padding:2px;color:black;background-color:orange}",
                "QToolButton:checked{border-radius:5px;padding:2px;color:black;background-color:blue}"
                );
    groupBtn[1] = new BaseToolButton(BaseToolButton::ICON);
    groupBtn[1]->setCheckable(true);
    groupBtn[1]->setBtnIcon("./images/4.ico");
    groupBtn[1]->setBtnName("groupBtn[1]");
    groupBtn[1]->setBtnStyleSheet(
                "QToolButton{border-radius:5px;padding:2px;color:black;background-color:lightGray}",
                "QToolButton:pressed{border-radius:5px;padding:2px;color:black;background-color:orange}",
                "QToolButton:checked{border-radius:5px;padding:2px;color:black;background-color:blue}"
                );
    groupBtn[2] = new BaseToolButton(BaseToolButton::ICON);
    groupBtn[2]->setCheckable(true);
    groupBtn[2]->setBtnIcon("./images/3.ico");
    groupBtn[2]->setBtnName("groupBtn[2]");
    groupBtn[2]->setBtnStyleSheet(
                "QToolButton{border-radius:5px;padding:2px;color:black;background-color:lightGray}",
                "QToolButton:pressed{border-radius:5px;padding:2px;color:black;background-color:orange}",
                "QToolButton:checked{border-radius:5px;padding:2px;color:black;background-color:blue}"
                );
    groupBtn[3] = new BaseToolButton(BaseToolButton::ICON);
    groupBtn[3]->setCheckable(true);
    groupBtn[3]->setBtnIcon("./images/1.ico");
    groupBtn[3]->setBtnName("groupBtn[3]");
    groupBtn[3]->setBtnStyleSheet(
                "QToolButton{border-radius:5px;padding:2px;color:black;background-color:lightGray}",
                "QToolButton:pressed{border-radius:5px;padding:2px;color:black;background-color:orange}",
                "QToolButton:checked{border-radius:5px;padding:2px;color:black;background-color:blue}"
                );

    btnGroup->addButton(groupBtn[0],4);
    btnGroup->addButton(groupBtn[1],3);
    btnGroup->addButton(groupBtn[2],2);
    btnGroup->addButton(groupBtn[3],1);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(singleButtonsSlot(int)));

    ui->baseToolButtonHLayout3->addWidget(groupBtn[0]);
    ui->baseToolButtonHLayout3->addWidget(groupBtn[1]);
    ui->baseToolButtonHLayout3->addWidget(groupBtn[2]);
    ui->baseToolButtonHLayout3->addWidget(groupBtn[3]);
}
/*
 *@brief:   按钮组点击响应槽
 *@author:  缪庆瑞
 *@date:    2020.04.03
 *@param:   btnIndex:按钮组按钮index
 */
void Widget::singleButtonsSlot(int btnIndex)
{
    qDebug()<<"group index:"<<btnIndex;
    BaseToolButton *btn = qobject_cast<BaseToolButton *>(btnGroup->button(btnIndex));
    qDebug()<<"btn name:"<<btn->getBtnName();
}
//防抖测试
void Widget::btnClickedSlot()
{
    qDebug()<<QTime::currentTime().toString("HH:mm:ss.zzz")<<"textAndIconBtn[0] btn clicked.";
}
//长按测试
void Widget::btnLongPressSlot()
{
    qDebug()<<"textAndIconBtn[3] btn long press";
}

