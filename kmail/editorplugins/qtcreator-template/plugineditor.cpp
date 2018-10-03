%{Cpp:LicenseTemplate}\

#include "%{CN}plugineditor.h"
#include "%{CN}plugineditorinterface.h"
#include <kpluginfactory.h>

K_PLUGIN_CLASS_WITH_JSON(%{CN}PluginEditor, "%{CN}editorplugin.json")

%{CN}PluginEditor::%{CN}PluginEditor(QObject *parent, const QList<QVariant> &)
    : MessageComposer::PluginEditor(parent)
{
}

%{CN}PluginEditor::~%{CN}PluginEditor()
{
}

bool %{CN}PluginEditor::hasPopupMenuSupport() const
{
    return false;
}

MessageComposer::PluginEditorInterface *%{CN}PluginEditor::createInterface(KActionCollection *ac, QObject *parent)
{
    %{CN}PluginEditorInterface *interface = new %{CN}PluginEditorInterface(parent);
    interface->createAction(ac);
    return interface;
}

#include "%{JS: '%{ProjectName}plugineditor.moc'.toLowerCase() }"
