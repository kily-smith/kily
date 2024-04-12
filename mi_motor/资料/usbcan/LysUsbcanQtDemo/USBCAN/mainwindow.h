/**
******************************************************************************
* @file    mainwindow.h
* @author  lys
* @version V1.1.0
* @date    2018
* @brief   USBCAN调试助手
*
* @verbatim
*
* @note
*
* @endverbatim
*
******************************************************************************
* 版权:长沙六叶树教育科技有限公司
* 官网:www.liuyeshu.cn
* 更多资料下载见官网
******************************************************************************
修改日期    版本号   修改者   功能描述
2018.11.10 V1.1.0  LYS
----------------------------------------------------------------------------
******************************************************************************
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QDialog>
#include <QMessageBox>
#include <QLibrary>
#include <QThread>
#ifdef __linux__
#include "ControlCAN_linux.h"
#else
#include "ControlCAN.h"
#endif
#include "lysStyle.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow;

class QUserThread : public QThread
{
public:
    QUserThread();
    ~QUserThread();
    //任务主体
    void run();
    //线程初始化
    void threadInit(MainWindow *ptr);
    //启动线程
    void threadStart(void);
    //停止线程
    void threadStop(void);
    void canRecAnalysis(LCAN_CAN_OBJ *objPtr, UINT len);
    void canFDRecAnalysis(LCAN_CANFD_OBJ *objPtr, UINT len);
private:
    MainWindow *mainWindowPtr;
    bool  m_exit;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

 public:
    void showDataTrig(QString strDir, QString strID, QString strFormat, QString strType, QString strLen, QString strData);   

signals:
    void showData(QString strDir, QString strID, QString strFormat, QString strType, QString strLen, QString strData);

private:
    int chartoint(unsigned char chr, unsigned char *cint);
    int strtodata(unsigned char *str, unsigned char *data, int len, int flag);

    void windowInit(void);
    void windowStyleInit(void);
    void windowExit(void);
    void windowUpdate(void);
    //设备
    void windowDevInitCfgGetHandle(void);
    void devConnectHandle(void);
    void devCanSendHandle(void);
    void devCanFDSendHandle(void);

private slots:
        void OnSelChangeComboDevType(int nIndex);
        void OnSelChangeComboWorkPro(int nIndex);
        void OnButtonDevOpen();
        void OnButtonDevClose();
        void OnButtonDevSend();
        void OnButtonExit();
        void OnButtonClr();
        void onShowData(QString dir, QString strID, QString strFormat, QString strType, QString strLen, QString strData);

public:
    //lib
    QLibrary *controlCANLib;
    //设备连接状态
    int m_devConnect;
    //设备类型
    int m_devType;
    //设备索引
    int m_devIndex;
    //通道索引
    int m_devChIndex;
    //工作模式
    int m_devWorkMode;
    //过滤器模式
    int m_devFliterMode;
    //是否是FD设备
    bool isFd;
    //是否工作于CAN模式
    bool workProIsCan;
    //CAN配置信息
    LCAN_INIT_CONFIG canInitCfgInfo;
    //FD配置信息
    LCAN_INITFD_CONFIG canInitFDCfgInfo;
private:
    Ui::MainWindow *ui;
    //style
    lysStyle lysStyleInstance;
    //thread
    QUserThread userThread;
};
#endif // MAINWINDOW_H
