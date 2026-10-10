#include "maindialog.hpp"
#include "ui_maindialog.h"
#include "styles.hpp"
#include "gui/appproxystyle.hpp"
#include <QCloseEvent>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMetaEnum>


MainDialog::MainDialog(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MainDialog)
    , m_settings{}
    , m_tray{} /// No parent for value member QObject
    , m_linksMenu{m_settings}
    , m_model{new QFileSystemModel()}
    , m_linksMenuIsOpen{false}
{
    qInfo().noquote() << QCoreApplication::applicationName() << QCoreApplication::applicationVersion() << "starting";

    m_settings.loadFromStore();

    ui->setupUi(this);

    setWindowFlag(Qt::WindowMinMaxButtonsHint, false);
    setWindowFlag(Qt::WindowMinimizeButtonHint, true);
    setWindowFlag(Qt::WindowFullscreenButtonHint, false);

    setWindowTitle(QString("%1 v%2").arg(QApplication::applicationDisplayName(), QCoreApplication::applicationVersion()));


    const auto icon_enum = QMetaEnum::fromType<Styles::Icons>();
    const auto icon_list = Styles::fluentIconsList();
    for(int i = int(Styles::Icons::Default) + 1; i < icon_list.count(); ++i) /// Don't add the default
    {
        const auto name = icon_enum.valueToKey(i);
        ui->icon_comboBox->addItem(icon_list.at(i), name);
    }
//    ui->icon_comboBox->setItemDelegate(new IconDelegate); /// Fix icons that change color when hovered BREAKS the visualization of selected item

    connectSettings();
    m_settings.emitAllSettingsChanged();
    connectUi();

    setFont(m_settings.getAppFont());
    qApp->setStyleSheet(Styles::styleSheetWindow());
    qApp->setStyle(new AppProxyStyle); /// Ownership of the style object is transferred to QApplication

    recalculateUiSizes();

    /// Set styleSheetLinksMenu only for links menu to not restyle this dialog's context menus
    m_linksMenu.setStyleSheet(Styles::styleSheetLinksMenu());

    setupTrayIcon();
    m_tray.trayIcon().show();

    connect(&m_linksMenu, &QMenu::aboutToHide, this, [this]() {
        qDebug() << "aboutToHide";
        /// Check if the hiding of the menu happened because of a click ON the tray icon.
        /// In that case we want the menu to close, and not to reopen when the mouse is realeased.
        /// For that we need to block the reopen
        if(m_tray.trayIcon().geometry().contains(QCursor::pos()))
        {
            /// Dismissed by clicking on the tray icon, menu should not be reopened when mouse releases.
            /// Tray::activated will fire now and will set m_linksMenuIsOpen to false.
            /// NOTE that there might be edge case if programmatically closing the menu while mouse happens to be on the icon.
            /// That happens also when double clicking, so we handle that as well
        }
        else
        {
            /// Dismissed somewhere else or spontaneously, next click on the tray icon should open the menu
            m_linksMenuIsOpen = false;
        }
    });
}

MainDialog::~MainDialog()
{
    m_settings.persistToStore();
    delete ui;
}

/// This only happens when the main window is closed, but not when the app is quit from the system tray
void MainDialog::closeEvent(QCloseEvent* event)
{
    qDebug() << "MainDialog::closeEvent";
    /// Keep app running, just close the dialog
    if(m_tray.trayIcon().isVisible())
    {
        hide();
        event->ignore();
    }
}

