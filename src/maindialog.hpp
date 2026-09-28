#ifndef MAINDIALOG_HPP
#define MAINDIALOG_HPP

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
QT_END_NAMESPACE

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget* parent = nullptr);
    ~MainDialog() override;

private:
    Ui::MainDialog* ui;
};
#endif // MAINDIALOG_HPP
