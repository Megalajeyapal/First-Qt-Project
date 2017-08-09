/*****************************************************************//*
 *
 * @file        PopUp.h
 * @brief       This class implements the PopUp UI.
 *
 * @copyright Copyright 2016 by Honeywell International Inc.
 * All rights reserved.  This software and code comprise proprietary
 * information of Honeywell International Inc.  This software and code
 * may not be reproduced, used, altered, re-engineered, distributed or
 * disclosed to others without the written consent of Honeywell.
 ********************************************************************/

#ifndef POPUP_H
#define POPUP_H

#include <QMap>
#include <QWidget>
#include <QFileDialog>
#include <QFile>

namespace Ui {
class PopUp;
}

/************************************************//*
 *  @class      PopUp
 *  @brief      PopUp class declaration
 *************************************************/

class PopUp : public QWidget
{
    Q_OBJECT

public:

    //public methods declaration begins
    explicit PopUp(QWidget *parent = 0);
    ~PopUp();
    void SetText(QStringList eventCode);

private slots:

    //private slots declaration begins
    void on_pushButton_clicked();

private:

    //private attributes declaration begins
    Ui::PopUp *ui;
    QFileDialog *m_fd;
    static int rowCount;

};

#endif // POPUP_H
