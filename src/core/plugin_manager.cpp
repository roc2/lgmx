#include <QDir>
#include <QPluginLoader>
#include <QApplication>
#include <QString>

#include <iostream>

#include <plugin_manager.h>
#include <interfaces.h>

using namespace std;

plugin_manager::plugin_manager()
{
	plugin_map_[file_type::CPP] = (QStringList() << "liblgmx_cpp_highlighter.so");
}

plugin_manager::~plugin_manager()
{
}

bool plugin_manager::load_plugins(file_type::type tp)
{
	switch (tp) {
	
	case file_type::UNKNOWN:
		break;

	case file_type::CPP:
		load_cpp_plugins();
		break;
	
	default:
		break;
	}
	
	return false;
}

bool plugin_manager::load_cpp_plugins()
{
	QDir pluginsDir = QDir(QApplication::applicationDirPath());
	pluginsDir.cd("plugins");
	
	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));

		QObject *plugin = loader.instance();	// get plugin instance

		if (plugin) {
			cout << "plugin loaded: " << fileName.toStdString() << endl;
			
			Highlighter *hl = qobject_cast<Highlighter *>(plugin);
			
			if (hl) {
				cout << "plugin Ok!!" << endl;
				cout << hl->test_interface().toStdString() << endl;
				
				if (loader.unload()) {
					cout << "plugin unloaded Ok!!" << endl;
				} else {
					cout << "could not unload plugin" << endl;
				}
			} else
				cout << "plugin not ok" << endl; 
			
		} else {
			cout << "plugin not loaded: " << fileName.toStdString() << endl;
		}
	 }
	 
	 return true;
}		 
		 
		 
		 
		 
