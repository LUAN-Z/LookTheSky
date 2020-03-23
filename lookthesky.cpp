#include "lookthesky.h"

#include "ui_lookthesky.h"

LookTheSky::LookTheSky(QWidget *parent) : QMainWindow(parent), ui(new Ui::LookTheSky)
{
    ui->setupUi(this);
    this->setWindowProperty();
    this->loadUIEmements();
    this->loadCitycodeFile();
    this->readSettings();
    connect(&manager, &QNetworkAccessManager::finished, this, &LookTheSky::readData);
    this->on_requsetBtn_clicked();
}

LookTheSky::~LookTheSky()
{
    delete ui;
}

void LookTheSky::setWindowProperty()
{
    // 设置图标
    this->setWindowIcon(QIcon(":/assets/icon/icon.png"));
    // 设置标题
    this->setWindowTitle("看, 天气");
    // 设置字体
    this->setFont(QFont(":/assets/fonts/SFD.ttf"));
    //窗口居中
    this->move((QApplication::desktop()->width() - this->width()) / 2,
               (QApplication::desktop()->height() - this->height()) / 2);
    // 隐藏标题栏
    this->setWindowFlag(Qt::FramelessWindowHint);
    // 设置背景透明
    // this->setAttribute(Qt::WA_TranslucentBackground);
    // 背景设置阴影
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setColor(Qt::darkGray);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setBlurRadius(30);
    ui->centralwidget->setGraphicsEffect(shadowEffect);
    //    ui->centralwidget->setWindowOpacity(0.2);
    ui->centralwidget->setAttribute(Qt::WA_TranslucentBackground);
}

QString LookTheSky::words2Img(QString data)
{
    QString imgUrl;
    if (data == "晴")
    {
        imgUrl = ":/assets/weather/qing.png";
    }
    else if (data == "阴")
    {
        imgUrl = ":/assets/weather/yin.png";
    }
    else if (data == "多云")
    {
        imgUrl = ":/assets/weather/duoyun.png";
    }
    else if (data == "小雨")
    {
        imgUrl = ":/assets/weather/xiaoyu.png";
    }
    else if (data == "中雨")
    {
        imgUrl = ":/assets/weather/zhongyu.png";
    }
    else if (data == "大雨")
    {
        imgUrl = ":/assets/weather/dayu.png";
    }
    else if (data == "阵雨")
    {
        imgUrl = ":/assets/weather/zhenyu.png";
    }
    else if (data == "雷阵雨")
    {
        imgUrl = ":/assets/weather/leizhenyu.png";
    }
    else if (data == "雨夹雪")
    {
        imgUrl = ":/assets/weather/yujiaxue.png";
    }
    else if (data == "小雪")
    {
        imgUrl = ":/assets/weather/xiaoxue.png";
    }
    else if (data == "中雪")
    {
        imgUrl = ":/assets/weather/zhongxue.png";
    }
    else if (data == "大雪")
    {
        imgUrl = ":/assets/weather/daxue.png";
    }
    else if (data == "阵雪")
    {
        imgUrl = ":/assets/weather/zhenxue.png";
    }
    else if (data == "雾")
    {
        imgUrl = ":/assets/weather/wu.png";
    }
    else
    {
        imgUrl = ":/assets/weather/wu.png";
    }

    return imgUrl;
}

QString LookTheSky::extractTemp(QString data)
{
    QString rtnString = data;
    rtnString.remove("高温 ").remove("低温 ").remove("℃");
    return rtnString;
}

void LookTheSky::setAQIColor(int value)
{
    QString color;
    if (value >= 0 && value <= 50)
    {
        color = "#009966";
    }
    else if (value >= 51 && value <= 100)
    {
        color = "#ffde33";
    }
    else if (value >= 101 && value <= 150)
    {
        color = "#ff9933";
    }
    else if (value >= 151 && value <= 200)
    {
        color = "#cc0033";
    }
    else if (value >= 201 && value <= 300)
    {
        color = "#660099";
    }
    else
    {
        color = "#7e0023";
    }
    QString styleSheet = QString("font:14pt \".PingFang SC0\";"
                                 "color: rgb(255, 255, 255);"
                                 "background-color: %1;")
                             .arg(color);
    ui->aqi->setNum(value);
    ui->aqi->setStyleSheet(styleSheet);
    ui->aqiWord->setStyleSheet(styleSheet);
}

