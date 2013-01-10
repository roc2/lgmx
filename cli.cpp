#include <QStringList>
#include <list>

#include <cli.h>
#include <exception.h>
#include <view_manager.h>


lgmx::cli::cli(view_manager *manager)
{
	manager_ = manager;
	add_command(new tab_width_cmd("tabwidth", manager));
	add_command(new version_cmd("version", manager));
}


lgmx::cli::~cli()
{
	// delete all commands from map
}

bool lgmx::cli::add_command(command *cmd)
{
	cmd_map_[cmd->get_name()] = cmd;
	return true;
};


/**
 *
 */

cmd::stat lgmx::cli::execute(QString &cmd_str, QString &result)
{
	std::map<QString, command *>::iterator it;

	// split command into tokens
	QStringList cmd(cmd_str.split(' ', QString::SkipEmptyParts));

	it = cmd_map_.find(cmd.at(0));
	
	if (it == cmd_map_.end()) {
		result = "Unknown command: " + cmd.at(0);
		return cmd::ERR;
	}

	return it->second->execute(cmd, result);
}

void lgmx::cli::parse(QString &cmd_str)
{
	


}











