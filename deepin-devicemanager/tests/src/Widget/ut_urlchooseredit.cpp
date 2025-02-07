/*
* Copyright (C) 2019 ~ 2020 UnionTech Software Technology Co.,Ltd
*
* Author:      jixiaomei <jixiaomei@uniontech.com>
* Maintainer:  jixiaomei <jixiaomei@uniontech.com>
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "UrlChooserEdit.h"
#include "ut_Head.h"

#include <DLineEdit>
#include <DSuggestButton>

#include <QDir>
#include <QIcon>
#include <QFileDialog>

#include <gtest/gtest.h>
#include "stub.h"

class UT_UrlChooserEdit : public UT_HEAD
{
public:
    void SetUp()
    {
        m_UrlChooserEdit = new UrlChooserEdit;
    }
    void TearDown()
    {
        delete m_UrlChooserEdit;
    }

    UrlChooserEdit *m_UrlChooserEdit;
};

QString ut_getExistingDirectory()
{
    return "/home";
}

QString ut_getExistingDirectory_empty()
{
    return "";
}

TEST_F(UT_UrlChooserEdit, UT_UrlChooserEdit_initConnections)
{
    Stub stub;
    stub.set(ADDR(QFileDialog, getExistingDirectory), ut_getExistingDirectory);

    m_UrlChooserEdit->initConnections();

    emit m_UrlChooserEdit->mp_urlBtn->click();

    EXPECT_STREQ("/home", m_UrlChooserEdit->mp_urlEdit->text().toStdString().c_str());
    EXPECT_STREQ("/home", m_UrlChooserEdit->mp_elidParh.toStdString().c_str());
    EXPECT_STREQ("/home", m_UrlChooserEdit->mp_folderPath.toStdString().c_str());
}

TEST_F(UT_UrlChooserEdit, UT_UrlChooserEdit_slotChooseUrl)
{
    Stub stub;
    stub.set(ADDR(QFileDialog, getExistingDirectory), ut_getExistingDirectory_empty);

    m_UrlChooserEdit->slotChooseUrl();

    QString dir = QDir::homePath();
    EXPECT_STREQ(dir.toStdString().c_str(), m_UrlChooserEdit->mp_urlEdit->text().toStdString().c_str());
    EXPECT_STREQ(dir.toStdString().c_str(), m_UrlChooserEdit->mp_elidParh.toStdString().c_str());
    EXPECT_STREQ(dir.toStdString().c_str(), m_UrlChooserEdit->mp_folderPath.toStdString().c_str());
}

TEST_F(UT_UrlChooserEdit, UT_UrlChooserEdit_checkLocalFolder_001)
{
    m_UrlChooserEdit->mp_folderPath = "smb://10.4.10.4";
    m_UrlChooserEdit->checkLocalFolder("/home");
    EXPECT_TRUE(m_UrlChooserEdit->mp_urlEdit->isAlert());
}

TEST_F(UT_UrlChooserEdit, UT_UrlChooserEdit_text)
{
    m_UrlChooserEdit->mp_folderPath = "/home";
    QString text = m_UrlChooserEdit->text();
    EXPECT_STREQ("/home", text.toStdString().c_str());
}
