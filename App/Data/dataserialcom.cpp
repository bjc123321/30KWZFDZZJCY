#include "dataserialcom.h"

#include "Base/Communication/serialCom/serialportmanager.h"
#include "Base/Communication/serialCom/modbusprotocolparser.h"
DataSerialCom::DataSerialCom(QObject *parent) : QObject(parent)
{

    SerialPortManager &serialManager = SerialPortManager::getInstance();
    //绑定onDataReceived槽,即当串口接收到数据后台处理
    connect(&serialManager, &SerialPortManager::dataReceived, this, &DataSerialCom::onDataReceived);


}


void DataSerialCom::onTabChanged(int index)
{
    qDebug()<<"切换到了"<<index<<"页";

    tabIndex = index;
    QStringList requestFramList;
    if(index == 0){
        qDebug()<<"稳态数据：读取...0";

        /*
         * 注意：对于只读取仪表示数前，要先锁定当前数据所在页面0，之后切换仪表显示页，再执行请求帧
         * 这里的requestFramList.at(1):读取线路1瞬时电压偏差-
         * 这里的requestFramList.at(2):读取线路1电压恢复时间
        */

//        SerialPortManager::getInstance().writeData("COM4", QByteArray::fromHex("011000000001020080"));
        requestFramList =QStringList( {"011000000001020080","011000000001020041",
                                       "0103001e0002","010300200002","010300220002",
                                       "0103002e0002","010300300002","010300320002",
                                       "010300380002","0103003a0002","0103003c0002",
                                       "010300520002","010300540002","010300560002",
                                       "0103002a0002","010300340002","0103003e0002","010300580002",
                                       "010300240002","010300260002","010300280002","0103002c0002"});



    }else if(index == 1){
        qDebug()<<"进入整定测试页面,读取...1";
        requestFramList = QStringList({"011000000001020080","011000000001020045",
                                       "010313da0002","010313dc0002","010313de0002","010313e00002",
                                       "010313e20002","010313e40002","010313e60002","010313e80002",
                                       "010313ca0002","010313cc0002","010313ce0002","010313d00002",
                                       "010313d20002","010313d40002","010313d60002","010313d80002"});



    }else{

    }

    for(int i = 0;i<requestFramList.length();i++){

        QString hexString = requestFramList.at(i);  // 获取 QString
        QByteArray byteArray = hexString.toUtf8();  // 将 QString 转换为 QByteArray
        QByteArray dataToSend = QByteArray::fromHex(byteArray);  // 使用 QByteArray::fromHex
        controlDataQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
    }

    // 如果串口不忙，立即发送
    if (!isSerialControlBusy) {
        qDebug()<<"如果串口不忙，立即发送";
        sendNextControlData();
    }else{

         // 弹出一个提示框，告知用户串口正在发送数据请等待...
        qDebug()<<"弹出一个提示框，告知用户串口正在发送数据请等待...";

    }

    requestFramList.clear();
}

void DataSerialCom::startSuddIncreaseSlot()
{
    qDebug()<<"开始突加测试";

    //获取曲线的Y坐标值，034c之后每隔2个寄存器可以获取一个浮点数一直到098c
    QStringList requestFramList ={"011000000001020081",
                                  "0103034c0002","0103034e0002","010303500002",
                                  "010303520002","010303540002","010303560002"
                                  };
    for(int i = 0;i<requestFramList.length();i++){

        QString hexString = requestFramList.at(i);  // 获取 QString
        QByteArray byteArray = hexString.toUtf8();  // 将 QString 转换为 QByteArray
        QByteArray dataToSend = QByteArray::fromHex(byteArray);  // 使用 QByteArray::fromHex
        controlDataQueue.enqueue(dataToSend); // 将数据加入队列
        qDebug() << "数据加入队列：" << dataToSend.toHex()<<"串口是否忙碌:"<<isSerialControlBusy;
    }

    // 如果串口不忙，立即发送
    if (!isSerialControlBusy) {
        qDebug()<<"如果串口不忙，立即发送";
        sendNextControlData();
    }else{

         // 弹出一个提示框，告知用户串口正在发送数据请等待...
        qDebug()<<"弹出一个提示框，告知用户串口正在发送数据请等待...";

    }

    requestFramList.clear();
}



void DataSerialCom::sendNextControlData()
{
    qDebug()<<"********************队列中数据个数:"<<controlDataQueue.length()<<"*******************";
    if (!controlDataQueue.isEmpty()) {
        QByteArray nextData = controlDataQueue.dequeue(); // 从队列中取出数据
        isSerialControlBusy = true; // 标记为忙碌
        if(SerialPortManager::getInstance().writeData("COM4", nextData)){
            qDebug()<<"写数据执行成功Yes";
        }else{
            qDebug()<<"长时间未写入数据,执行失败No..........";
        }
    }else{
        qDebug()<<"队列中数据全部发送完毕!!";

        emit updatePage(dataStrQueue,getTabIndex());
        dataStrQueue.clear();

    }
}

void DataSerialCom::onDataReceived(const QString &portName, const QByteArray &data)
{
     // 仪表返回到主机缓存的数据
    qDebug() << "Data received on port" << portName << ":" << data.toHex();
    //解析缓存的响应帧数据
    analyzingData(data);
    //解析完之后，标记不忙碌。
    isSerialControlBusy = false;
    //继续发送缓存队列中的下一个响应帧数据
    sendNextControlData();


}


void DataSerialCom::analyzingData(const QByteArray &data)
{

    //注意：显示界面数据时，用于切换页面的响应帧不需要解析,而只需要解析查询数据的响应帧
    // 提取功能码(值得注意的是：异常响应中"功能码"的最高位会设置为 1，表示错误响应,)
    uint8_t functionCode = static_cast<uint8_t>(data.at(1));
    if (functionCode >= 0x01 && functionCode <= 0x04){

        ModbusProtocolParser parser;
        QByteArray tempData = data;
        if(parser.parseReponse(tempData)){

            QByteArray dataField = parser.getDataField();
            qDebug()<<"成功analyzingData返回响应帧的数据域:"<<dataField.toHex();
            QString dataStr = QString::number(parser.toFloatData(dataField),'f',3);
            qDebug()<<"dataStr"<<dataStr;


            dataStrQueue.enqueue(dataStr);


        }else{
            qDebug()<<"数据域解析失败！！！！";
            return ;
        }

    }else{
        qDebug()<<"写的响应帧不需要解析";
    }





}
