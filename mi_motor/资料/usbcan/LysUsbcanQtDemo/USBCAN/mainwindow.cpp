/**
******************************************************************************
* @file    mainwindow.c
* @author  lys
* @version V1.1.0
* @date    2018
* @brief   USBCAN调试助手
* 使用设备USBCAN1做用例，其他设备类型参照该用例即可
* 默认波特:500K 支持数据接收和发送
*
* @note
*
* @endverbatim
*
******************************************************************************
* 版权:长沙六叶树教育科技有限公司
* 官网:www.liuyeshu.cn
* 更多资料教程下载见官网
******************************************************************************
修改日期    版本号   修改者   功能描述
2018.11.10 V1.1.0  LYS
----------------------------------------------------------------------------
******************************************************************************
*/
#include <stdio.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"


//仲裁域查找表
static const UINT kAbitTiming[] = {
    0x01400106,//1Mbps
    0x0100020A,//800kbps
    0x02C00106,//500kbps
    0x05C00106,//250kbps
    0x0BC00106,//125kbps
    0x0EC00106,//100kbps
    0x1DC00106 //50kbps
};
//数据域查找表
static const UINT kDbitTiming[] = {
    0x00800001,//5Mbps
    0x00800002,//4Mbps
    0x00800106,//2Mbps
    0x01400106,//1Mbps
    0x0100020A,//800kbps
    0x02C00106,//500kbps
    0x05C00106,//250kbps
    0x0BC00106,//125kbps
    0x0EC00106,//100kbps
    0x1DC00106 //50kbps
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //init
    windowInit();

    //更新界面
    windowUpdate();
}

MainWindow::~MainWindow()
{
    //exit
    windowExit();
    delete ui;
}


/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:16进制字符有效性检查
 */
