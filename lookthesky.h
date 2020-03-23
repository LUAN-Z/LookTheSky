#ifndef LOOKTHESKY_H
#define LOOKTHESKY_H

#include <QComboBox>
#include <QDebug>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

QT_BEGIN_NAMESPACE
namespace Ui
{
class LookTheSky;
}
QT_END_NAMESPACE

class LookTheSky : public QMainWindow
{
    Q_OBJECT

public:
    LookTheSky(QWidget *parent = nullptr);
    ~LookTheSky();

    void setWindowProperty();

    QString words2Img(QString data);
    QString extractTemp(QString data);
    void setAQIColor(int value);
    bool loadCitycodeFile();
    void loadProvinceData();
    void loadUIEmements();
    void readSettings();
    void saveSettings();

#if 1
    void mousePressEvent(QMouseEvent *event);   //按下
    void mouseMoveEvent(QMouseEvent *event);    //移动
    void mouseReleaseEvent(QMouseEvent *event); //抬起
#endif

public slots:
    void readData(QNetworkReply *reply);

private:
    Ui::LookTheSky *ui;

    int selectedProvinceId;
    int lastProvinceId;
    int lastCityId;

    QString lastCityCode;
    QString selectedCityCode;

    QFile *jsonFile = new QFile(this);
    QJsonArray jsonArray;
    QNetworkAccessManager manager; // 管理http通信的对象

    QLabel *dateLabelList[8];
    QLabel *hTempLabelList[8];
    QLabel *lTempLabelList[8];
    QLabel *weatherImgLabelList[8];
    QLabel *weatherLabelList[8];
    QLabel *windLabelList[8];

    bool m_draging = false; // 是否拖动
    QPoint m_startPostion;  // 拖动前鼠标位置
    QPoint m_framPostion;   // 窗体的原始位置

protected:
    const QString citycodeFilePath = ":/assets/cityCode/citycode-2019-08-23.json";
    const QString settingsFilePath = "settings.json";
    const QString targetUrlPrefix = "http://t.weather.sojson.com/api/weather/city/";

private slots:
    void on_provinceBox_currentIndexChanged(int index);
    void on_cityBox_currentIndexChanged(int index);
    void on_updateBtn_clicked();
    void on_requsetBtn_clicked();
    void on_minimumBtn_clicked();
};
#endif // LOOKTHESKY_H
