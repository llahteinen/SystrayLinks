#ifndef APPPROXYSTYLE_HPP
#define APPPROXYSTYLE_HPP

#include <QProxyStyle>
#include <QStyledItemDelegate>
#include <QWidget>

inline constexpr int FLUENT_CONTROL_HEIGHT = 32;
/*
300 Light
350 Semilight
400 Regular
600 Semibold
700 Bold
https://learn.microsoft.com/en-us/windows/apps/design/signature-experiences/typography
*/
inline constexpr int FONT_WEIGHT_SEMIBOLD = 600;


class AppProxyStyle : public QProxyStyle
{
    Q_OBJECT

public:
    using QProxyStyle::QProxyStyle; /// base style: "windows11"

    int styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn *returnData) const override;

    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& contentSize, const QWidget* widget) const override;

    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* option, SubControl sc, const QWidget* widget) const override;

    QRect subElementRect(SubElement sr, const QStyleOption* option, const QWidget* widget) const override;

    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;

    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;

    void polish(QWidget* widget) override;
};

class IconDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};


#endif // APPPROXYSTYLE_HPP