int MainWindow::chartoint(unsigned char chr, unsigned char *cint)
{
    unsigned char cTmp;
    cTmp = chr - 48;
    if (cTmp <= 9)
    {
        *cint = cTmp;
        return 0;
    }
    cTmp = chr - 65;
    if (cTmp <= 5)
    {
        *cint = (cTmp + 10);
        return 0;
    }
    cTmp = chr - 97;
    if (cTmp <= 5)
    {
        *cint = (cTmp + 10);
        return 0;
    }
    return 1;
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:16进制字符串有效性检查
 */
int MainWindow::strtodata(unsigned char *str, unsigned char *data, int len, int flag)
{
    unsigned char cTmp = 0;
    int i = 0;
    for (int j = 0; j < len; j++)
    {
        if (chartoint(str[i++], &cTmp))
            return 1;
        data[j] = cTmp;
        if (chartoint(str[i++], &cTmp))
            return 1;
        data[j] = (data[j] << 4) + cTmp;
        if (flag == 1)
            i++;
    }
    return 0;
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:window初始化
 * 初始化相关组件
 */
void MainWindow::windowInit(void)
{
    //设备
    m_devType = LCAN_USBCAN2;
    m_devIndex = 0;
    m_devChIndex = 0;
    m_devConnect = false;
    workProIsCan = false;
    isFd = false;

    //logo
    setWindowIcon(QIcon(":/logo.png"));

    //标题设置
    setWindowTitle("六叶树USBCAN二次开发用例");

    //使用六叶树库美化界面
    windowStyleInit();

#ifndef __linux__
    controlCANLib = nullptr;
    controlCANLib = new QLibrary("ControlCAN.dll");
    controlCANLib->load();
#endif

    QStringList strList;

    //仲裁域波特率
    ui->comboBox_aBaud->clear();
    strList.clear();
    strList << "1000kbps 80%";
    strList << "800kbps 80%";
    strList << "500kbps 80%";
    strList << "250kbps 80%";
    strList << "125kbps 80%";
    strList << "100kbps 80%";
    strList << "50kbps 80%";
    strList << tr("自定义");
    ui->comboBox_aBaud->addItems(strList);
    //数据域波特率
    ui->comboBox_dBaud->clear();
    strList.clear();
    strList << "5Mbps 75%";
    strList << "4Mbps 80%";
    strList << "2Mbps 80%";
    strList << "1Mbps 80%";
    strList << "800kbps 80%";
    strList << "500kbps 80%";
    strList << "250kbps 80%";
    strList << "125kbps 80%";
    strList << "100kbps 80%";
    strList << tr("自定义");
    ui->comboBox_dBaud->addItems(strList);

    //UI
    QTableWidget *tableWidget = ui->tableWidget;

    //构造一个QTableWidget类的实体对象tableWidget
    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderLabels(QStringList() << tr("方向") << tr("ID") << tr("帧格式") << tr("帧类型") << tr("长度") << tr("数据(HEX)"));

    tableWidget->setColumnWidth(0, 70);
    tableWidget->setColumnWidth(1, 90);
    tableWidget->setColumnWidth(2, 70);
    tableWidget->setColumnWidth(3, 70);
    tableWidget->setColumnWidth(4, 80);

    tableWidget->horizontalHeader()->setStretchLastSection(true);

    //设备参数
    ui->comboBox_devIndex->setCurrentIndex(0);
    ui->comboBox_devIndex->setCurrentIndex(0);
    ui->comboBox_chIndex->setCurrentIndex(0);

    //信号连接
    connect(ui->comboBox_devType, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelChangeComboDevType(int)));
    connect(ui->comboBox_workPro, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelChangeComboWorkPro(int)));
    connect(ui->pushButton_devOpen, SIGNAL(clicked()), this, SLOT(OnButtonDevOpen()));
    connect(ui->pushButton_devClose, SIGNAL(clicked()), this, SLOT(OnButtonDevClose()));
    connect(ui->pushButton_send, SIGNAL(clicked()), this, SLOT(OnButtonDevSend()));
    connect(ui->pushButton_clr, SIGNAL(clicked()), this, SLOT(OnButtonClr()));
    connect(ui->pushButton_exit, SIGNAL(clicked()), this, SLOT(OnButtonExit()));
    connect(this, SIGNAL(showData(QString, QString, QString, QString, QString, QString)), this, SLOT(onShowData(QString, QString, QString, QString, QString, QString)));
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:window退出处理
 */
void MainWindow::windowExit(void)
{
    //设备关闭
    if(true == m_devConnect)
    {
        devConnectHandle();
    }

#ifndef __linux__
    //释放LIB
    if(nullptr != controlCANLib)
    {
        controlCANLib->unload();
        delete controlCANLib;
        controlCANLib = nullptr;
    }
#endif
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:window风格初始化
 */
void MainWindow::windowStyleInit(void)
{
    //模块初始化
    lysStyleInstance.sytleInit(this, ui->verticalLayout);
    //按钮初始化
    lysStyleInstance.pushButtonStyleInit(ui->pushButton_devOpen);
    lysStyleInstance.pushButtonStyleInit(ui->pushButton_devClose);
    lysStyleInstance.pushButtonStyleInit(ui->pushButton_send);
    lysStyleInstance.pushButtonStyleInit(ui->pushButton_clr);
    lysStyleInstance.pushButtonStyleInit(ui->pushButton_exit);
    //comboBox
    lysStyleInstance.comboBoxStyleInit(ui->comboBox_devType);
    lysStyleInstance.comboBoxStyleInit(ui->comboBox_devIndex);
    lysStyleInstance.comboBoxStyleInit(ui->comboBox_chIndex);
    lysStyleInstance.comboBoxStyleInit(ui->comboBox_fliterMode);
    lysStyleInstance.comboBoxStyleInit(ui->comboBox_frameFormat);
    lysStyleInstance.comboBoxStyleInit(ui->comboBox_frameType);
    lysStyleInstance.comboBoxStyleInit(ui->comboBox_sendMode);
    lysStyleInstance.comboBoxStyleInit(ui->comboBox_workMode);
    //lineEdit
    lysStyleInstance.lineEditStyleInit(ui->lineEdit_acc);
    lysStyleInstance.lineEditStyleInit(ui->lineEdit_btr0);
    lysStyleInstance.lineEditStyleInit(ui->lineEdit_btr1);
    lysStyleInstance.lineEditStyleInit(ui->lineEdit_mask);
    lysStyleInstance.lineEditStyleInit(ui->lineEdit_sendData);
    lysStyleInstance.lineEditStyleInit(ui->lineEdit_sendId);
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:window组件刷新
 */
void MainWindow::windowUpdate(void)
{
    if(true == m_devConnect)
    {
        ui->pushButton_devOpen->setEnabled(false);
        ui->pushButton_devClose->setEnabled(true);
        ui->pushButton_send->setEnabled(true);
    }
    else
    {
        ui->pushButton_devOpen->setEnabled(true);
        ui->pushButton_devClose->setEnabled(false);
        ui->pushButton_send->setEnabled(false);
    }

    if(isFd)
    {
        //CAN2.0相关
        ui->label_timing0->setVisible(false);
        ui->label_timing1->setVisible(false);
        ui->label_timingTip->setVisible(false);
        ui->label_fliterMode->setVisible(false);
        ui->label_accCode->setVisible(false);
        ui->label_maskCode->setVisible(false);
        ui->lineEdit_btr0->setVisible(false);
        ui->lineEdit_btr1->setVisible(false);
        ui->lineEdit_acc->setVisible(false);
        ui->lineEdit_mask->setVisible(false);
        ui->comboBox_fliterMode->setVisible(false);

        //FD相关组件
        ui->label_aBaud->setVisible(true);
        ui->label_dBaud->setVisible(true);
        ui->label_workPro->setVisible(true);
        ui->comboBox_aBaud->setVisible(true);
        if(0 == ui->comboBox_workPro->currentIndex())
        {
            //FD模式
            ui->label_iso->setVisible(true);
            ui->label_dBaud->setVisible(true);
            ui->label_framePro->setVisible(true);
            ui->comboBox_dBaud->setVisible(true);
            ui->comboBox_iso->setVisible(true);
            ui->comboBox_framePro->setEnabled(true);
            ui->comboBox_framePro->setVisible(true);
            //
            ui->comboBox_frameFormat->setEnabled(false);
            ui->comboBox_frameFormat->setCurrentIndex(0);
        }
        else
        {
            //CAN模式
            ui->label_iso->setVisible(false);
            ui->label_dBaud->setVisible(false);
            ui->label_framePro->setVisible(false);
            ui->comboBox_dBaud->setVisible(false);
            ui->comboBox_iso->setVisible(false);
            ui->comboBox_framePro->setVisible(false);
            ui->comboBox_frameFormat->setEnabled(true);
        }
        ui->comboBox_workPro->setVisible(true);
    }
    else
    {
        //CAN2.0相关
        ui->label_timing0->setVisible(true);
        ui->label_timing1->setVisible(true);
        ui->label_timingTip->setVisible(true);
        ui->label_fliterMode->setVisible(true);
        ui->label_accCode->setVisible(true);
        ui->label_maskCode->setVisible(true);
        ui->lineEdit_btr0->setVisible(true);
        ui->lineEdit_btr1->setVisible(true);
        ui->lineEdit_acc->setVisible(true);
        ui->lineEdit_mask->setVisible(true);
        ui->comboBox_fliterMode->setVisible(true);
        //FD相关组件
        ui->label_aBaud->setVisible(false);
        ui->label_dBaud->setVisible(false);
        ui->label_iso->setVisible(false);
        ui->label_workPro->setVisible(false);
        ui->label_framePro->setVisible(false);
        ui->comboBox_aBaud->setVisible(false);
        ui->comboBox_dBaud->setVisible(false);
        ui->comboBox_iso->setVisible(false);
        ui->comboBox_workPro->setVisible(false);
        ui->comboBox_framePro->setVisible(false);
    }

    //设备已连接
    if(m_devConnect)
    {
        //CAN相关配置,不能修改
        ui->lineEdit_btr0->setEnabled(false);
        ui->lineEdit_btr1->setEnabled(false);
        ui->lineEdit_acc->setEnabled(false);
        ui->lineEdit_mask->setEnabled(false);
        ui->comboBox_fliterMode->setEnabled(false);

        //CANFD相关配置,不能修改
        ui->comboBox_aBaud->setEnabled(false);
        ui->comboBox_dBaud->setEnabled(false);
        ui->comboBox_iso->setEnabled(false);
        ui->comboBox_workPro->setEnabled(false);

        //工作模式 设备类型
        ui->comboBox_workMode->setEnabled(false);
        ui->comboBox_devType->setEnabled(false);
        ui->comboBox_devIndex->setEnabled(false);
        ui->comboBox_chIndex->setEnabled(false);
    }
    else
    {
        //CAN
        ui->lineEdit_btr0->setEnabled(true);
        ui->lineEdit_btr1->setEnabled(true);
        ui->lineEdit_acc->setEnabled(true);
        ui->lineEdit_mask->setEnabled(true);
        ui->comboBox_fliterMode->setEnabled(true);
        //CANFD
        ui->comboBox_aBaud->setEnabled(true);
        ui->comboBox_dBaud->setEnabled(true);
        ui->comboBox_iso->setEnabled(true);
        ui->comboBox_workPro->setEnabled(true);
        //工作模式 设备类型
        ui->comboBox_workMode->setEnabled(true);
        ui->comboBox_devType->setEnabled(true);
        ui->comboBox_devIndex->setEnabled(true);
        ui->comboBox_chIndex->setEnabled(true);
    }
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:window组件刷新
 */
void MainWindow::windowDevInitCfgGetHandle(void)
{
    //获取变量值
    m_devIndex = ui->comboBox_devIndex->currentIndex();
    m_devChIndex = ui->comboBox_chIndex->currentIndex();

    m_devFliterMode = ui->comboBox_fliterMode->currentIndex();
    m_devWorkMode = ui->comboBox_workMode->currentIndex();


    if(isFd)
    {
        //FD
        memset(&canInitFDCfgInfo, 0, sizeof(LCAN_INITFD_CONFIG));
        if(ui->comboBox_iso->currentIndex())
        {
            canInitFDCfgInfo.isoEn = 1;
        }
        else
        {
            canInitFDCfgInfo.isoEn = 0;
        }
        if(workProIsCan)
        {
            //工作于CAN模式
            canInitFDCfgInfo.abit_timing = kAbitTiming[ui->comboBox_aBaud->currentIndex()];
            canInitFDCfgInfo.dbit_timing = canInitFDCfgInfo.abit_timing;
            canInitFDCfgInfo.fdEn = 0;
        }
        else
        {
            //工作于FD模式
            canInitFDCfgInfo.abit_timing = kAbitTiming[ui->comboBox_aBaud->currentIndex()];
            canInitFDCfgInfo.dbit_timing = kDbitTiming[ui->comboBox_dBaud->currentIndex()];
            canInitFDCfgInfo.fdEn = 1;
        }
    }
    else
    {
        //CAN
        memset(&canInitCfgInfo, 0, sizeof(LCAN_INIT_CONFIG));
        QString strAcc = ui->lineEdit_acc->text();
        QString strMask = ui->lineEdit_mask->text();
        QString strBtr0 = ui->lineEdit_btr0->text();
        QString strBtr1 = ui->lineEdit_btr1->text();

        //init_config
        char szAcc[20], szMask[20], szBtr0[20], szBtr1[20];
        unsigned char sztmp[4];
        UINT Acc, Mask, Timing0, Timing1;

        sprintf(szAcc, "%s", strAcc.toLocal8Bit().constData());
        sprintf(szMask, "%s", strMask.toLocal8Bit().constData());
        sprintf(szBtr0, "%s", strBtr0.toLocal8Bit().constData());
        sprintf(szBtr1, "%s", strBtr1.toLocal8Bit().constData());

        //过滤码
        if (strtodata((unsigned char*)szAcc, sztmp, 4, 0) != 0)
        {
            QMessageBox::information(this, "ACC数据格式不对!","警告",QMessageBox::Ok);
            return;
        }
        Acc = strAcc.toUInt(nullptr,16);
        //屏蔽码
        if (strtodata((unsigned char*)szMask, sztmp, 4, 0) != 0)
        {
            QMessageBox::information(this, "MASK数据格式不对!","警告",QMessageBox::Ok);
            return;
        }
        Mask = strMask.toUInt(nullptr, 16);

        //定时器0
        if (strtodata((unsigned char*)szBtr0, sztmp, 1, 0) != 0)
        {
            QMessageBox::information(this, "定时器0数据格式不对!","警告",QMessageBox::Ok);
            return;
        }
        Timing0 = ((DWORD)sztmp[0]);

        //定时器1
        if (strtodata((unsigned char*)szBtr1, sztmp, 1, 0) != 0)
        {
            QMessageBox::information(this, "定时器1数据格式不对!","警告",QMessageBox::Ok);
            return;
        }
        Timing1 = ((DWORD)sztmp[0]);

        canInitCfgInfo.accCode = Acc;
        canInitCfgInfo.accMask = Mask;
        canInitCfgInfo.timing0 = Timing0;
        canInitCfgInfo.timing1 = Timing1;
        canInitCfgInfo.filter = m_devFliterMode;
        canInitCfgInfo.mode = m_devWorkMode;
    }
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:数据显示
 */
void MainWindow::showDataTrig(QString strDir, QString strID, QString strFormat, QString strType, QString strLen, QString strData)
{
    emit showData(strDir, strID, strFormat, strType, strLen, strData);
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:设备连接处理
 */
void MainWindow::devConnectHandle(void)
{
#ifndef __linux__
    LCAN_OpenDevice devOpenFun = (LCAN_OpenDevice)controlCANLib->resolve("LCAN_OpenDevice");
    LCAN_CloseDevice devCloseFun = (LCAN_CloseDevice)controlCANLib->resolve("LCAN_CloseDevice");
    LCAN_InitCAN devInitFun = (LCAN_InitCAN)controlCANLib->resolve("LCAN_InitCAN");
    LCAN_InitCANFD devInitFDFun = (LCAN_InitCANFD)controlCANLib->resolve("LCAN_InitCANFD");
#endif

    if (false == m_devConnect)
    {
        //设备还未连接,开始连接
        windowDevInitCfgGetHandle();
#ifdef __linux__
        if (LCAN_OpenDevice(m_devType, m_devIndex) != LCAN_STATUS_OK)
#else
        if (devOpenFun(m_devType, m_devIndex) != LCAN_STATUS_OK)
#endif
        {
            QMessageBox::information(this, "提示", "打开设备失败", QMessageBox::Ok);
            return;
        }
        if(isFd)
        {
        //FD设备
        #ifdef __linux__
            if(0 >= LCAN_InitCANFD(m_devType, m_devIndex, m_devChIndex, &canInitFDCfgInfo))
        #else
            if(0 >= devInitFDFun(m_devType, m_devIndex, m_devChIndex, &canInitFDCfgInfo))
        #endif
            {
                //初始化失败
                QMessageBox::information(this, "提示", "初始化设备失败", QMessageBox::Ok);
        #ifdef __linux__
                LCAN_CloseDevice(m_devType, m_devIndex);
        #else
                devCloseFun(m_devType, m_devIndex);
        #endif
                return;
            }
        }
        else
        {
        //CAN2.0设备
        #ifdef __linux__
            if(0 >= LCAN_InitCAN(m_devType, m_devIndex, m_devChIndex, &canInitCfgInfo))
        #else
            if(0 >= devInitFun(m_devType, m_devIndex, m_devChIndex, &canInitCfgInfo))
        #endif
            {
                //初始化失败
                QMessageBox::information(this, "提示", "初始化设备失败", QMessageBox::Ok);
        #ifdef __linux__
                LCAN_CloseDevice(m_devType, m_devIndex);
        #else
                devCloseFun(m_devType, m_devIndex);
        #endif
                return;
            }
        }
        //
        m_devConnect = true;
        //启动线程
        userThread.threadInit(this);
        userThread.threadStart();
    }
    else
    {
        m_devConnect = false;
        //停止线程
        userThread.threadStop();
#ifdef __linux__
        LCAN_CloseDevice(m_devType, m_devIndex);
#else
        devCloseFun(m_devType, m_devIndex);
#endif
    }

}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:设备CAN发送处理
 */
void MainWindow::devCanSendHandle(void)
{
    LCAN_CAN_OBJ frameinfo;
    memset((void*)&frameinfo, 0, sizeof(frameinfo));

    if (ui->lineEdit_sendId->text().length() == 0)
    {
        QMessageBox::information(this, "错误", "ID不能为空", QMessageBox::Ok);
        return;
    }

    if (ui->lineEdit_sendData->text().length() == 0)
    {
        QMessageBox::information(this, "错误", "发送数据不能为空", QMessageBox::Ok);
        return;
    }

    //ID
    QString strId = ui->lineEdit_sendId->text();
    strId.remove(QRegExp("\\s"));
    if (strId.length()>8)
    {
        QMessageBox::information(this, "错误", "ID超过范围", QMessageBox::Ok);
        return;
    }

    //DATA
    QString strData = ui->lineEdit_sendData->text();
    strData.remove(QRegExp("\\s"));
    QByteArray  sData = QByteArray::fromHex(strData.toLocal8Bit());

    for (int i = 0; i < sData.size(); ++i)
    {
        frameinfo.data[i] = sData.at(i);
    }

    if (sData.size() > 8)
    {
        QMessageBox::information(this, "错误",
                                 "数据长度超过范围,最大为8个字节", QMessageBox::Ok);
        return;
    }

    frameinfo.dataLen = sData.size();
    frameinfo.remoteFlag = ui->comboBox_frameFormat->currentIndex();
    frameinfo.externFlag = ui->comboBox_frameType->currentIndex();
    frameinfo.id = strId.toInt(nullptr, 16);
    if(frameinfo.externFlag)
    {
        frameinfo.id &= 0x1FFFFFFF;
    }
    else
    {
        frameinfo.id &= 0x000007FF;
    }
    strId = QString("%1").arg(frameinfo.id,0,16);

    frameinfo.sendType = ui->comboBox_sendMode->currentIndex();
    //远程帧
    if(frameinfo.remoteFlag)
    {
        //数据必须为空
        frameinfo.dataLen = 0;
    }

#ifdef __linux__
    int ret = LCAN_Transmit(m_devType, m_devIndex, m_devChIndex, &frameinfo, 1);
#else
    LCAN_Transmit devTransmit = (LCAN_Transmit)controlCANLib->resolve("LCAN_Transmit");
    int ret = devTransmit(m_devType, m_devIndex, m_devChIndex, &frameinfo, 1);
#endif
    if (0 >= ret)
    {
        //发送失败
        QMessageBox::information(this, "提示", "发送失败", QMessageBox::Ok);
    }
    else
    {
        //发送成功
        QString tmpstr;
        strData = "";
        for (int j = 0; j < frameinfo.dataLen; j++)
        {
            tmpstr.sprintf("%02x ", frameinfo.data[j]);
            strData.append(QString().sprintf("%02x", frameinfo.data[j]));
            strData.append(" ");
        }

        emit showData("发送", "0X"+ strId,
                      (0 == frameinfo.remoteFlag)?"数据帧":"远程帧",
                      (0 == frameinfo.externFlag)?"标准帧":"扩展帧",
                      QString::number(frameinfo.dataLen), strData);
    }
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:设备CANFD发送处理
 */
void MainWindow::devCanFDSendHandle(void)
{
    LCAN_CANFD_OBJ frameinfo;
    memset((void*)&frameinfo, 0, sizeof(LCAN_CANFD_OBJ));

    if (ui->lineEdit_sendId->text().length() == 0)
    {
        QMessageBox::information(this, "错误", "ID不能为空", QMessageBox::Ok);
        return;
    }

    if (ui->lineEdit_sendData->text().length() == 0)
    {
        QMessageBox::information(this, "错误", "发送数据不能为空", QMessageBox::Ok);
        return;
    }

    //ID
    QString strId = ui->lineEdit_sendId->text();
    strId.remove(QRegExp("\\s"));
    if (strId.length()>8)
    {
        QMessageBox::information(this, "错误", "ID超过范围", QMessageBox::Ok);
        return;
    }

    //DATA
    QString strData = ui->lineEdit_sendData->text();
    strData.remove(QRegExp("\\s"));
    QByteArray  sData = QByteArray::fromHex(strData.toLocal8Bit());

    for (int i = 0; i < sData.size(); ++i)
    {
        frameinfo.data[i] = sData.at(i);
    }

    if (sData.size() > 64)
    {
        QMessageBox::information(this, "错误",
                                 "数据长度超过范围,最大为64个字节", QMessageBox::Ok);
        return;
    }

    frameinfo.dataLen = sData.size();
    frameinfo.remoteFlag = 0;
    frameinfo.externFlag = ui->comboBox_frameType->currentIndex();
    frameinfo.id = strId.toInt(nullptr, 16);
    frameinfo.sendType = ui->comboBox_sendMode->currentIndex();
    if(frameinfo.externFlag)
    {
        frameinfo.id &= 0x1FFFFFFF;
    }
    else
    {
        frameinfo.id &= 0x000007FF;
    }
    strId = QString("%1").arg(frameinfo.id,0,16);

    //FD设备的BRS
    frameinfo.flag = 0;
    switch(ui->comboBox_framePro->currentIndex())
    {
        case 2:
        //fd brs
        frameinfo.flag |= LCAN_CANFD_BRS;
        break;
        default:
        break;
    }

#ifdef __linux__
    int ret = LCAN_TransmitFD(m_devType, m_devIndex, m_devChIndex, &frameinfo, 1);
#else
    LCAN_TransmitFD devTransmit = (LCAN_TransmitFD)controlCANLib->resolve("LCAN_TransmitFD");
    int ret = devTransmit(m_devType, m_devIndex, m_devChIndex, &frameinfo, 1);
#endif
    if (0 >= ret)
    {
        //发送失败
        QMessageBox::information(this, "提示", "发送失败", QMessageBox::Ok);
    }
    else
    {
        //发送成功
        QString tmpstr;
        strData = "";
        for (int j = 0; j < frameinfo.dataLen; j++)
        {
            tmpstr.sprintf("%02x ", frameinfo.data[j]);
            strData.append(QString().sprintf("%02x", frameinfo.data[j]));
            strData.append(" ");
        }

        emit showData("发送", "0X"+ strId,
                      (0 == frameinfo.remoteFlag)?"数据帧":"远程帧",
                      (0 == frameinfo.externFlag)?"标准帧":"扩展帧",
                      QString::number(frameinfo.dataLen), strData);
    }
}


/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:设备类型选择处理
 */
void MainWindow::OnSelChangeComboDevType(int nIndex)
{
    //设备类型
    switch(nIndex)
    {
        case 0:
            //USBCAN1
            isFd = false;
            m_devType = LCAN_USBCAN1;
            break;
        case 1:
            //USBCAN2
            isFd = false;
            m_devType = LCAN_USBCAN2;
            break;
        case 2:
            //USBCANFDMini
            isFd = true;
            m_devType = LCAN_USBCANFDMini;
            break;
        case 3:
            //USBCANFD1
            isFd = true;
            m_devType = LCAN_USBCANFD1;
            break;
        case 4:
            //USBCANFD2
            isFd = true;
            m_devType = LCAN_USBCANFD2;
            break;
        default:
            isFd = false;
            break;
    }

    //refresh
    windowUpdate();
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:设备工作协议选择处理
 */
void MainWindow::OnSelChangeComboWorkPro(int nIndex)
{
    if(0 == nIndex)
    {
        workProIsCan = false;
    }
    else
    {
        workProIsCan = true;
    }

    //refresh
    windowUpdate();
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:设备打开处理
 */
void MainWindow::OnButtonDevOpen()
{
    //设备连接处理
    devConnectHandle();
    //更新界面
    windowUpdate();
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:设备关闭处理
 */
void MainWindow::OnButtonDevClose()
{
    //设备关闭处理
    devConnectHandle();
    //更新界面
    windowUpdate();
}


/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:设备发送处理
 */
void MainWindow::OnButtonDevSend()
{
    if (false == m_devConnect)
    {
        //设备还未连接
        QMessageBox::information(this, "提示", "先打开设备!!!", QMessageBox::Ok);
        return;
    }

    if((true == isFd)&&(false == workProIsCan)&&
            (0 != ui->comboBox_framePro->currentIndex()))
    {
        //FD模式
        devCanFDSendHandle();
    }
    else
    {
        //CAN2.0模式
        devCanSendHandle();
    }
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:清空显示
 */
void MainWindow::OnButtonClr()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}


/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:退出
 */
void MainWindow::OnButtonExit()
{
    //exit
    this->close();
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:数据显示
 */
void MainWindow::onShowData( QString strDir, QString strCANID, QString strFrameFormat, QString strFrameType, QString dataLen, QString strData)
{
    int nCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(nCount);

    QTableWidgetItem  *pItem = new QTableWidgetItem;
    pItem->setText(strDir);
    ui->tableWidget->setItem(nCount, 0, pItem);

    pItem = new QTableWidgetItem;
    pItem->setText(strCANID);
    ui->tableWidget->setItem(nCount, 1, pItem);

    pItem = new QTableWidgetItem;
    pItem->setText(strFrameFormat);
    ui->tableWidget->setItem(nCount, 2, pItem);

    pItem = new QTableWidgetItem;
    pItem->setText(strFrameType);
    ui->tableWidget->setItem(nCount, 3, pItem);

    pItem = new QTableWidgetItem;
    pItem->setText(dataLen);
    ui->tableWidget->setItem(nCount, 4, pItem);

    pItem = new QTableWidgetItem;
    pItem->setText(strData);
    ui->tableWidget->setItem(nCount, 5, pItem);

    ui->tableWidget->setRowHeight(nCount,23);
}


/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:用户线程类 构造函数
 */
QUserThread::QUserThread()
{
    m_exit = false;
    mainWindowPtr = nullptr;
}


/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:用户线程类 析构函数
 */
QUserThread::~QUserThread()
{

}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:用户线程类 任务
 */
void QUserThread::run()
{
    LCAN_CAN_OBJ frameinfo[32];
    LCAN_CANFD_OBJ frameFDinfo[32];
    UINT len,cnt = 0;

#ifdef __linux__

#else
    LCAN_GetReceiveNum devReceiveNum = (LCAN_GetReceiveNum)mainWindowPtr->controlCANLib->resolve("LCAN_GetReceiveNum");
    LCAN_Receive devReceive = (LCAN_Receive)mainWindowPtr->controlCANLib->resolve("LCAN_Receive");
    LCAN_ReceiveFD devReceiveFD = (LCAN_ReceiveFD)mainWindowPtr->controlCANLib->resolve("LCAN_ReceiveFD");
#endif

    while (true)
    {
        if (m_exit)
            break;
        //restart cal
        cnt = 0;
        /////////////////////////////////////CAN接收/////////////////////////////////////////
        #ifdef __linux__
        if(0 < LCAN_GetReceiveNum(mainWindowPtr->m_devType, mainWindowPtr->m_devIndex, mainWindowPtr->m_devChIndex, 0))
        #else
        if(0 < devReceiveNum(mainWindowPtr->m_devType, mainWindowPtr->m_devIndex, mainWindowPtr->m_devChIndex, 0))
        #endif
        {
            #ifdef __linux__
            len = LCAN_Receive(mainWindowPtr->m_devType, mainWindowPtr->m_devIndex, mainWindowPtr->m_devChIndex, frameinfo, 32, 0);
            #else
            len = devReceive(mainWindowPtr->m_devType, mainWindowPtr->m_devIndex, mainWindowPtr->m_devChIndex, frameinfo, 32, 0);
            #endif
            if(0 < len)
            {
                cnt += len;
                canRecAnalysis(frameinfo, len);
            }
        }
        /////////////////////////////////////FD接收/////////////////////////////////////////
        if((true == mainWindowPtr->isFd) && (false == mainWindowPtr->workProIsCan))
        {
            //FD设备处于FD模式
            #ifdef __linux__
            if(0 < LCAN_GetReceiveNum(mainWindowPtr->m_devType, mainWindowPtr->m_devIndex, mainWindowPtr->m_devChIndex, 1))
            #else
            if(0 < devReceiveNum(mainWindowPtr->m_devType, mainWindowPtr->m_devIndex, mainWindowPtr->m_devChIndex, 1))
            #endif
            {
                #ifdef __linux__
                len = LCAN_ReceiveFD(mainWindowPtr->m_devType, mainWindowPtr->m_devIndex, mainWindowPtr->m_devChIndex, frameFDinfo, 32, 0);
                #else
                len = devReceiveFD(mainWindowPtr->m_devType, mainWindowPtr->m_devIndex, mainWindowPtr->m_devChIndex, frameFDinfo, 32, 0);
                #endif
                if(0 < len)
                {
                    cnt += len;
                    canFDRecAnalysis(frameFDinfo, len);
                }
            }
        }

        if (0 == cnt)
        {
            QThread::usleep(200);
            continue;
        }
    }

    m_exit = false;
}

/**
 * @输入参数: 设备类型 设备索引 通道索引
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:用户线程类 初始化
 */
void QUserThread::threadInit(MainWindow *ptr)
{
    m_exit = false;
    mainWindowPtr = ptr;
}


/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:启动线程
 */
void QUserThread::threadStart(void)
{
    m_exit = false;
    start();
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:停止线程
 */
void QUserThread::threadStop(void)
{
    m_exit = true;
    QThread::usleep(200);

    if(isRunning())
    {
        quit();
    }
}

/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:CAN帧解析
 */
void QUserThread::canRecAnalysis(LCAN_CAN_OBJ *objPtr, UINT len)
{
    UINT i = 0;
    QString tmpstr;
    QString strCANID;
    QString strFormat;
    QString strType;
    QString strLen;
    QString strData;

    for (i = 0; i < len; i++)
    {
        strCANID.clear();
        strFormat.clear();
        strData.clear();
        strLen.clear();
        strType.clear();

        strCANID.sprintf("0X%08x ", objPtr[i].id);

        if (objPtr[i].remoteFlag == 0)
            strFormat = "数据帧 ";
        else
            strFormat = "远程帧 ";

        if (objPtr[i].externFlag == 0)
            strType = "标准帧 ";
        else
            strType = "扩展帧 ";

        if (objPtr[i].remoteFlag == 0)
        {
            strLen.sprintf("%d", objPtr[i].dataLen);
            for (int j = 0; j < objPtr[i].dataLen; j++)
            {
                tmpstr.sprintf("%02x ", objPtr[i].data[j]);
                strData.append(QString().sprintf("%02x", objPtr[i].data[j]));
                strData.append(" ");
            }
        }
        else
        {
            strLen = QString("0");
        }

        mainWindowPtr->showDataTrig("接收", strCANID, strFormat, strType, strLen, strData);
    }
}
/**
 * @输入参数: 无
 * @输出参数: 无
 * @返 回 值: 无
 * @说    明:FD帧解析
 */
void QUserThread::canFDRecAnalysis(LCAN_CANFD_OBJ *objPtr, UINT len)
{
    UINT i = 0;
    QString tmpstr;
    QString strCANID;
    QString strFormat;
    QString strType;
    QString strLen;
    QString strData;

    for (i = 0; i < len; i++)
    {
        strCANID.clear();
        strFormat.clear();
        strData.clear();
        strLen.clear();
        strType.clear();

        strCANID.sprintf("0X%08x ", objPtr[i].id);

        if (objPtr[i].remoteFlag == 0)
            strFormat = "数据帧 ";
        else
            strFormat = "远程帧 ";

        if (objPtr[i].externFlag == 0)
            strType = "标准帧 ";
        else
            strType = "扩展帧 ";

        if (objPtr[i].remoteFlag == 0)
        {
            strLen.sprintf("%d", objPtr[i].dataLen);
            for (int j = 0; j < objPtr[i].dataLen; j++)
            {
                tmpstr.sprintf("%02x ", objPtr[i].data[j]);
                strData.append(QString().sprintf("%02x", objPtr[i].data[j]));
                strData.append(" ");
            }

        }

        mainWindowPtr->showDataTrig("接收", strCANID, strFormat, strType, strLen, strData);
    }
}
