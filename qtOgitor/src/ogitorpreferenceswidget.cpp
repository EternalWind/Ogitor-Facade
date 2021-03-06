/*/////////////////////////////////////////////////////////////////////////////////
/// An
///    ___   ____ ___ _____ ___  ____
///   / _ \ / ___|_ _|_   _/ _ \|  _ \
///  | | | | |  _ | |  | || | | | |_) |
///  | |_| | |_| || |  | || |_| |  _ <
///   \___/ \____|___| |_| \___/|_| \_\
///                              File
///
/// Copyright (c) 2008-2011 Ismail TARIM <ismail@royalspor.com> and the Ogitor Team
//
/// The MIT License
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////*/

#include <iostream>

#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QTranslator>

#include <QtGui/QMessageBox>
#include "OgreRoot.h"
#include "ogitorpreferenceswidget.hxx"

OgitorPreferencesWidget::OgitorPreferencesWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    styleSheetList->addItem(":/stylesheets/obsidian.qss");
    styleSheetList->addItem(":/stylesheets/aqua.qss");
    styleSheetList->addItem(":/stylesheets/osx.qss");
    styleSheetList->addItem(":/stylesheets/KDE4.qss");

    styleSheetList->setCurrentIndex(0);

    QDir myDir("../languages");
    QStringList list = myDir.entryList(QStringList("ogitor_*.qm")); // filter only translation files
    
    /** Add System Default Option Manually */
    languageFileList->addItem(tr("Use System Default..."));
    mLanguageMap[tr("Use System Default...")] = "";

    for (int i = 0; i < list.size(); ++i)
    {
        QString str = list.at(i);
        str.remove("ogitor_");
        str.remove(".qm");
        QLocale loc(str);
        languageFileList->addItem(QLocale::languageToString(loc.language()));
        mLanguageMap[QLocale::languageToString(loc.language())] = list.at(i);
    }
    
    /** Add English Option Manually */
    languageFileList->addItem("English");
    mLanguageMap["English"] = "ogitor_en.qm";
    
    langGroupBox->setToolTip(tr("System locale: ") + QLocale::system().name());
    
    fillOgreTab();

    QVBoxLayout *vlayout = new QVBoxLayout();
    QHBoxLayout *hlayout = new QHBoxLayout();

    treeWidget = new QTreeWidget();
    treeWidget->setHeaderLabel(tr("List of all loaded Plugins"));

    const Ogitors::PluginEntryMap *pluginmap = Ogitors::OgitorsRoot::getSingletonPtr()->GetPluginMap();

    Ogitors::PluginEntryMap::const_iterator it = pluginmap->begin();

    unsigned int t;
    QTreeWidgetItem *topItem;
    QTreeWidgetItem *catItem;
    QTreeWidgetItem *subItem;

    while(it != pluginmap->end())
    {
        topItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(it->second.mName.c_str())));
        treeWidget->addTopLevelItem(topItem);

        if(it->second.mEditorObjects.size() > 0)
        {
            catItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Editor Objects")));
            catItem->setIcon(0, QIcon(":/icons/objects.svg"));
            topItem->addChild(catItem);

            for(t = 0;t < it->second.mEditorObjects.size();t++)
            {
                subItem =  new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(it->second.mEditorObjects[t].c_str())));
                subItem->setIcon(0, QIcon(":/icons/bullet.svg"));
                catItem->addChild(subItem);
            }
        }

        if(it->second.mSerializers.size() > 0)
        {
            catItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Scene Serializers")));
            catItem->setIcon(0, QIcon(":/icons/import.svg"));
            topItem->addChild(catItem);

            for(t = 0;t < it->second.mSerializers.size();t++)
            {
                subItem =  new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(it->second.mSerializers[t].c_str())));
                subItem->setIcon(0, QIcon(":/icons/bullet.svg"));
                catItem->addChild(subItem);
            }
        }

        if(it->second.mScriptInterpreters.size() > 0)
        {
            catItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Script Interpreters")));
            catItem->setIcon(0, QIcon(":/icons/script.svg"));
            topItem->addChild(catItem);

            for(t = 0;t < it->second.mScriptInterpreters.size();t++)
            {
                subItem =  new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(it->second.mScriptInterpreters[t].c_str())));
                subItem->setIcon(0, QIcon(":/icons/bullet.svg"));
                catItem->addChild(subItem);
            }
        }

        if(it->second.mTabWidgets.size() > 0)
        {
            catItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Tab Widgets")));
            catItem->setIcon(0, QIcon(":/icons/showhide.svg"));
            topItem->addChild(catItem);

            for(t = 0;t < it->second.mTabWidgets.size();t++)
            {
                try
                {
                    subItem =  new QTreeWidgetItem((QTreeWidget*)0, QStringList(static_cast<QWidget*>(it->second.mTabWidgets[t])->objectName()));
                    subItem->setIcon(0, QIcon(":/icons/bullet.svg"));
                    catItem->addChild(subItem);
                }
                catch(...)
                {
                }
            }
        }

        if(it->second.mDockWidgets.size() > 0)
        {
            catItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Dock Widgets")));
            catItem->setIcon(0, QIcon(":/icons/showhide.svg"));
            topItem->addChild(catItem);

            for(t = 0;t < it->second.mDockWidgets.size();t++)
            {
                try
                {
                    subItem =  new QTreeWidgetItem((QTreeWidget*)0, QStringList(static_cast<QWidget*>(it->second.mDockWidgets[t])->objectName()));
                    subItem->setIcon(0, QIcon(":/icons/bullet.svg"));
                    catItem->addChild(subItem);
                }
                catch(...)
                {
                }
            }
        }

        if(it->second.mToolbars.size() > 0)
        {
            catItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(tr("Toolbars")));
            catItem->setIcon(0, QIcon(":/icons/toolbar.svg"));
            topItem->addChild(catItem);

            for(t = 0;t < it->second.mToolbars.size();t++)
            {
                try
                {
                    subItem =  new QTreeWidgetItem((QTreeWidget*)0, QStringList(static_cast<QWidget*>(it->second.mToolbars[t])->objectName()));
                    subItem->setIcon(0, QIcon(":/icons/bullet.svg"));
                    catItem->addChild(subItem);
                }
                catch(...)
                {
                }
            }
        }

        it++;
    }

    hlayout->addWidget(treeWidget);
    vlayout->addLayout(hlayout);
    tabPluginsInfo->setLayout(vlayout);

    connect(splashscreenCheckBox,     SIGNAL(stateChanged(int)), this, SLOT(setDirty()));
    connect(loadLastCheckBox,         SIGNAL(stateChanged(int)), this, SLOT(setDirty()));
    connect(styleSheetList,           SIGNAL(currentIndexChanged(int)), this, SLOT(setDirty()));
    connect(languageFileList,         SIGNAL(currentIndexChanged(int)), this, SLOT(setDirty()));
    connect(languageFileList,         SIGNAL(currentIndexChanged(int)), this, SLOT(languageChanged()));
    connect(renderSystemComboBox,     SIGNAL(currentIndexChanged(int)), this, SLOT(setDirty()));
    connect(useVSyncCheckBox,         SIGNAL(stateChanged(int)),        this, SLOT(setDirty()));
    connect(antiAlliasingComboBox,    SIGNAL(currentIndexChanged(int)), this, SLOT(setDirty()));
}

