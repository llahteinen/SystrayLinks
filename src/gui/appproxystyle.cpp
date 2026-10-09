#include "appproxystyle.hpp"

#include <QAbstractSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QAbstractItemView>
#include <QFont>
#include <QRect>
#include <QSize>
#include <QPainter>
#include <QStyleOptionButton>
#include <QStyleOptionComboBox>


int AppProxyStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const
{
//    if(hint == SH_ComboBox_Popup)
//    {
//        return 1;
//    }

    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

int AppProxyStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
//    switch(metric)
//    {
//        case PM_ToolTipLabelFrameWidth: /// Does not trigger
//            qDebug() << "PM_ToolTipLabelFrameWidth" << QProxyStyle::pixelMetric(metric, option, widget);
//            return 10;
//        case QStyle::PM_SpinBoxSliderHeight: /// Doesn't return anything
//            qDebug() << "slider" << QProxyStyle::pixelMetric(metric, option, widget);
//            return 6;
//        case QStyle::PM_SpinBoxFrameWidth: /// Adjusts both horizontal and vertical padding
//            qDebug() << "sp" << QProxyStyle::pixelMetric(metric, option, widget);
//            return 6; /// 4 seems closest we can get without some hacking, since we can't adjust horiz and vert separately. 8 already cuts text
//        case QStyle::PM_ComboBoxFrameWidth: /// Adjusts both horizontal and vertical padding
//            /// Qt's default seems 2 for the Windows 11 style
//            return 4; /// 4 seems closest we can get without some hacking, since we can't adjust horiz and vert separately. 8 already cuts text
//        case PM_SmallIconSize:
//            return 24;
//        case PM_MenuHMargin: return 0;
//        case PM_MenuVMargin: return 0;
//        case PM_MenuPanelWidth: return 4; /// margins above already provide it
//    default:
//        return QProxyStyle::pixelMetric(metric, option, widget);
//    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}

QSize AppProxyStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& contentSize, const QWidget* widget) const
{
    QSize sz = QProxyStyle::sizeFromContents(type, option, contentSize, widget);
    switch(type)
    {
    case CT_MenuItem: /// Note that links menu uses its own style, not this
        break;
    case CT_ComboBox: /// IS NOT CALLED if stylesheet is set?
    case CT_SpinBox: /// Setting height here does not increase spinbox button height, adjust that manually
        {
            /// For some reason it needs + 2 to achieve the wanted size
            sz.setHeight(qMax(sz.height(), FLUENT_CONTROL_HEIGHT + 2));
            break;
        }
    case CT_LineEdit:
        {
            const QWidget* parent = widget->parentWidget();
            bool is_embedded_widget = qobject_cast<const QAbstractSpinBox*>(parent) != nullptr ||
                                      qobject_cast<const QComboBox*>(parent) != nullptr;
            if(!is_embedded_widget)
            {
                sz.setHeight(qMax(sz.height(), FLUENT_CONTROL_HEIGHT + 2));
            }
            break;
        }
    case CT_PushButton:
        {
            sz.setHeight(qMax(sz.height(), FLUENT_CONTROL_HEIGHT + 2));
            sz.rwidth() += 8; /// 4 px more each side
            break;
        }
    default:
        break;
    }
    return sz;
}

QRect AppProxyStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex* option, SubControl sc, const QWidget* widget) const
{
    QRect r = QProxyStyle::subControlRect(cc, option, sc, widget);

    if(cc == QStyle::CC_ComboBox &&
        sc == QStyle::SC_ComboBoxEditField)
    {
        const auto* cb = qstyleoption_cast<const QStyleOptionComboBox*>(option);
        const auto indicatorWidth = pixelMetric(PM_MenuButtonIndicator, option, widget);

        /// For some reason the right side has very huge margin, even before the button too,
        /// so adjust the whole text to right reducing the right margin
        const int left = cb->rect.left() + 6;
        const int max_right = cb->rect.right() - indicatorWidth - 6;
        r.setLeft(left);
        r.setRight(max_right);
    }

    if(cc == QStyle::CC_SpinBox)
    {
        if(sc == QStyle::SC_SpinBoxEditField)
        {
            r.adjust(4, 0, 0, 0); /// Increase left margin
        }
        if(sc == QStyle::SC_SpinBoxUp || sc == QStyle::SC_SpinBoxDown)
        {
            r.setHeight(FLUENT_CONTROL_HEIGHT - 4);
            r.setTop(2);
        }
    }

    return r;
}

