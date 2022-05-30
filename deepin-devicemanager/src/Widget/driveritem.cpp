#include "driveritem.h"
#include "commontools.h"
#include "BtnLabel.h"

#include <DFontSizeManager>
#include <DApplicationHelper>
#include <DApplication>
#include <DToolTip>

#include <QHBoxLayout>

DriverCheckItem::DriverCheckItem(DWidget *parent, bool header)
    : DWidget(parent)
    , mp_cb(new DCheckBox(this))
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    if (header) {
        hLayout->setContentsMargins(9, 7, 0, 0);
    } else {
        hLayout->setContentsMargins(1, 0, 0, 0);
    }
    hLayout->addWidget(mp_cb);
    this->setLayout(hLayout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(mp_cb, &DCheckBox::stateChanged, this, &DriverCheckItem::slotStateChanged);
}

void DriverCheckItem::setChecked(bool checked, bool dis)
{
    // 设置之前先断开
    if (dis)
        disconnect(mp_cb, &DCheckBox::stateChanged, this, &DriverCheckItem::slotStateChanged);

    mp_cb->setChecked(checked);

    // 设置之后重新链接
    if (dis)
        connect(mp_cb, &DCheckBox::stateChanged, this, &DriverCheckItem::slotStateChanged);
}

bool DriverCheckItem::checked()
{
    return mp_cb->isChecked();
}

bool DriverCheckItem::isEnabled()
{
    return mp_cb->isEnabled();
}

void DriverCheckItem::setCbEnable(bool e)
{
    mp_cb->setEnabled(e);
}

void DriverCheckItem::paintEvent(QPaintEvent *event)
{
    return DWidget::paintEvent(event);
}

void DriverCheckItem::slotStateChanged(int state)
{
    if (Qt::Unchecked == state)
        emit sigChecked(false);
    else
        emit sigChecked(true);
}

DriverNameItem::DriverNameItem(DWidget *parent, DriverType dt)
    : DWidget(parent)
    , mp_Icon(new DLabel(this))
    , mp_Type(new DLabel(this))
    , mp_Name(new DLabel(this))
    , m_Index(-1)
{
    mp_Type->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    mp_Type->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mp_Type->setElideMode(Qt::ElideRight);
    DFontSizeManager::instance()->bind(mp_Type, DFontSizeManager::T6);

    // 设置名称的相关参数：字体大小、颜色、对齐、布局
    mp_Name->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mp_Name->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mp_Name->setElideMode(Qt::ElideRight);
//    mp_Name->setStyleSheet("background:green;");
    DFontSizeManager::instance()->bind(mp_Name, DFontSizeManager::T8);
    DPalette pa = DApplicationHelper::instance()->palette(mp_Name);
    pa.setColor(DPalette::Text, pa.color(DPalette::TextTips));
    DApplicationHelper::instance()->setPalette(mp_Name, pa);

    // 切换主题
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [ = ] {
        DPalette plt = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette();
        plt.setColor(DPalette::Text, plt.color(DPalette::TextTips));
        mp_Name->setPalette(plt);
    });

    mp_Icon->setPixmap(QIcon(CommonTools::getDriverPixmap(dt)).pixmap(ICON_SIZE_WIDTH, ICON_SIZE_HEIGHT));
    QString ts = DApplication::translate("QObject", CommonTools::getDriverType(dt).toStdString().data());
    mp_Type->setText(ts);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(10, 0, 0, 0);
    hLayout->addWidget(mp_Icon);
    hLayout->addSpacing(20);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addSpacing(3);
    vLayout->setSpacing(0);
    vLayout->addWidget(mp_Type);
    vLayout->addWidget(mp_Name);
    hLayout->addLayout(vLayout);
//    hLayout->addSpacing(20);

    this->setLayout(hLayout);
}

void DriverNameItem::setName(const QString &name)
{
    mp_Name->setText(name);
    mp_Name->setToolTip(name);
}

void DriverNameItem::setIndex(int index)
{
    m_Index = index;
}

int DriverNameItem::index()
{
    return m_Index;
}


DriverLabelItem::DriverLabelItem(DWidget *parent,  const QString &txt)
    : DWidget(parent)
    , mp_Txt(new DLabel(this))
{
    mp_Txt->setText(txt);
    mp_Txt->setElideMode(Qt::ElideRight);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addStretch();
    vLayout->addWidget(mp_Txt);
    vLayout->addStretch();
    this->setLayout(vLayout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    DFontSizeManager::instance()->bind(mp_Txt, DFontSizeManager::T8);

    DPalette pa = DApplicationHelper::instance()->palette(mp_Txt);
    pa.setColor(DPalette::Text, pa.color(DPalette::TextTitle));
    DApplicationHelper::instance()->setPalette(mp_Txt, pa);

    // 切换主题
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [ = ] {
        DPalette plt = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette();
        plt.setColor(DPalette::Text, plt.color(DPalette::TextTitle));
        mp_Txt->setPalette(plt);
    });
}

