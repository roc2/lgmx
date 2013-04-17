#include <QDir>
#include <QPluginLoader>
#include <QApplication>
#include <QString>

#include <iostream>

#include <plugin_manager.h>
#include <interfaces.h>


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
			std::cout << "plugin loaded: " << fileName.toStdString() << std::endl;
			
			Highlighter *hl = qobject_cast<Highlighter *>(plugin);
			
			if (hl) {
				std::cout << "plugin Ok!!" << std::endl;
				std::cout << hl->test_interface().toStdString() << std::endl;
				
				if (loader.unload()) {
					std::cout << "plugin unloaded Ok!!" << std::endl;
				} else {
					std::cout << "could not unload plugin" << std::endl;
				}
			} else
				std::cout << "plugin not ok" << std::endl; 
			
		} else {
			std::cout << "plugin not loaded: " << fileName.toStdString() << std::endl;
		}
	 }
	 
	 return true;
}		 
		 
		 
		 
		 