bool LookTheSky::loadCitycodeFile()
{
    this->jsonFile->setFileName(this->citycodeFilePath);
    if (!this->jsonFile->open(QIODevice::ReadOnly))
    {
        qDebug() << "Can not open " << this->citycodeFilePath;
        return false;
    }
    if (!this->jsonFile->isReadable())
    {
        qDebug() << "This file is no allowed to read";
        this->jsonFile->close();
        return false;
    }
    else
    {
        this->loadProvinceData();
        return true;
    }
}

void LookTheSky::loadProvinceData()
{
    //    qDebug() << __FUNCTION__;
    QByteArray fileData = this->jsonFile->readAll();
    //    qDebug() << fileData.size();
    // 将字符串转成Json格式
    QJsonParseError error;
    QJsonDocument jsonData = QJsonDocument::fromJson(fileData, &error);
    // 如果error.error 不是NoError说明字符串不是JSON数据
    if (error.error != QJsonParseError::NoError) // 如果
    {
        qDebug() << "不是Json数据";
        return;
    }
    // 如果是Json数据，就开始解析 object方法就是返回JSOn的对象
    jsonArray = jsonData.array();
    int arraySize = jsonArray.size();
    QString province;
    int index;
    for (auto i = 0; i < arraySize; i++)
    {
        if (i < 34)
        {
            province = jsonArray.at(i).toObject().value("city_name").toString();
            index = jsonArray.at(i).toObject().value("id").toInt();
            ui->provinceBox->insertItem(index, province);
        }
        else
        {
            ui->provinceBox->setCurrentIndex(0);
            break;
        }
    }
}

void LookTheSky::loadUIEmements()
{
    this->dateLabelList[0] = ui->tdyDate;
    this->dateLabelList[1] = ui->d1Date;
    this->dateLabelList[2] = ui->d2Date;
    this->dateLabelList[3] = ui->d3Date;
    this->dateLabelList[4] = ui->d4Date;
    this->dateLabelList[5] = ui->d5Date;
    this->dateLabelList[6] = ui->d6Date;
    this->dateLabelList[7] = ui->d7Date;

    this->weatherLabelList[1] = ui->d1Weather;
    this->weatherLabelList[2] = ui->d2Weather;
    this->weatherLabelList[3] = ui->d3Weather;
    this->weatherLabelList[4] = ui->d4Weather;
    this->weatherLabelList[5] = ui->d5Weather;
    this->weatherLabelList[6] = ui->d6Weather;
    this->weatherLabelList[7] = ui->d7Weather;

    this->weatherImgLabelList[0] = ui->tdyWeatherImg;
    this->weatherImgLabelList[1] = ui->d1WeatherImg;
    this->weatherImgLabelList[2] = ui->d2WeatherImg;
    this->weatherImgLabelList[3] = ui->d3WeatherImg;
    this->weatherImgLabelList[4] = ui->d4WeatherImg;
    this->weatherImgLabelList[5] = ui->d5WeatherImg;
    this->weatherImgLabelList[6] = ui->d6WeatherImg;
    this->weatherImgLabelList[7] = ui->d7WeatherImg;

    this->hTempLabelList[0] = ui->tdyHTemp;
    this->hTempLabelList[1] = ui->d1HTemp;
    this->hTempLabelList[2] = ui->d2HTemp;
    this->hTempLabelList[3] = ui->d3HTemp;
    this->hTempLabelList[4] = ui->d4HTemp;
    this->hTempLabelList[5] = ui->d5HTemp;
    this->hTempLabelList[6] = ui->d6HTemp;
    this->hTempLabelList[7] = ui->d7HTemp;

    this->lTempLabelList[0] = ui->tdyLTemp;
    this->lTempLabelList[1] = ui->d1LTemp;
    this->lTempLabelList[2] = ui->d2LTemp;
    this->lTempLabelList[3] = ui->d3LTemp;
    this->lTempLabelList[4] = ui->d4LTemp;
    this->lTempLabelList[5] = ui->d5LTemp;
    this->lTempLabelList[6] = ui->d6LTemp;
    this->lTempLabelList[7] = ui->d7LTemp;

    this->windLabelList[1] = ui->d1Wind;
    this->windLabelList[2] = ui->d2Wind;
    this->windLabelList[3] = ui->d3Wind;
    this->windLabelList[4] = ui->d4Wind;
    this->windLabelList[5] = ui->d5Wind;
    this->windLabelList[6] = ui->d6Wind;
    this->windLabelList[7] = ui->d7Wind;
}