void MainDialog::connectSettings()
{
    connect(&m_settings, &Settings::linkPathChanged, &m_linksMenu, &LinksMenu::setRootPath);

    /// Connect settings to ui elements
    connect(&m_settings, &Settings::linkPathChanged, ui->linkPath_lineEdit, &QLineEdit::setText);

    connect(&m_settings, &Settings::itemLimitChanged, ui->itemLimit_spinBox, &QSpinBox::setValue);

    connect(&m_settings, &Settings::sortConditionChanged, ui->sortType_comboBox, &QComboBox::setCurrentIndex);
    connect(&m_settings, &Settings::sortConditionChanged, ui->sortDirection_comboBox, [this](int sort) {
        ui->sortDirection_comboBox->clear();
        QStringList items;
        switch(sort)
        {
        case 1: /// Size
            items = {"Smallest first", "Largest first"};
            break;
        case 3: /// Time
            items = {"Oldest first", "Newest first"};
            break;
        case 0: /// Name
        case 2: /// Type
        default:
            items = {"A to Z", "Z to A"};
            break;
        }
        ui->sortDirection_comboBox->addItems(items);
    });
    connect(&m_settings, &Settings::sortDirectionChanged, ui->sortDirection_comboBox, &QComboBox::setCurrentIndex);

    connect(&m_settings, &Settings::menuFontChanged, ui->fontFamily_lineEdit, [this](const QFont& font) {
        ui->fontFamily_lineEdit->setText(font.family());
    });
    connect(&m_settings, &Settings::menuFontChanged, ui->fontSizePx_spinBox, [this](const QFont& font) {
        /// QFont contains only either pt size or pixel size, but not both - the other is always -1
        ui->fontSizePx_spinBox->setValue(font.pixelSize());
    });

    connect(&m_settings, &Settings::iconChanged, ui->icon_comboBox, [this](QChar glyph) {
        const auto icon_index = Styles::fluentIconEnum(glyph);
        const auto combo_index = static_cast<int>(icon_index) - 1; /// ComboBox doesn't include the default
        ui->icon_comboBox->setCurrentIndex(combo_index);
    });
    connect(&m_settings, &Settings::iconChanged, this, [this](QChar glyph) {
        const auto icon_fb = QIcon::fromTheme("emblem-favorite"); /// Fallback from XDG icon set
        const auto icon = Styles::fluentIcon(glyph, icon_fb);
        m_tray.trayIcon().setIcon(icon);
        setWindowIcon(icon);
    });

}

