#ifndef CLI_H
#define CLI_H

#include <map>
#include <QString>
#include <command.h>

class command;
class view_manager;

namespace lgmx {

class cli
{
public:
	cli(view_manager *manager);
	~cli();

	bool add_command(command *cmd);

	cmd::stat execute(QString &cmd_str, QString &result);

	void parse(QString &cmd_str);
		// faz o match do nome do comando no map e chama o respectivo 
		// cmd_obj passando os parametros em uma lista de strings (tokens)
		


private:
	view_manager *manager_;
	std::map<QString, command *> cmd_map_;
};

}


#endif