void LookTheSky::readSettings()
{
    QFile settingsFile;
    settingsFile.setFileName(this->settingsFilePath);
    if (!settingsFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Can not open settings.json";
        this->lastProvinceId = 0;
        this->lastCityId = 0;
        return;
    }
    QByteArray fileData = settingsFile.readAll();
    settingsFile.close();
    QJsonParseError error;
    QJsonDocument jsonData = QJsonDocument::fromJson(fileData, &error);
    if (error.error != QJsonParseError::NoError)
    {
        qDebug() << "不是Json数据";
        return;
    }

    QJsonObject obj = jsonData.object();

    this->lastProvinceId = obj.value("province").toInt();
    ui->provinceBox->setCurrentIndex(this->lastProvinceId);
    this->lastCityId = obj.value("city").toInt();
    this->lastCityCode = obj.value("city_code").toString();
    //    qDebug() << this->lastProvinceId << this->lastCityId << this->lastCityCode;
    //    this->on_provinceBox_currentIndexChanged(this->lastProvinceId);
    this->selectedCityCode = this->lastCityCode;
    ui->cityBox->setCurrentIndex(this->lastCityId);
}

void LookTheSky::saveSettings()
{
    QFile settingsFile("./settings.json");
    //    settingsFile.setFileName(this->citycodeFilePath);
    if (!settingsFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Can not open settings.json";
        return;
    }

    QJsonObject obj;
    obj.insert("province", this->lastProvinceId);
    obj.insert("city", this->lastCityId);
    obj.insert("city_code", this->lastCityCode);
    QJsonDocument doc;
    doc.setObject(obj);
    settingsFile.write(doc.toJson());
    settingsFile.close();
}

void LookTheSky::readData(QNetworkReply *reply)
{
    // reply 里面是一张图片的数据（因为我的网址就是图片地址）
    QByteArray array = reply->readAll();

    //    qDebug() << array;

    // 将字符串转成Json格式
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(array, &error);
    // 如果error.error 不是NoError说明字符串不是JSON数据
    if (error.error != QJsonParseError::NoError) // 如果
    {
        qDebug() << "不是Json数据";
        return;
    }

    // 如果是Json数据，就开始解析 object方法就是返回JSOn的对象
    QJsonObject obj = doc.object();

    QString tempData = obj.value("cityInfo").toObject().value("parent").toString() +
                       obj.value("cityInfo").toObject().value("city").toString();
    ui->cityLabel->setText(tempData);

    tempData = obj.value("cityInfo").toObject().value("updateTime").toString();
    ui->updateTime->setText("更新时间 " + tempData);

    tempData = obj.value("data").toObject().value("shidu").toString();
    ui->humidity->setText("湿度 " + tempData);

    double pm25 = obj.value("data").toObject().value("pm25").toDouble();
    ui->pm25->setNum(static_cast<int>(pm25));
    double pm10 = obj.value("data").toObject().value("pm10").toDouble();
    ui->pm10->setNum(static_cast<int>(pm10));

    QString quality = obj.value("data").toObject().value("quality").toString();
    ui->aqiWord->setText(quality);

    QString todayTemp = obj.value("data").toObject().value("wendu").toString();
    ui->tdyTemperature->setText(todayTemp);

    QJsonArray forecastArray = obj.value("data").toObject().value("forecast").toArray();
    QString ymd, high, low, type, week, sunrise, sunset, fx, fl;
    int aqi, count;
    for (int i = 0; i < forecastArray.count(); i++)
    {
        high = forecastArray.at(i).toObject().value("high").toString();
        low = forecastArray.at(i).toObject().value("low").toString();
        ymd = forecastArray.at(i).toObject().value("ymd").toString();
        week = forecastArray.at(i).toObject().value("week").toString();
        sunrise = forecastArray.at(i).toObject().value("sunrise").toString();
        sunset = forecastArray.at(i).toObject().value("sunset").toString();
        aqi = forecastArray.at(i).toObject().value("aqi").toInt();
        fx = forecastArray.at(i).toObject().value("fx").toString();
        fl = forecastArray.at(i).toObject().value("fl").toString();
        type = forecastArray.at(i).toObject().value("type").toString();
        if (i >= 0 && i <= 7)
        {
            count = i;
            if (count == 0)
            {
                this->dateLabelList[count]->setText(ymd);
                this->weatherImgLabelList[count]->setPixmap(QPixmap(words2Img(type)));
                this->hTempLabelList[count]->setText(extractTemp(high));
                this->lTempLabelList[count]->setText(extractTemp(low));
                ui->sunsetTime->setText(sunset);
                ui->sunriseTime->setText(sunrise);
                //                ui->aqi->setNum(aqi);
                this->setAQIColor(aqi);
            }
            else
            {
                this->dateLabelList[count]->setText(ymd);
                this->weatherLabelList[count]->setText(type);
                this->weatherImgLabelList[count]->setPixmap(QPixmap(words2Img(type)));
                this->hTempLabelList[count]->setText(extractTemp(high));
                this->lTempLabelList[count]->setText(extractTemp(low));
                this->windLabelList[count]->setText(fx + " " + fl);
            }
        }
    }
}