void MainDialog::connectUi()
{
    connect(ui->close_pushButton, &QPushButton::clicked, this, &MainDialog::close);
    connect(ui->shutdown_pushButton, &QPushButton::clicked, this, &QApplication::exit);


    connect(ui->linkPath_lineEdit, &QLineEdit::editingFinished, &m_settings, [this]() {
        m_settings.setLinkPath(ui->linkPath_lineEdit->text());
    });
    connect(ui->linkPath_pushButton, &QPushButton::clicked, this, [this]() {
        const QString dir = QFileDialog::getExistingDirectory(
            this, tr("Select Directory"), m_settings.getLinkPath(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(!dir.isEmpty())
        {
            m_settings.setLinkPath(QDir::toNativeSeparators(dir));
        }
    });

    connect(ui->itemLimit_spinBox, &QSpinBox::valueChanged, &m_settings, &Settings::setItemLimit);

    connect(ui->sortType_comboBox, &QComboBox::currentIndexChanged, &m_settings, &Settings::setSortCondition);
    connect(ui->sortDirection_comboBox, &QComboBox::currentIndexChanged, &m_settings, [this](int index) {
        m_settings.setSortDirection(index == 0 ? Qt::SortOrder::AscendingOrder : Qt::SortOrder::DescendingOrder);
    });

    connect(ui->fontFamily_lineEdit, &QLineEdit::editingFinished, &m_settings, [this]() {
        auto font = m_settings.getMenuFont();
        font.setFamily(ui->fontFamily_lineEdit->text());
        m_settings.setMenuFont(font);
    });
    connect(ui->fontSizePx_spinBox, &QSpinBox::valueChanged, &m_settings, &Settings::setMenuFontPxSize);

    connect(ui->icon_comboBox, &QComboBox::currentIndexChanged, &m_settings, [this](int index) {
        const auto enum_val = static_cast<Styles::Icons>(index + 1); /// No default in the combobox
        const auto glyph = Styles::fluentIconGlyph(enum_val);
        m_settings.setIcon(glyph);
    });
}

/// This should be called after spinbox settings are set and combobox items are populated
/// and font sizes etc are set
void MainDialog::recalculateUiSizes()
{
    /// Set spinboxes to same width
    int size_hint = qMax(ui->itemLimit_spinBox->sizeHint().width(), ui->fontSizePx_spinBox->sizeHint().width());
    size_hint += 12; /// Add some margin
    ui->itemLimit_spinBox->setMinimumWidth(size_hint);
    ui->itemLimit_spinBox->setMaximumWidth(size_hint);
    ui->fontSizePx_spinBox->setMinimumWidth(size_hint);
    ui->fontSizePx_spinBox->setMaximumWidth(size_hint);

//    /// Set comboboxes to longest content width and same width
////    size_hint = qMax(ui->sortType_comboBox->sizeHint().width(), ui->sortDirection_comboBox->sizeHint().width());
//    size_hint = ui->sortType_comboBox->sizeHint().width();
//    ui->sortType_comboBox->setMinimumWidth(size_hint);
//    ui->sortType_comboBox->setMaximumWidth(size_hint);
////    size_hint = qMax(ui->sortType_comboBox->sizeHint().width(), ui->sortDirection_comboBox->sizeHint().width());
//    size_hint = ui->sortDirection_comboBox->sizeHint().width();
//    ui->sortDirection_comboBox->setMinimumWidth(size_hint);
//    ui->sortDirection_comboBox->setMaximumWidth(size_hint);
}

void MainDialog::closeLinksMenu(bool check)
{
    m_linksMenuIsOpen = false;
    if(!check || m_linksMenu.isVisible())
    {
        m_linksMenu.hide();
    }
}

void MainDialog::setupTrayIcon()
{
    static constexpr bool setting_menuAlignToCursor = false;
    static constexpr int setting_standardSpacing = 4;
    static constexpr int setting_menuIconGap = 3 * setting_standardSpacing; /// 12 px == same as Windows 11 tray overflow menu

    const auto alignMenuToIcon = [this](const QSize hint, bool /*mirror*/) -> QPoint {
        /// Anchor bottom-left of the menu at the cursor to open it upward
        /// like the native tray menu.
        /// Make the direction customizable!
        int offset_vertical = m_tray.trayIcon().geometry().top(); /// Align to top of tray icon
        offset_vertical -= setting_menuIconGap; /// Create gap
        offset_vertical -= hint.height(); /// Move up for the height of the menu itself

        int offset_horizontal = m_tray.trayIcon().geometry().center().x(); /// Align to center of tray icon
        offset_horizontal -= hint.width() / 2; /// Move left half for the width of the menu

        QPoint p(offset_horizontal, offset_vertical);
        /// Right-align if the menu would overflow the right screen edge ?
        /// If we align to center of the icon, it is not super mandatory though because Qt or Windows automatically keep the whole menu inside the screen area.
        /// But if there is option to align right
        const QRect avail = m_linksMenu.screen()->availableGeometry();
        if(p.x() + hint.width() > avail.right())
        {
            p = m_tray.trayIcon().geometry().topRight() - QPoint(hint.width(), hint.height()); /// Open the menu above the tray icon openig to the left
        }
        return p;
    };
    const auto alignMenuToCursor = [this](const QSize hint, bool /*mirror*/) -> QPoint {
        /// QMenu::popup sets the menu bottom left on the given coordinate == "opens to the right"
        const auto cursor = QCursor::pos();
        QPoint p = cursor - QPoint(0, hint.height()); /// Move up for the height of the menu itself

        /// Open to the left if the menu would overflow the right screen edge
        const QRect avail = m_linksMenu.screen()->availableGeometry();
        if(p.x() + hint.width() > avail.right())
        {
            p.setX(cursor.x() - hint.width()); /// Mirror the menu to the left from the cursor tip
        }
        return p;
    };

    connect(&m_tray, &Tray::activated, this, [=, this]() {
        qDebug() << "activated";

        /// Every other click should open the menu and every other close it.
        /// Close happens when clicking anywhere on the screen, including on the tray icon.
        /// Problem is that at least on Windows the mouse down event already closes the menu, so
        /// we can't track its isVisible flag in Tray::activated because it emits later.
        /// We use this flag to track if the menu should be open and therefore not triggered to open again
        if(m_linksMenuIsOpen)
        {
            /// We should rather close the menu on this activation instead of reopening it
            closeLinksMenu();
            return;
        }

        /// Else open the menu
        m_linksMenuIsOpen = true;

        m_linksMenu.updateMenu();

        const QSize hint = m_linksMenu.sizeHint();
        QPoint p;
        if(setting_menuAlignToCursor)
        {
            p = alignMenuToCursor(hint, false);
        }
        else
        {
            p = alignMenuToIcon(hint, false);
        }
        m_linksMenu.popup(p);
    });

    connect(&m_tray, &Tray::restoreTriggered, this, [this]() {
        qDebug() << "restoreTriggered";
        showNormal(); /// Window "restore" action
        activateWindow(); /// Make the dialog foreground if user had clicked something else

        closeLinksMenu();
    });

    connect(&m_tray, &Tray::contextActivated, this, [this]() {
        qDebug() << "contextActivated";
        closeLinksMenu();
    });

    connect(&m_tray, &Tray::quitTriggered, qApp, &QCoreApplication::quit);

}