OgitorPreferencesWidget::~OgitorPreferencesWidget()
{
}

void OgitorPreferencesWidget::getPreferences(Ogre::NameValuePairList& preferences)
{
    // just save the stuff on our own - no need to pass it to the manager
    QSettings settings;
    settings.setValue("preferences/skipSplash", splashscreenCheckBox->isChecked());
    settings.setValue("preferences/loadLastLoadedScene", loadLastCheckBox->isChecked());
    
    // Why is this in a Ogre::NameValuePairList ???!!
    preferences.insert(Ogre::NameValuePairList::value_type("customStyleSheet",
                                Ogre::String(this->styleSheetList->currentText().toStdString())));
    
    QString langFileName = languageFileList->itemText(0);
    if(languageFileList->currentIndex() != 0)
        langFileName = mLanguageMap.value(languageFileList->currentText());
    
    preferences.insert(Ogre::NameValuePairList::value_type("customLanguage",
                                                            Ogre::String(langFileName.toStdString())));
    preferences.insert(Ogre::NameValuePairList::value_type("useVSync",
                                                            Ogre::StringConverter::toString(this->useVSyncCheckBox->isChecked())));
    preferences.insert(Ogre::NameValuePairList::value_type("antiAliasing",
                                Ogre::String(this->antiAlliasingComboBox->currentText().toStdString())));
}