void LookTheSky::mousePressEvent(QMouseEvent *event)
{
    m_draging = true;
    if (event->buttons() & Qt::LeftButton) //只响应鼠标左键
    {
        m_startPostion = event->globalPos();
        m_framPostion = frameGeometry().topLeft();
    }
    QWidget::mousePressEvent(event); //调用父类函数保持原按键行为
}

void LookTheSky::mouseMoveEvent(QMouseEvent *event)
{
    if (event->globalX() >= this->x() + 200 && event->globalX() < this->x() + 820 &&
        event->globalY() >= this->y() && event->globalY() < this->y() + ui->cityBox->height())
    {
        return;
    }
    if (event->buttons() & Qt::LeftButton)
    {
        // offset 偏移位置
        QPoint offset = event->globalPos() - m_startPostion;
        move(m_framPostion + offset);
    }
    QWidget::mouseMoveEvent(event); //调用父类函数保持原按键行为
}

void LookTheSky::mouseReleaseEvent(QMouseEvent *event)
{
    m_draging = false;
    QWidget::mouseReleaseEvent(event);
}

void LookTheSky::on_provinceBox_currentIndexChanged(int index)
{
    this->lastProvinceId = index;
    // qDebug() << __FUNCTION__;
    // qDebug() << "Current index: " << index;
    index++;
    this->selectedProvinceId = index;
    QString city;
    int arraySize = jsonArray.size();
    int innerIndex = 0;
    int cityIndex = 0;
    ui->cityBox->clear();
    if (index == 33)
    {
        ui->cityBox->insertItem(0, "澳门");
        this->selectedCityCode = jsonArray.at(32).toObject().value("city_code").toString();
        ui->cityBox->setCurrentIndex(0);
        return;
    }
    for (auto i = 0; i < arraySize; i++)
    {
        cityIndex = jsonArray.at(i).toObject().value("pid").toInt();
        if (cityIndex == index)
        {
            ui->cityBox->insertItem(innerIndex++,
                                    jsonArray.at(i).toObject().value("city_name").toString());
        }
    }
    ui->cityBox->setCurrentIndex(0);
}

void LookTheSky::on_cityBox_currentIndexChanged(int index)
{
    this->lastCityId = index;
    //    qDebug() << __FUNCTION__;
    int arraySize = jsonArray.size();
    int innerIndex = 0;
    if (this->selectedProvinceId == 33)
    {
        return;
    }
    for (auto i = 0; i < arraySize; i++)
    {
        if (this->selectedProvinceId == jsonArray.at(i).toObject().value("pid").toInt())
        {
            if (index == innerIndex++)
            {
                this->selectedCityCode = jsonArray.at(i).toObject().value("city_code").toString();
                // qDebug() << this->selectedCityCode;
            }
        }
    }
    // qDebug() << this->selectedCityCode;
}

void LookTheSky::on_updateBtn_clicked()
{
    this->close();
}

void LookTheSky::on_requsetBtn_clicked()
{
    this->lastCityCode = this->selectedCityCode;
    this->saveSettings();
#if 1
    QString reqUrl = this->targetUrlPrefix + this->selectedCityCode;
    //获取界面上的网址
    QUrl url(reqUrl);

    //创建一个请求对象
    QNetworkRequest request(url);

    //通过通信管理对象，将请求的内容发送出去
    manager.get(request);
#endif
}

void LookTheSky::on_minimumBtn_clicked()
{
    this->showMinimized();
}
