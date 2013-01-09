#include <command.h>
#include <view_manager.h>
#include <iostream>

command::command(QString& name, view_manager* manager) : name_(name), manager_(manager)
{
	
}

	
command::~command()
{
}

/////////////////////

tab_width_cmd::tab_width_cmd(QString name, view_manager* manager) : command(name, manager)
{
	
}

tab_width_cmd::~tab_width_cmd()
{
}

void tab_width_cmd::param_parser()
{
	
}

int tab_width_cmd::execute(QStringList &params, QString &result)
{
	switch (params.size()) {
	case 1:
		result = "tab width = 4";
		return 0;
		
	case 2: {
		bool ok = false;
		int size = params.at(1).toInt(&ok, 10);
		
		if (ok) {
			manager_->set_tab_width(size);
		} else {
			result = "Invalid parameter: " + params.at(1);
			return -1;
		}
		
		return 0;
	}

	default:
		result = "Invalid parameters";
		return -1;
	}

	
	std::cout << "Executing tab_width_cmd!!" << std::endl;
	return 0;
}













