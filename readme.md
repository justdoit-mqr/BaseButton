# BaseButton
这是一个基于Qt的按钮基础组件项目，旨在封装不同的按钮基类，去实现不同按钮样式与功能需求。  
## 1.BaseToolButton
### 功能概述：
* 该类继承自QToolButton，可以使用父类自身的setToolButtonStyle()方法实现按钮不同的显示风格（文本/图标/文本+图标）。同时因为QToolButton又继承自QAbstractButton，所以该类也可以使用Qt基类按钮的很多通用功能，比如pressed、clicked、autoRepeat等功能，以及使用QButtonGroup来管理单选按钮组，降低了代码复杂度，减少不必要的重复造轮子。  
* 该类重新实现了QAbstractButton的nextCheckState()方法，方便通过check状态模拟按钮的"开关"状态，并可以配置在点击时自动/手动切换状态。  
* 该类重新实现了mousePressEvent()和mouseReleaseEvent()鼠标事件处理方法，添加了防抖和长按的功能处理  
* 该类可使用QToolButton的样式表实现各式各样的按钮样式，在cpp文件中提供了一组默认样式可供参考。  

### 接口函数：
```
    //设置按钮图标和样式
    void setBtnIcon(const QString &iconUrl,QSize iconSize = QSize(32,32),bool scaledUp = false);
    void setBtnIcons(QString normalIcon,QString checkedIcon,QString disabledIcon,QSize iconSize = QSize(32,32));
    void setBtnStyleSheet(const QString &normalStyle="",const QString &pressedStyle="",const QString &checkedStyle="",const QString &disabledStyle="");
    //设置按钮文本居左
    void setBtnTextAlignLeft();
    
    //设置/获取按钮名称
    void setBtnName(QString btnName){this->btnName = btnName;}
    QString getBtnName(){return this->btnName;}
    //设置按钮是否可以自动check
    void setBtnAutoChecked(bool isAutoChecked){this->isAutoChecked = isAutoChecked;}
    //设置按钮防抖属性
    void setBtnAntiShakeProperty(bool antiShakeEnabled,uint antiShakeTime = 200);
    //设置按钮长按属性
    void setBtnLongPresseProperty(bool longPressEnable,uint longPressTime = 3000);
    void releaseBtn();//手动释放按钮
```
## 作者联系方式:
**邮箱:justdoit_mqr@163.com**  
**新浪微博:@为-何-而来**  