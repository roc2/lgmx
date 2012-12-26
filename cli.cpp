
#include <list>

#include <cli.h>
#include <exception.h>

command::command(cli *parent)
{
	parent->add_command(this);	
}





bool cli::add_command(command *cmd)
{
	cmd_map_[cmd->get_name()] = cmd;
	return true;
};




void cli::execute(QString &cmd_str)
{
	QString cmd_name;
	std::map<QString, command *>::iterator it;

	it = cmd_map_.find(cmd_name);
	
	if (it == cmd_map_.end()) {
		lgmx::exception excp("Unknown command: " + cmd_name.toStdString());
		throw excp;
	}
	
}


class tab_width_cmd : public command
{
	




};




