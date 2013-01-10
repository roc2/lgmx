#include <command.h>
#include <view_manager.h>
#include <iostream>

command::command(QString& name, view_manager* manager) : name_(name), manager_(manager)
{
}

command::~command()
{
}

QString& command::get_name()
{
	return name_;
}

/////////////////////

tab_width_cmd::tab_width_cmd(QString name, view_manager* manager) : command(name, manager)
{
	
}

tab_width_cmd::~tab_width_cmd()
{
}

cmd::stat tab_width_cmd::execute(QStringList &params, QString &result)
{
	switch (params.size()) {
	case 1:
		result = "tab width = 4";
		return cmd::OK_RES;
		
	case 2: {
		bool ok = false;
		int size = params.at(1).toInt(&ok, 10);
		
		if (ok) {
			manager_->set_tab_width(size);
		} else {
			result = "Invalid parameter: " + params.at(1);
			return cmd::ERR;
		}
		
		return cmd::OK;
	}

	default:
		result = "Invalid parameters";
		return cmd::ERR;
	}

	
	std::cout << "Executing tab_width_cmd!!" << std::endl;
	return cmd::OK;
}

/////////////////////

version_cmd::version_cmd(QString name, view_manager* manager) : command(name, manager)
{
}

version_cmd::~version_cmd()
{
}

cmd::stat version_cmd::execute(QStringList &params, QString &result)
{
	if (params.size() > 1) {
		result = "Invalid parameters";
		return cmd::ERR;
	}

	result = manager_->get_version_number();
	return cmd::OK_RES;
}












