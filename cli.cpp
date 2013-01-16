#include <QStringList>
#include <list>

#include <cli.h>
#include <exception.h>
#include <view_manager.h>

#include <QEvent>
#include <QKeyEvent>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <debug.h>
#include <exception.h>

lgmx::cli::cli(view_manager *manager) : QWidget(manager)
{
	manager_ = manager;
	
	cmd_history_ = new std::list<QString>();
	curr_cmd_ = cmd_history_->begin();
	
	result_ = new QLabel(this);
	result_->hide();

	input_ = new QLineEdit(this);
	input_->installEventFilter(this);
	
	layout_ = new QHBoxLayout(this);
	layout_->addWidget(input_);
	layout_->addWidget(result_);
	result_->setMaximumHeight (20);
	layout_->setContentsMargins(0, 0, 0, 0);
	layout_->setSpacing(0);
	
	this->setContentsMargins (0, 0, 0, 0);
	setLayout(layout_);
	this->setMaximumHeight (22);
	
	add_command(new tab_width_cmd("tabwidth", manager));
	add_command(new version_cmd("version", manager));
	add_command(new line_wrap_cmd("linewrap", manager));
}


lgmx::cli::~cli()
{
	delete cmd_history_;
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
		return (cmd::stat)2;
	}

	return it->second->execute(cmd, result);
}

void lgmx::cli::parse(QString &cmd_str)
{
	


}

/**
 * CLI interface event handler. Handles special keys such as 
 * Enter, to issue a command, and Up and Down keys to repeat 
 * commands.
 */

bool lgmx::cli::eventFilter(QObject *object, QEvent *event)
{
	if (object == input_ && event->type() == QEvent::KeyPress) {
		QKeyEvent *ke = static_cast<QKeyEvent *>(event);

		QString cmd(input_->text());
		QString result;
		cmd::stat status;

		switch (ke->key()) {
		case Qt::Key_Return:
		case Qt::Key_Enter:
			
			if (cmd.isEmpty())
				return true;
			
			std::cout << "command issued: " << cmd.toStdString() << std::endl;
			
			// handle command
			status = execute(cmd, result);

			if (status == cmd::OK) {
				result_->hide();
			} else if (status == cmd::OK_RES) {
				result_->setText("   " + result + "   " );
				result_->setStyleSheet("background-color: rgb(0, 0, 230);");
				result_->show();
			} else {
				result_->setText("   " + result + "   ");
				result_->setStyleSheet("background-color: rgb(200, 0, 0);");
				result_->show();
			}
			
			// add to history
			cmd_history_->push_back(cmd);
			curr_cmd_ = cmd_history_->end();
			input_->clear();
			return true;
			
		case Qt::Key_Up:
			if (curr_cmd_ != cmd_history_->begin()) {
				curr_cmd_--;
				input_->setText(*curr_cmd_);
			}
			return true;
		
		case Qt::Key_Down:
			if (curr_cmd_ == cmd_history_->end() || ++curr_cmd_ == cmd_history_->end())
				input_->clear();
			else
				input_->setText(*curr_cmd_);

			return true;
		
		case Qt::Key_Escape:
			this->hide();
			return true;
		
		default:
			break;
		}
	}
	return false;
}


