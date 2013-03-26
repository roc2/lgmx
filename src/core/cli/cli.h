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

private:
	cli();
	cli(const cli&);
	cli& operator=(const cli&);

public:
	cli(view_manager *manager);
	~cli();

	cmd::stat execute(QString &cmd_str, QString &result);

	void focusInEvent(QFocusEvent*);

private:
	bool eventFilter(QObject *object, QEvent *event);	

	bool add_command(command *cmd);
	void create_commands();
	void destroy_commands();

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
