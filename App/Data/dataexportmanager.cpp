#include "dataexportmanager.h"

DataExportManager::DataExportManager(QObject *parent) : QObject(parent)
{

}

void DataExportManager::createStaticTestExcel()
{
    QXlsx::Document xlsx;

        // 创建表头格式：字体等线，字号10.5，自动换行，水平/垂直居中
        QXlsx::Format headerFormat1;
        headerFormat1.setFontSize(26);
        headerFormat1.setFontBold(true);
        headerFormat1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);  // 水平居中
        headerFormat1.setVerticalAlignment(QXlsx::Format::AlignVCenter);  // 垂直居中



        // 设置标题：电站整机测试报告（合并单元格）
        xlsx.mergeCells("A1:N1", headerFormat1);
        xlsx.write("A1", "电站整机测试报告", headerFormat1);

        QXlsx::Format headerFormat2;
        headerFormat2.setFontSize(15);
        headerFormat2.setFontBold(true);
        headerFormat2.setHorizontalAlignment(QXlsx::Format::AlignHCenter);  // 水平居中
        headerFormat2.setVerticalAlignment(QXlsx::Format::AlignVCenter);  // 垂直居中


        // 设置副标题：连续工况试验报告（工频）（合并单元格）
        xlsx.mergeCells("A2:N2", headerFormat2);
        xlsx.write("A2", "稳态试验报告（工频）", headerFormat2);


        // 创建单独用于第3-7行的格式
       QXlsx::Format headerFormat3;
       headerFormat3.setFontName("DengXian");  // 字体等线
       headerFormat3.setFontSize(10.5);  // 字号10.5

       // 填写第 3-7 行数据并应用格式
       xlsx.write("A3", "测试单位：", headerFormat3);
       xlsx.write("A4", "额定功率（KW）：", headerFormat3);
       xlsx.write("A5", "产品名称：", headerFormat3);
       xlsx.write("A6", "油机型号：", headerFormat3);
       xlsx.write("A7", "环境温度（℃）：", headerFormat3);

       xlsx.write("E3", "执行标准：", headerFormat3);
       xlsx.write("E4", "额定电压 (V)：", headerFormat3);
       xlsx.write("E5", "产品编号：", headerFormat3);
       xlsx.write("E6", "油机编号：", headerFormat3);
       xlsx.write("E7", "相对湿度：", headerFormat3);

       xlsx.write("H4", "额定频率（Hz）：", headerFormat3);
       xlsx.write("H5", "产品编号：", headerFormat3);
       xlsx.write("H6", "电机型号：", headerFormat3);
       xlsx.write("H7", "大气压力（KPa）：", headerFormat3);

       xlsx.write("L3", "试验时间：", headerFormat3);
       xlsx.write("L4", "产品状态：", headerFormat3);
       xlsx.write("L5", "相/线：", headerFormat3);
       xlsx.write("L6", "电机编号：", headerFormat3);
       xlsx.write("L7", "测试负责人：", headerFormat3);

       // 合并顶部信息的单元格
       xlsx.mergeCells("A3:B3");
       xlsx.mergeCells("A4:B4");
       xlsx.mergeCells("A5:B5");
       xlsx.mergeCells("A6:B6");
       xlsx.mergeCells("A7:B7");

       xlsx.mergeCells("E3:F3");
       xlsx.mergeCells("E4:F4");
       xlsx.mergeCells("E5:F5");
       xlsx.mergeCells("E6:F6");
       xlsx.mergeCells("E7:F7");

       xlsx.mergeCells("H4:I4");
       xlsx.mergeCells("H5:I5");
       xlsx.mergeCells("H6:I6");
       xlsx.mergeCells("H7:I7");


        // 表格内容格式：自动换行，居左
        QXlsx::Format contentFormat;
        contentFormat.setFontName("DengXian");
        contentFormat.setFontSize(10.5);
        contentFormat.setTextWarp(true);  // 自动换行
        contentFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        contentFormat.setBorderStyle(QXlsx::Format::BorderThin);  // 细框线


        // 第8行表头
        xlsx.mergeCells("A8:A9", contentFormat);
        xlsx.write("A8", "负载状况 (%)", contentFormat);

        xlsx.mergeCells("B8:B9", contentFormat);
        xlsx.write("B8", "有功功率\n(KW)", contentFormat);

        xlsx.mergeCells("C8:C9", contentFormat);
        xlsx.write("C8", "功率因数\n(PF)", contentFormat);

        xlsx.mergeCells("D8:F8", contentFormat);
        xlsx.write("D8", "电流 (A)", contentFormat);
        xlsx.write("D9", "Ia", contentFormat);
        xlsx.write("E9", "Ib", contentFormat);
        xlsx.write("F9", "Ic", contentFormat);

        xlsx.mergeCells("G8:L8", contentFormat);
        xlsx.write("G8", "电压 (V)", contentFormat);
        xlsx.write("G9", "Uan", contentFormat);
        xlsx.write("H9", "Ubn", contentFormat);
        xlsx.write("I9", "Ucn", contentFormat);
        xlsx.write("J9", "Uab", contentFormat);
        xlsx.write("K9", "Ubc", contentFormat);
        xlsx.write("L9", "Uca", contentFormat);

        xlsx.mergeCells("M8:N8", contentFormat);
        xlsx.write("M8", "频率 (Hz)", contentFormat);
        xlsx.write("M9", "farb", contentFormat);
        xlsx.write("N9", "fmax", contentFormat);

        // 填充第10行至18行的空数据
        QStringList loadConditions = {"0", "25", "50", "75", "100", "75", "50", "25", "0"};
        for (int row = 10; row <= 18; ++row) {
            xlsx.write(QString("A%1").arg(row), loadConditions.at(row - 10), contentFormat);
            xlsx.write(QString("B%1").arg(row), "", contentFormat);  // 有功功率空白
            xlsx.write(QString("C%1").arg(row), "", contentFormat);  // 功率因数空白
            xlsx.write(QString("D%1").arg(row), "", contentFormat);  // Ia空白
            xlsx.write(QString("E%1").arg(row), "", contentFormat);  // Ib空白
            xlsx.write(QString("F%1").arg(row), "", contentFormat);  // Ic空白
            xlsx.write(QString("G%1").arg(row), "", contentFormat);  // Uan空白
            xlsx.write(QString("H%1").arg(row), "", contentFormat);  // Ubn空白
            xlsx.write(QString("I%1").arg(row), "", contentFormat);  // Ucn空白
            xlsx.write(QString("J%1").arg(row), "", contentFormat);  // Uab空白
            xlsx.write(QString("K%1").arg(row), "", contentFormat);  // Ubc空白
            xlsx.write(QString("L%1").arg(row), "", contentFormat);  // Uca空白
            xlsx.write(QString("M%1").arg(row), "", contentFormat);  // farb空白
            xlsx.write(QString("N%1").arg(row), "", contentFormat);  // fmax空白
        }

        // 第21行到25行
        xlsx.mergeCells("A21:E22", contentFormat);
        xlsx.write("A21", "负载状况 (%)", contentFormat);
        for (int col = 6; col <= 14 && col - 6 < loadConditions.size(); ++col) {
            xlsx.write(21, col, loadConditions.at(col - 6), contentFormat);
            xlsx.write(22, col, "", contentFormat);// 空数据单元格
        }

        xlsx.mergeCells("A23:E23", contentFormat);
        xlsx.write("A23", "电压波动率δUb (%)", contentFormat);
        for (int col = 6; col <= 14; ++col) {
            xlsx.write(23, col, "", contentFormat);  // 空数据单元格
        }

        xlsx.mergeCells("A24:E24", contentFormat);
        xlsx.write("A24", "稳态频率调整系数δf (%)", contentFormat);
        for (int col = 6; col <= 14; ++col) {
            xlsx.write(24, col, "", contentFormat);  // 空数据单元格
        }

        xlsx.mergeCells("A25:E25", contentFormat);
        xlsx.write("A25", "频率波动率δf (%)", contentFormat);
        for (int col = 6; col <= 14; ++col) {
            xlsx.write(25, col, "", contentFormat);  // 空数据单元格
        }

        // 第27行: 测试结果

        // 创建单独用于第3-7行的格式
       QXlsx::Format headerFormat4;
       headerFormat4.setFontName("DengXian");  // 字体等线
       headerFormat4.setFontSize(10.5);  // 字号10.5
       headerFormat4.setTextWarp(true);  // 自动换行

        xlsx.mergeCells("A27:B27", headerFormat4);
        xlsx.write("A27", "测试结果：", headerFormat4);

        // 第28行: 稳态电压调整率 δU (%) 和 稳态电压调整率指标
        xlsx.write("A28", "稳态电压调整率 δU (%)：", headerFormat4);

        xlsx.write("E28", "稳态电压调整率指标：", headerFormat4);

        xlsx.write("H28", "电压波动率 δUb (%)：", headerFormat4);

        xlsx.write("L28", "稳态电压波动率指标：", headerFormat4);

        // 第29行: 稳态频率调整率 δf (%) 和 稳态频率调整率指标

        xlsx.write("A29", "稳态频率调整率 δf (%)：", headerFormat4);

        xlsx.write("E29", "稳态频率调整率指标：", headerFormat4);

        xlsx.write("H29", "频率波动率 δfB (%)：", headerFormat4);

        xlsx.write("L29", "稳态频率波动率指标：", headerFormat4);

        // 保存Excel文件到桌面
        QString filePath = QDir::homePath() + "/Desktop/StaticWorking.xlsx";
        if (xlsx.saveAs(filePath)) {
            qDebug() << "Excel report with two tables created and saved at:" << filePath;
        } else {
            qDebug() << "Failed to save the Excel report.";
        }
}