void *OgitorPreferencesWidget::getPreferencesWidget(const Ogre::NameValuePairList& list)
{
//     Ogre::NameValuePairList::const_iterator ni;
    
//     if ((ni = list.find("Appearence/useCustumStyleSheet")) != list.end())
//     {
//         this->useCustomStyleCheckbox->setChecked(Ogre::StringConverter::parseBool(ni->second));
//
//         if(Ogre::StringConverter::parseBool(ni->second))
//         {
//             if(list.find("Appearence/customCtyleSheetListSelected") != list.end())
//             {
//                 ni = list.find("Appearence/customCtyleSheetListSelected");
//                 Ogre::String str(ni->second);
//                 QString qstr(str.c_str());
//                 int index = styleSheetList->findText(qstr, Qt::MatchExactly);
//                 styleSheetList->setCurrentIndex(index);
//             }
//         }
//     }
    
    QSettings settings;
    settings.beginGroup("preferences");
    
    loadLastCheckBox->setChecked(settings.value("loadLastLoadedScene", false).toBool());
    splashscreenCheckBox->setChecked(settings.value("skipSplash", false).toBool());

    QString style = settings.value("customStyleSheet").toString();
    int result = styleSheetList->findText(style);
    if(result > -1)
        styleSheetList->setCurrentIndex(result);
    else
        styleSheetList->setCurrentIndex(0);

    QString lang = settings.value("customLanguage").toString();
    QMapIterator<QString, QString> i(mLanguageMap);
    while (i.hasNext()) 
    {
        i.next();
        if(lang == i.value())
        {
            languageFileList->setCurrentIndex(languageFileList->findText(i.key()));
            break;
        }
    }

    mLanguageChanged = false;
    
    if(antiAlliasingComboBox->findText(settings.value("antiAliasing").toString()) >= 0)
        antiAlliasingComboBox->setCurrentIndex(antiAlliasingComboBox->findText(settings.value("antiAliasing").toString()));
    
    useVSyncCheckBox->setChecked(settings.value("useVSync").toBool());
    
    settings.endGroup();
    
    applyPreferences();
    return this;
}


bool OgitorPreferencesWidget::applyPreferences()
{
    // Use default should always be at index 0
     if(styleSheetList->currentIndex() != 0)
     {
         if(!QFile::exists(getPrefCustomStyleSheet()))
         {
             QMessageBox::warning(QApplication::activeWindow(),tr("Preferences"), tr("Can not find the requested StyleSheet"), QMessageBox::Ok);
             return false;
         }
         QFile file(getPrefCustomStyleSheet());
         file.open(QFile::ReadOnly);
         QString styleSheet = QLatin1String(file.readAll());
         qApp->setStyleSheet(styleSheet);
         file.close();
     }
     else
     {
         QFile file(getPrefCustomStyleSheet());
         file.open(QFile::ReadOnly);
         QString styleSheet = QLatin1String(file.readAll());
         qApp->setStyleSheet(styleSheet);
         file.close();
     }

    if(mLanguageChanged)
        QMessageBox::warning(QApplication::activeWindow(),tr("Preferences"), tr("Language will be changed when Ogitor is restarted!"), QMessageBox::Ok);
    mLanguageChanged = false;
    
//     if(useVSyncCheckBox->isChecked())
//     {
//         Ogre::Root::getSingletonPtr()->getRenderSystem()->setConfigOption("VSync", "Yes");
//     Ogre::Root::getSingletonPtr()->getRenderSystem()->reinitialise();
//     }
//     else
//     {
//         Ogre::Root::getSingletonPtr()->getRenderSystem()->setConfigOption("VSync", "No");
//         Ogre::Root::getSingletonPtr()->getRenderSystem()->reinitialise();
//     }
    
    return true;
}

void OgitorPreferencesWidget::fillOgreTab()
{
    Ogre::RenderSystemList::const_iterator pRend = Ogre::Root::getSingletonPtr()->getAvailableRenderers().begin();
    while (pRend != Ogre::Root::getSingletonPtr()->getAvailableRenderers().end())
    {
        renderSystemComboBox->addItem((*pRend)->getName().c_str());
        pRend++;
    }

    antiAlliasingComboBox->addItem("0");
    antiAlliasingComboBox->addItem("2");
    antiAlliasingComboBox->addItem("4");
    antiAlliasingComboBox->addItem("8");
    antiAlliasingComboBox->addItem("16");
    antiAlliasingComboBox->setCurrentIndex(0);
}


void OgitorPreferencesWidget::setDirty()
{
    emit isDirty();
}

void OgitorPreferencesWidget::languageChanged()
{
    mLanguageChanged= true;
}

QString OgitorPreferencesWidget::getPrefCustomStyleSheet()
{
    return styleSheetList->currentText();
}

QString OgitorPreferencesWidget::getPrefCustomLanguage()
{
    return languageFileList->currentText();
}