QRect AppProxyStyle::subElementRect(SubElement sr, const QStyleOption* option, const QWidget* widget) const
{
    /// Combobox contents / menu
    if(widget && qobject_cast<const QAbstractItemView*>(widget))
    {
        /// Style the icons, does not affect contents without icons
        if(sr == SE_ItemViewItemDecoration)
        {
            if(const auto* vi = qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                QStyleOptionViewItem new_option = *vi;
                /// For whatever reason the 16x16 icon is actually drawn smaller unless we set size to 18
                /// Same is true for 24 that becomes 22
                new_option.decorationSize.setWidth(18);
                new_option.decorationSize.setHeight(18);
                QRect r = QProxyStyle::subElementRect(sr, &new_option, widget);
                /// Add padding before the icon
                r.translate(4, 0);
                return r;
            }
        }
        /// Style the text, affects with or without icons
        else if(sr == SE_ItemViewItemText)
        {
            if(const auto* vi = qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                QRect r = QProxyStyle::subElementRect(sr, option, widget);
                /// Add more right spacing when no icon
                if(!vi->features.testFlag(QStyleOptionViewItem::HasDecoration))
                {
                    r.translate(4, 0);
                }
                else
                {
                    /// or more gap between the icon and text
                    r.translate(4 + 4, 0);
                }
                return r;
            }
        }
    }
    return QProxyStyle::subElementRect(sr, option, widget);
}

void AppProxyStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    if(element == CE_PushButtonLabel)
    {
        if(const auto* buttonOption = qstyleoption_cast<const QStyleOptionButton*>(option))
        {

            QStyleOptionButton opt(*buttonOption);
            /// Move text up 1px
            opt.rect.translate(0, -1);
            QProxyStyle::drawControl(element, &opt, painter, widget);
            return;
        }
    }
    /// Combobox itself
    if(element == CE_ComboBoxLabel)
    {
        if(const auto* cb = qstyleoption_cast<const QStyleOptionComboBox*>(option))
        {
            QStyleOptionComboBox new_option = *cb;
            /// The actual implementation is in QCommonStyle::drawControl and the gaps are hard-coded
            if(const auto box = qobject_cast<const QComboBox*>(widget))
            {
                if(!box->itemIcon(box->currentIndex()).isNull()) /// Or maybe we could use userData
                {
                    new_option.iconSize.rwidth() += 2 * 6; /// Add the margins to the icon width (seems bit of a hack)
                    new_option.rect.adjust(-6,0,0,0); /// Compensate right margin for the added icon width
                }
            }

            QProxyStyle::drawControl(element, &new_option, painter, widget);
            return;
        }
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}

void AppProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
//    qDebug() << "drawPrimitive" << element;
//    if(element == PE_IndicatorRadioButton)
//    {
//        qDebug() << "PE_IndicatorRadioButton"
//                 << option->rect;
//        /// Radio button size is unfortunately hard coded in QWindows11Style::drawPrimitive
//    }
//
//    if(element == PE_IndicatorItemViewItemCheck ||
//        element == PE_IndicatorMenuCheckMark)
//    {
//        qDebug() << "indicator:"
//                 << element
//                 << "rect:" << option->rect
//                 << "state:" << option->state
//                 << "widget:" << widget;
//    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void AppProxyStyle::polish(QWidget* widget)
{
    QProxyStyle::polish(widget);

    if(auto* line_edit = qobject_cast<QLineEdit*>(widget))
    {
        /// Don't apply normal line edit margins for Spinbox editor
        if(qobject_cast<QAbstractSpinBox*>(line_edit->parentWidget()))
        {
            line_edit->setTextMargins(4, 0, -4, 0); /// Add padding to left and move right edge closer to the buttons
            return;
        }

        /// Supposed to be 10px from the edge. We need to account for the padding of the box
        line_edit->setTextMargins(6, 0, 6, 0);
    }

    if(const auto role = widget->property("role");
        role.isValid() && role.toString() == "subtitle")
    {
        QFont font = widget->font();
        font.setVariableAxis("wght", FONT_WEIGHT_SEMIBOLD);
        widget->setFont(font);
    }
}

void IconDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    /// Temporarily clear the Selected state so the icon draws in Normal mode
    /// because selected mode inverts the color
    opt.state &= ~QStyle::State_Selected;
    QStyledItemDelegate::paint(painter, opt, index);
}
