/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <MessageViewer/ViewerPluginInterface>
class KActionCollection;
namespace PimCommonTextTranslator
{
class TranslatorWidget;
}
namespace MessageViewer
{
class ViewerPluginTranslatorInterface : public ViewerPluginInterface
{
    Q_OBJECT
public:
    explicit ViewerPluginTranslatorInterface(KActionCollection *ac, QWidget *parent = nullptr);
    ~ViewerPluginTranslatorInterface() override;

    void setText(const QString &text) override;
    Q_REQUIRED_RESULT QList<QAction *> actions() const override;
    void showWidget() override;
    Q_REQUIRED_RESULT ViewerPluginInterface::SpecificFeatureTypes featureTypes() const override;

private:
    Q_REQUIRED_RESULT PimCommonTextTranslator::TranslatorWidget *widget();
    void createAction(KActionCollection *ac);
    QList<QAction *> mAction;
    PimCommonTextTranslator::TranslatorWidget *mTranslatorWidget = nullptr;
};
}
