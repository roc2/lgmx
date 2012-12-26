#ifndef CLI_H
#define CLI_H

#include <map>
#include <QString>

class cli;

class command
{
protected:

	command(cli *parent);

	virtual void param_parser(std::list<QString> &params) = 0;

	virtual int execute() = 0;

public:
	QString& get_name() {return name;};

protected:
	QString name;
	int multiplicity;
	bool write;
};




class cli
{
	//enum cmd_ = {TAB_WIDTH, FONT_COLOR};
public:
	bool add_command(command *cmd);

	void execute(QString &cmd_str);

	void parse(QString &cmd_str) {
		// faz o match do nome do comando no map e chama o respectivo 
		// cmd_obj passando os parametros em uma lista de strings (tokens)
		
	}


private:
	std::map<QString, command *> cmd_map_;
};




#endif
