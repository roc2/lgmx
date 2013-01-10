#ifndef CLI_H
#define CLI_H

#include <map>
#include <QString>
#include <command.h>
#include <QWidget>

class command;
class view_manager;
class QEvent;
class QLabel;
class QLineEdit;
class QHBoxLayout;

namespace lgmx {

class cli : public QWidget
{
	Q_OBJECT

public:
	cli(view_manager *manager);
	~cli();

	bool add_command(command *cmd);

	cmd::stat execute(QString &cmd_str, QString &result);

	void parse(QString &cmd_str);
		// faz o match do nome do comando no map e chama o respectivo 
		// cmd_obj passando os parametros em uma lista de strings (tokens)

private:
	bool eventFilter(QObject *object, QEvent *event);	


private:
	view_manager *manager_;
	std::map<QString, command *> cmd_map_;
	
	QLabel *result_;
	QLineEdit *input_;
	QHBoxLayout *layout_;
	std::list<QString>::iterator curr_cmd_;
	std::list<QString> *cmd_history_;
};

}


#endif