DriverStatusItem::DriverStatusItem(DWidget *parent, Status s)
    : DWidget(parent)
    , mp_Spinner(new DSpinner(this))
    , mp_Icon(new DLabel(this))
    , mp_Status(new BtnLabel(this))
{
    mp_Spinner->setFixedSize(16, 16);
    mp_Spinner->start();
    mp_Status->setElideMode(Qt::ElideRight);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    this->setContentsMargins(0, 0, 0, 0);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(4);
    hLayout->addWidget(mp_Spinner);
    hLayout->addWidget(mp_Icon);
    hLayout->addWidget(mp_Status);
    hLayout->addStretch();
    this->setLayout(hLayout);

    DFontSizeManager::instance()->bind(mp_Status, DFontSizeManager::T8);
    DPalette pa = DApplicationHelper::instance()->applicationPalette();
    pa.setColor(DPalette::Text, pa.color(DPalette::TextTitle));
    DApplicationHelper::instance()->setPalette(mp_Status, pa);

    // 切换主题
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [ = ] {
        DPalette plt = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette();
        plt.setColor(DPalette::Text, plt.color(DPalette::TextTitle));
        mp_Status->setPalette(plt);
    });


    // 初始化图标状态和状态信息
    setStatus(s);
}

void DriverStatusItem::setStatus(Status st)
{
    showSpinner(ST_DOWNLOADING == st || ST_INSTALL == st);
    mp_Icon->setPixmap(QIcon(CommonTools::getStatusPixmap(st)).pixmap(16, 16));

    // bug132075 安装成功状态此button无法点击
    QString ts = DApplication::translate("QObject", CommonTools::getStausType(st).toStdString().data());

    if (ST_FAILED == st) {
        QString statusStr = QString("<a style=\"text-decoration:none\" href=\"failed\">") + ts + "</a>";
        mp_Status->setText(statusStr);
    } else {
        mp_Status->setText(ts);
    }

    m_Status = st;
}

Status DriverStatusItem::getStatus()
{
    return m_Status;
}

void DriverStatusItem::setErrorMsg(const QString &msg)
{
    mp_Status->setDesc(msg);
}

void DriverStatusItem::showSpinner(bool spin)
{
    mp_Icon->setVisible(!spin);
    mp_Spinner->setVisible(spin);
    spin ? mp_Spinner->start() : mp_Spinner->stop();
    mp_Status->setVisible(true);
}

DriverOperationItem::DriverOperationItem(DWidget *parent, bool install)
    : DWidget(parent)
    , mp_Btn(new DToolButton(this))
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->addWidget(mp_Btn);
    mp_Btn->setFixedSize(36, 36);
    mp_Btn->setIconSize(QSize(36, 36));
    m_IsInstall = install;
    setBtnIcon();
    this->setLayout(hLayout);
    // bug134379 驱动更新/安装按钮，悬浮框提示
    mp_Btn->setToolTip(install ? QObject::tr("Install") : QObject::tr("Update"));
    // 处理信号逻辑
    connect(mp_Btn, &DIconButton::clicked, this, &DriverOperationItem::clicked);

    // 切换主题
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [ = ] {
        setBtnIcon();
    });
}

void DriverOperationItem::setBtnEnable(bool enable)
{
    mp_Btn->setEnabled(enable);
}

void DriverOperationItem::setBtnIcon()
{
    DGuiApplicationHelper::ColorType theme = DGuiApplicationHelper::instance()->themeType();
    if (DGuiApplicationHelper::DarkType == theme) {
        if (m_IsInstall) {
            mp_Btn->setIcon(QIcon(":/icons/deepin/builtin/icons/install_dark.svg"));
        } else {
            mp_Btn->setIcon(QIcon(":/icons/deepin/builtin/icons/reinstall_dark.svg"));
        }
    } else {
        if (m_IsInstall) {
            mp_Btn->setIcon(QIcon(":/icons/deepin/builtin/icons/install.svg"));
        } else {
            mp_Btn->setIcon(QIcon(":/icons/deepin/builtin/icons/update-btn.svg"));
        }
    }
}

void DriverOperationItem::enterEvent(QEvent *event)
{
    return DWidget::enterEvent(event);
}

void DriverOperationItem::leaveEvent(QEvent *event)
{
    return DWidget::leaveEvent(event);
}
